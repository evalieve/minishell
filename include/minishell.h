/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:33:39 by evalieve      #+#    #+#                 */
/*   Updated: 2024/01/30 18:33:31 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "../libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <limits.h>

#define ERROR -1
#define SUCCESS 0
#define FAILURE 1
#define READ_END 0
#define WRITE_END 1
#define CHILD 0
#define OLD_STATUS -2 // beter andere naam geven want dit betekent voor meerdere cases dat de huidige exit status geruikt moet worden
#define NON_NUMERIC -3
#define UNSET -4

#define CWD_E_MESSAGE "cd: error retrieving current directory: getcwd: cannot access parent directories"

typedef enum e_signal
{
	S_PARENT,
	S_CHILD,
	S_HEREDOC,
}			t_signal;

typedef enum e_word
{
	CMD,
	FIL,
	LIM,
	INPUT,
} t_word;

typedef enum e_type
{
	WORD,
	RDOUT,
	RDAPPND,
	RDHDOC,
	RDIN,
	PIPE,
	WHITE,
} t_type;

typedef enum e_status
{
	E_ERROR = -1,
	E_SUCCESS = 0,
	E_FAILURE = 1,
	E_COMMAND_NOT_FOUND = 127,
	E_NO_SUCH_FILE_OR_DIRECTORY = 127,
	E_INVALID_ARGUMENT = 128,
	E_SIGINT = 130,
	E_SIGQUIT = 131,
	E_UNKNOWN = 255,
	E_SYNTAX_ERROR = 258,
}	t_status;

typedef struct	s_env
{
	char			*key;
	char			*value;
	bool			equal_sign;
	struct s_env	*next;
	struct s_env	*prev;
}				t_env;

typedef struct s_tokens
{
	char *value;
	enum e_type type;
	enum e_word word;
	int		quote;
	struct s_tokens *next;
	struct s_tokens *prev;
	size_t	len;
} t_tokens;

typedef struct s_redir
{
	char *file;
	int type;
	struct s_redir *next;
	struct s_redir *prev;
} t_redir;


typedef struct s_cmds
{
	char			*cmd;
	char			**args;
	int				fd_out;
	int				fd_in;
	int				pipe[2];
	pid_t			pid;
	bool			builtin;
	bool			absolute;
	struct s_redir	*redir;
	struct s_cmds	*next;
	struct s_cmds	*prev;
}				t_cmds;

typedef struct s_exec
{
	int				pipe[2];
	int				prev_read;
}				t_exec;

typedef struct	s_minishell
{
	t_env			*env;
	int				status;
	bool			exit;
	bool			simple;
	char			*pwd;
	char			*oldpwd;
	char			*cwd;
	char 			*line;
	struct s_cmds	*cmds;
}				t_minishell;

typedef struct	s_builtin
{
	const char		*cmd;
	void			(*func)(t_cmds *cmds, t_minishell *minishell);
}				t_builtin;

/* BUILTINS */
// cd
void	builtin_cd(t_cmds *cmd, t_minishell *minishell);
char	*return_value(t_env *env, char *key);

// echo
void	builtin_echo(t_cmds *cmd, t_minishell *minishell);
bool	is_flag(char *arg);

// env
void	builtin_env(t_cmds *cmd, t_minishell *minishell);
void	print_env_line(t_env *env, int fd);

// exit
void	builtin_exit(t_cmds *cmd, t_minishell *minishell);

//export
void	builtin_export(t_cmds *cmd, t_minishell *minishell);
void	print_env(t_minishell *minishell, int fd); // dit is wel heel lelijk met print_env_line ...
bool	validate_key(char *arg); // key check 
bool	key_exist(t_env *env, char *arg);
void	change_value(t_env *env, char *arg);
void	add_to_env(t_env *env, char *arg, bool equal_sign);
bool	equal_sign_exist(char *arg);
char	*get_value(char *arg);
char	*get_key(char *arg);

// pwd
void	builtin_pwd(t_cmds *cmd, t_minishell *minishell);

// unset
void	builtin_unset(t_cmds *cmd, t_minishell *minishell);
void	key_encounter_unset(t_env *env, char *key);
void	delete_from_env(t_minishell *minishell, t_env *key);

/* ENVIRONMEMT */
t_minishell	*init_struct(char *envp[]);
t_env	*env_init(char *envp[]);
void	node_init(t_env **env, char *envp);
char	*split_key(char *envp);
char	*split_value(char *envp);

/* EXECUTOR */
void	executor(t_minishell *minishell);
void	exec_pipe(t_cmds *cmds, t_exec *exec, t_minishell *minishell);
void	exec_command(t_cmds *cmd, t_minishell *minishell);
void	exec_builtin(t_cmds *cmd, t_minishell *minishell);
// static t_builtin	builtin_lookup(char *cmd);
int	redirect(t_cmds *cmd);
char	**env_to_envp(t_env *env);
char	*get_path(char *cmd, t_env *env);
int	get_fd_out(t_cmds *cmd);
int	ft_strcmp(const char *s1, const char *s2);

/* TOKENIZER FILE */
t_cmds	*tokenize(t_minishell *minishell);
void printmini(t_minishell *mini);
t_tokens *idword(t_tokens *tokens);
t_tokens	*remove_white(t_tokens *head);
t_cmds *parse(t_tokens *tokens);
char **ft_addargs(t_tokens *tokens);
char	**alloc_args(char **args, int size);
t_tokens *idtokens(t_tokens *list, t_minishell *minishell);
t_tokens *mergetokens(t_tokens *list);
int	compvalue(t_tokens *list, t_tokens *next);
void    combine_words(t_tokens *head);
t_tokens	*quotes(char *line);
int closedquote(char *line, char quote);
t_tokens *split_input(t_tokens *old);
void	ft_append(char **line, char *buf, size_t l);
t_tokens	*ft_lstadd(t_tokens *lst, t_tokens *new);
t_redir	*ft_redirnew(char *file, t_type type);
t_redir	*ft_rediradd(t_redir *head, t_redir *new);
t_cmds *ft_nodeadd(t_cmds *lst, t_cmds *new);
void	ft_lstremove(t_tokens *lst);
t_tokens *ft_lstfirst(t_tokens *lst);
t_tokens	*ft_lstlast_token(t_tokens *lst);
t_tokens	*ft_lstnew_token(char *content, int quote);
t_cmds *ft_nodenew(void);
void *ft_malloc(size_t size);
char	*ft_strchr_delim(const char *s);
bool	is_delim(char c);
void printlist(t_tokens *list);
int	ft_memcmp(const void *str1, const void *str2, size_t n);
bool	iswhspace(char *str);
bool ft_isabsolute(char *command);
bool ft_checkbi(char *command);
t_tokens	*comp_merge(t_tokens *node);
void	assign_type(t_tokens *node);
t_tokens	*split_delim(t_tokens *last, char *new, char *rest);
t_cmds	*parse_loop(t_tokens *tokens, t_cmds *list);

/* REDIRECTIONS */
char	*heredoc_loop(char *line, t_cmds *node);
void	handle_redir(t_cmds *node);
//void	handle_red_out(t_cmds *node);
void	handle_red_out(t_cmds *node, t_redir *redir);
//void	handle_red_in(t_cmds *node);
void	handle_red_in(t_cmds *node, t_redir *redir);
bool	handle_redir_out_loop(t_cmds *node, t_redir *tmp);
bool	handle_redir_in_loop(t_cmds *node, t_redir *tmp);

/* EXPANDER */

t_tokens	*expand(t_tokens *list, t_minishell *mini);
char	*ft_expand(char *line, t_minishell *mini);
char	*expand_exit(char *line, t_minishell *mini, int i);
char	*expand_var(char *line, t_minishell *mini, int i);

bool	check_lim(t_tokens *node);
char *ft_replace(char *line, char *var, char *value, int start);
char *check_var(char *line);
char *find_var(char *var, t_env *env);

/* ERROR */

int	check_syntax(t_tokens *tokens, t_minishell *mini);
void	free_env(t_env *env);
void	free_node(t_cmds *node);
void	free_redir(t_redir *redir);
void	free_args(char **args);
void	free_list(t_tokens *list);
void	fatal(char *str, char *pstr);
void	non_fatal(char *str, char *pstr);
void	free_mini_struct(t_minishell *mini);
void	clean_shell(t_minishell *mini);
void	free_double_char(char **str);
void	error_message(char *cmd, char *arg, char *message);

void *ft_malloc(size_t size);
int ft_close(int fd);
int ft_open(char *file, int flag, int mode);
pid_t	ft_fork(void);
void	ft_pipe(int *p);
pid_t	ft_waitpid(pid_t pid, int *status, int option);
int	ft_dup2(int fd1, int fd2);
int	ft_execve(char *path, char **argv, char **envp);


/* SIGNALS */
void signals(t_signal sig);
void signal_ctrl_d(void);


/* UTILS??? */
void	check_for_pwd_and_oldpwd(t_minishell *minishell, char *arg, bool equal_sign);
void	set_working_dir(t_minishell *minishell, char *arg_key, char *arg);
void	adopt_wd_value_from_struct(t_minishell *minishell, char *arg_key);
void	print_env_line(t_env *env, int fd);
void	print_env(t_minishell *minishell, int fd);
void	print_export(t_minishell *minishell, int fd);
void	key_encounter_unset(t_env *env, char *key);
void	delete_from_env(t_minishell *minishell, t_env *key);
void	add_to_env(t_env *env, char *arg, bool equal_sign);
char	*get_value(char *arg);
char	*get_key(char *arg);
bool	equal_sign_exist(char *arg);
bool	key_exist(t_env *env, char *arg);
bool	validate_key(char *arg);
void	change_value(t_env *env, char *arg);
void	close_fds(t_cmds *cmd);
long long	str_to_num(char *str);
void	add_to_env(t_env *env, char *arg, bool equal_sign);

void	printlist(t_tokens *list);
void	printnode(t_cmds *node);

#endif