/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   syntax_check.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/28 17:31:30 by marlou        #+#    #+#                 */
/*   Updated: 2024/02/01 14:58:12 by marlou        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	check_pipe_syntax(t_tokens *tokens)
{
	t_tokens	*tmp;

	tmp = tokens;
	while (tmp && tmp->next)
	{
		if (tmp->type == PIPE && tmp->next->type == PIPE)
		{
			free_list(tokens);
			ft_putstr_fd(\
			"minishell: Error: syntax error near unexpected token `||'\n", 2);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

bool	check_redir_type(int type)
{
	if (type == RDIN || type == RDOUT || type == RDAPPND || type == RDHDOC)
		return (true);
	return (false);
}

int	check_redir_syntax(t_tokens *tokens)
{
	t_tokens	*tmp;

	tmp = tokens;
	while (tmp && tmp->next)
	{
		if (check_redir_type(tmp->type) == true && tmp->next->type != WORD)
		{
			free_list(tokens);
			ft_putstr_fd("minishell: Error: syntax error\n", 2);
			return (1);
		}
		tmp = tmp->next;
	}
	if (tmp)
	{
		if (check_redir_type(tmp->type) == true || tmp->type == PIPE)
		{
			free_list(tokens);
			ft_putstr_fd(\
	"minishell: Error: syntax error near unexpected token `newline'\n", 2);
			return (1);
		}
	}
	return (0);
}

int	check_syntax(t_tokens *tokens)
{
	int	rv;

	rv = check_pipe_syntax(tokens);
	if (rv == 1)
		return (1);
	rv = check_redir_syntax(tokens);
	if (rv == 1)
		return (1);
	return (0);
}
