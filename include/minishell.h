/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:33:39 by evalieve      #+#    #+#                 */
/*   Updated: 2023/12/21 13:14:22 by evalieve      ########   odam.nl         */
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
	E_INVALID_ARGUMENT = 128,
	E_CTRL_C = 130,
	E_CTRL_BACKSLASH = 131,
	E_UNKNOWN = 255,
	// status nums -> als status -42 is dan error minishell en exit-> -1
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
	// char			*path;
	char			**args;
	// ohjee ?? executor
	int				fd_out;
	int				fd_in;
	// ohjee ?? executor
	bool			builtin;
	bool			absolute;
	struct s_redir	*in;
	struct s_redir	*out;
	struct s_cmds	*next;
	struct s_cmds	*prev;
}				t_cmds;

typedef struct s_exec
{
	int				pipe[2];
	int				prev_read;
	pid_t			pid;	
}				t_exec;

typedef struct	s_minishell
{
	t_env			*env;
	int				status;
	bool			exit; //?
	char			*pwd;
	char			*oldpwd;
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
void	redirect(t_cmds *cmd);
char	**env_to_envp(t_env *env);
char	*get_path(char *cmd, t_env *env);
int	get_fd_out(t_cmds *cmd);
int	ft_strcmp(const char *s1, const char *s2);

/* TOKENIZER FILE */
t_cmds	*parse(t_minishell *minishell);
void printmini(t_minishell *mini);
t_tokens *idword(t_tokens *tokens);
void    remove_white(t_tokens *head);
t_cmds *makenodes(t_tokens *tokens);
char **ft_addargs(t_tokens *tokens);
t_tokens *expand(t_tokens *list, t_minishell *mini);
int	check_lim(t_tokens *node);
char *ft_expand(char *line, t_env *env, t_minishell *mini);
char *ft_replace(char *line, char *var, char *value, int start);
char *check_var(char *line);
char *find_var(char *var, t_env *env);
t_tokens *idtokens(t_tokens *list);
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
char	*ft_substr(char const *s, unsigned int start, size_t len);
char			*ft_strdup(const char *src);
size_t	ft_strlen(const char *str);
void *ft_malloc(size_t size);
void	fatal(char *str);
char	*ft_strchr_delim(const char *s);
int	is_delim(char c);
void printlist(t_tokens *list);
int	ft_memcmp(const void *str1, const void *str2, size_t n);
int	iswhspace(char *str);
void	ft_putstr_fd(char *s, int fd);
int	ft_isalpha(int c);
int	ft_isdigit(int c);

/* SIGNALS */
void signals(void);
void signal_ctrl_c(int signum);
void signal_ctrl_d(void);

#endif