/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mkootstr <mkootstr@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/09/13 16:46:39 by mkootstr      #+#    #+#                 */
/*   Updated: 2024/01/25 16:51:32 by marlou        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_tokens *idword(t_tokens *tokens);

bool ft_checkbi(char *command)
{
	if (ft_strcmp(command, "echo") == 0)
		return (true);
	else if (ft_strcmp(command, "cd") == 0)
		return (true);
	else if (ft_strcmp(command, "pwd") == 0)
		return (true);
	else if (ft_strcmp(command, "export") == 0)
		return (true);
	else if (ft_strcmp(command, "unset") == 0)
		return (true);
	else if (ft_strcmp(command, "env") == 0)
		return (true);
	else if (ft_strcmp(command, "exit") == 0)
		return (true);
	else
		return (false);
}

bool ft_isabsolute(char *command)
{
	int i;

	i = 0;
	while (command[i] != '\0')
	{
		if (command[i] == '/')
		{
			// printf("true ret\n");
			return (true);
		}
		i++;
	}
	return (false);
}

void    remove_white(t_tokens *head);

int	iswhspace(char *str)
{
	if (!str)
		return (1);
	while(*str)
	{
		if (*str != ' ' && *str != '\t' && *str != '\n')
			return (1);
		str++;
	}
	return (0);
}

int	ft_memcmp(const void *str1, const void *str2, size_t n)
{
	size_t			i;
	unsigned char	*s1;
	unsigned char	*s2;

	i = 0;
	s1 = (unsigned char *)str1;
	s2 = (unsigned char *)str2;
	while (i < n)
	{
		if ((s1[i] - s2[i] < 0) || (s1[i] - s2[i] > 0))
		{
			return (s1[i] - s2[i]);
		}
		i++;
	}
	return (0);
}

void printlist(t_tokens *list)
{
	while (list != NULL)
	{
		printf("value = (%s)\n", list->value);
		printf("type = %d\n", list->type);
		printf("word = %d\n", list->word);
		list = list->next;
	}
}

int	is_delim(char c)
{
	if (c == '|' || c == '>' || c == '<' || \
	c == ' ' || c == '\t' || c == '\n')
		return (1);
	else
		return (0);
}

char	*ft_strchr_delim(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (is_delim(s[i]) == 1)
		{
			return ((char *)s + i);
		}
		i++;
	}
	return (NULL);
}

t_cmds *ft_nodenew(void)
{
	t_cmds *new;

	new = (t_cmds *)ft_malloc(1 *sizeof(t_cmds));
	new->cmd = NULL;
	// new->path = NULL;
	new->fd_in = STDIN_FILENO;
	new->fd_out = STDOUT_FILENO;
	new->args = NULL;
	new->next = NULL;
	new->prev = NULL;
	new->builtin = false;
	new->absolute = false;
	new->pid = UNSET;
	new->redir = NULL;
	return (new);
}

t_tokens	*ft_lstnew_token(char *content, int quote)
{
	t_tokens *new;

	new = (t_tokens *)ft_malloc(1 * sizeof(t_tokens));
	new->quote = quote;
	new->value = content;
	new->len = ft_strlen(content);
	new->prev = NULL;
	new->next = NULL;
	new->type = -1;
	new->word = -1;
	return (new);
}

t_tokens	*ft_lstlast_token(t_tokens *lst)
{
	while (lst && lst->next != NULL)
		lst = lst->next;
	return (lst);
}

t_tokens *ft_lstfirst(t_tokens *lst)
{
	while (lst && lst->prev != NULL)
		lst = lst->prev;
	return (lst);
}

void	ft_lstremove(t_tokens *lst)
{
	if (lst->prev)
		lst->prev->next = lst->next;
	if (lst->next)
		lst->next->prev = lst->prev;
	free(lst->value);
	free(lst);
}

t_cmds *ft_nodeadd(t_cmds *lst, t_cmds *new)
{
	if (lst && new)
	{
		while (lst->next)
			lst = lst->next;
		new->prev = lst;
		lst->next = new;
	}
	else if (lst == NULL && new)
		lst = new;
	return (new);
}

t_redir	*ft_rediradd(t_redir *head, t_redir *new)
{
	t_redir *temp;

	temp = head;
	if (!temp)
		return (new);
	while (temp->next)
		temp = temp->next;
	temp->next = new;
	new->prev = temp;
	return (head);
}

t_redir	*ft_redirnew(char *file, t_type type)
{
	t_redir *new;

	new = (t_redir *)ft_malloc(sizeof(t_redir));
	new->file = file;
	new->type = type;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

t_tokens	*ft_lstadd(t_tokens *lst, t_tokens *new)
{
	t_tokens *list;
	list = NULL;
	if (lst && new)
	{
		new->next = lst->next;
		new->prev = lst;
		lst->next = new;
		if (new->next)
			new->next->prev = new;
		list = new;
		return (list);
	}
	else if (lst == NULL && new)
	{
		list = new;
		return (list);
	}
	else
		return (NULL);
}

void	ft_append(char **line, char *buf, size_t l)
{
	char	*new_line;
	size_t	line_size;

	line_size = ft_strlen(*line);
	new_line = (char *)ft_malloc((line_size + 1 + l) * sizeof(char));
	new_line[line_size + l] = '\0';
	while (l--)
		new_line[line_size + l] = buf[l];
	while (line_size--)
		new_line[line_size] = (*line)[line_size];
	free(*line);
	*line = new_line;
}

t_tokens *split_input(t_tokens *old)
{
	char *rest;
	char *new;
	t_tokens *last;

	last = old;
	while (last != NULL && last->quote == 0)
	{
		if (ft_strchr_delim(last->value) != NULL)
		{
			if(ft_strchr_delim(last->value) == last->value)
			{
				rest = (ft_strchr_delim(last->value) + 1);
				new = ft_substr(last->value, 0, 1);
			}
			else
			{
				rest = ft_strchr_delim(last->value);
				new = ft_substr(last->value, 0, ft_strlen(last->value) - ft_strlen(rest));
			}
			if (ft_strlen(rest) > 0)
				ft_lstadd(last, ft_lstnew_token(ft_strdup(rest), 0));
			free(last->value);
			last->value = new;
		}
		last = last->next;
	}
	if (last != NULL)
		return (last);
	else 
		return (ft_lstlast_token(old));
}

int closedquote(char *line, char quote)
{
	int end;

	end = 0;
	while (line[end] != '\0' && line[end] != quote)
		end++;
	if (line[end] == '\0')
	{
		ft_putstr_fd("minishell: syntax error: unclosed quotes\n", 2);
		return (-1);
	}
	else if (line[end] == quote)
		return (end);
	return (end);
}

t_tokens	*quotes(char *line)
{
	t_tokens *list;
	int start;
	int end;
	
	list = NULL;
	start = 0;
	end = 0;
	while (line[start] != '\0')
	{
		while (line[end] != '\0' && line[end] != '\'' && line[end] != '\"')
			end++;
		if (end != start)
			list = ft_lstadd(ft_lstlast_token(list), ft_lstnew_token(ft_substr(line, start, end - start), 0));
		else if (line[end] == '\'' || line[end] == '\"')
		{
			start = end;
			end = closedquote(&line[start + 1], line[start]);
			if (end == -1)
			{
				free_list(list);
				return (NULL);
			}
			list = ft_lstadd(ft_lstlast_token(list), ft_lstnew_token(ft_substr(line, start + 1, end), line[start]));
			end = end + start + 2;
		}
		start = end;
	}
	return (ft_lstlast_token(list));
}

void    combine_words(t_tokens *head)
{
    t_tokens *temp;

    temp = head;
    while (temp)
    {
        if (temp->type == WORD && temp->next && temp->next->type == WORD)
        {
            ft_append(&(temp->value), temp->next->value, temp->next->len);
            if (!temp->value)
                return ;
            temp->len += temp->next->len;
            ft_lstremove(temp->next);
        }
        else
            temp = temp->next;
    }
}

int	compvalue(t_tokens *list, t_tokens *next)
{
	if (list->quote == 0 && next->quote == 0)
	{
		if (ft_strlen(list->value) == 1 && ft_strlen(next->value) == 1)
		{
			if (list->value[0] == '<' || list->value[0] == '>')
				return(ft_strcmp(list->value, next->value));
		}
	}
	return (1);
}

t_tokens *mergetokens(t_tokens *list)
{
	t_tokens *node;

	node = list;
	while (node && node->next)
	{
		if (compvalue(node, node->next) == 0)
		{
			ft_append(&(node->value), node->next->value, ft_strlen(node->next->value));
			ft_lstremove(node->next);
			node = node->next;
		}
		else if (iswhspace(node->value) == 0 && node->quote == 0)
		{
			free(node->value);
			node->value = ft_strdup(" ");
			if (iswhspace(node->next->value) == 0 && node->next->quote == 0)
				ft_lstremove(node->next);
			if (node->next && iswhspace(node->next->value) != 0)
				node = node->next;
		}
		else
			node = node->next;
	}
	return (list);
}

t_tokens *idtokens(t_tokens *list, t_minishell *minishell)
{
	t_tokens *node;

	node = list;
	while (node)
	{
		if (node->quote == 1 || node->quote == 2)
			node->type = WORD;
		else if (ft_strlen(node->value) == 1)
		{
			if (node->value[0] == '|')
				node->type = PIPE;
			else if (node->value[0] == '<')
				node->type = RDIN;
			else if (node->value[0] == '>')
				node->type = RDOUT;
			else if (node->value[0] == ' ')
				node->type = WHITE;
			else
				node->type = WORD;
		}
		else if (ft_strlen(node->value) == 2)
		{
			if (ft_strcmp(node->value, ">>") == 0)
				node->type = RDAPPND;
			else if (ft_strcmp(node->value, "<<") == 0)
				node->type = RDHDOC;
			else
				node->type = WORD;
		}
		else
			node->type = WORD;
		node = node->next;
	}
	list = expand(list, minishell);
	combine_words(list);
	remove_white(list);
	list = idword(list);
	return (list);
}

char **ft_addargs(t_tokens *tokens)
{
	int i;
	char **args;
	t_tokens *temp;

	i = 0;
	args = NULL;
	temp = tokens;
	while (temp && (temp->word == INPUT || temp->word == CMD))
	{
		i++;
		temp = temp->next;
	}
	temp = tokens;
	args = (char **)ft_malloc((i + 1) * sizeof(char *));
	args[i] = NULL;
	int j = i;
	i = 0;
	while (i < j)
	{
		args[i] = ft_strdup(temp->value);
		i++;
		temp = temp->next;
	}
	return (args);
}

t_cmds *makenodes(t_tokens *tokens)
{
	t_cmds *list;

	list = NULL;
	while (tokens != NULL)
	{
		list = ft_nodeadd(list, ft_nodenew());
		while (tokens && tokens->type != PIPE)
		{
			if (tokens->word == CMD)
			{
				list->cmd = ft_strdup(tokens->value);
				list->args = ft_addargs(tokens);
				list->absolute = ft_isabsolute(list->cmd);
				list->builtin = ft_checkbi(list->cmd);
			}
			else if (tokens->word == FIL || tokens->word == LIM)
			{
				if (tokens->prev->type == RDOUT || tokens->prev->type == RDAPPND)
					list->redir = ft_rediradd(list->redir, ft_redirnew(tokens->value, tokens->prev->type));
				else if (tokens->prev->type == RDHDOC || tokens->prev->type == RDIN)
					list->redir = ft_rediradd(list->redir, ft_redirnew(tokens->value, tokens->prev->type));
				else
					list->redir = ft_rediradd(list->redir, ft_redirnew(tokens->value, RDOUT));
			}
			tokens = tokens->next;
		}
		handle_redir(list);
		if (tokens)
			tokens = tokens->next;
	}
	while (list && list->prev)
		list = list->prev;
	return (list);
}


void    remove_white(t_tokens *head)
{
    t_tokens *tmp;
	t_tokens *prev;

    tmp = head;
    while (tmp)
    {
        if (tmp->type == WHITE)
		{
			if (tmp->prev)
				prev = tmp->prev;
			else if (tmp->next)
				prev = tmp->next;
			else
				prev = NULL;
			ft_lstremove(tmp);
			tmp = prev;
		}
        else
            tmp = tmp->next;
    }
}

t_tokens *idword(t_tokens *tokens)
{
	t_tokens *list;
	t_tokens *prev;
	bool cmd;

	cmd = false;
	list = tokens;
	prev = tokens;
	while (list)
	{
		if (list->type == PIPE)
			cmd = false;
		else if (list->type == WORD)
		{
			if (list->prev)
				prev = list->prev;
			if ((!list->prev || prev->type == PIPE || prev->word == FIL) && cmd == false)
			{
				list->word = CMD;
				cmd = true;
			}
			else if (prev->word == CMD || prev->word == INPUT)
				list->word = INPUT;
			else if (prev->type == RDHDOC)
				list->word = FIL;
			else
				list->word = FIL;
		}
		prev = NULL;
		list = list->next;
	}
	return (ft_lstfirst(tokens));
}

t_cmds	*parse(t_minishell *minishell)
{
	t_tokens *list;
	char *line;

	// spaties en tabs skippen aan het begin!!!!
	while (minishell->line && (*minishell->line == ' ' || *minishell->line == '\t' || *minishell->line == '\n'))
		minishell->line++;
	if (minishell->line == NULL || *minishell->line == '\0')
		return (NULL);
	line = minishell->line;
	list = quotes(line);
	if (list == NULL)
	{
		minishell->status = E_SYNTAX_ERROR;
		return (NULL);
	}
	while (list->prev)
		list = list->prev;
	while (1)
	{
		if (list->quote == 0)
		{
			list = split_input(list);
			if (ft_lstlast_token(list) == list)
				break ;
		}
		else if(list->next != NULL)
			list = list->next;
		else
			break ;
	}
	list = mergetokens(ft_lstfirst(list));
	list = idtokens(list, minishell);
	if (check_syntax(list, minishell) == 1)
	{
		minishell->status = E_SYNTAX_ERROR;
		return (NULL);
	}
	minishell->cmds = makenodes(list);
	free_list(list);
	return (minishell->cmds);
}
