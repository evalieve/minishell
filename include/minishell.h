/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: evalieve <evalieve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/06 11:33:39 by evalieve      #+#    #+#                 */
/*   Updated: 2024/02/02 14:13:19 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <signal.h>
# include <stdbool.h>
# include <sys/errno.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>

# define ERROR -1
# define SUCCESS 0
# define FAILURE 1
# define READ_END 0
# define WRITE_END 1
# define CHILD 0
# define CURRENT_STATUS -2
# define NON_NUMERIC -3
# define UNSET -4

# define CD_E_MESSAGE "cd: error retrieving current directory: " 
# define CWD_E_MESSAGE "getcwd: cannot access parent directories: "

typedef enum e_signal
{
	S_PARENT,
	S_CHILD,
	S_HEREDOC,
	S_IGNORE,
}				t_signal;

typedef enum e_word
{
	CMD,
	FIL,
	LIM,
	INPUT,
}			t_word;

typedef enum e_type
{
	WORD,
	RDOUT,
	RDAPPND,
	RDHDOC,
	RDIN,
	PIPE,
	WHITE,
}				t_type;

typedef enum e_status
{
	E_ERROR = -1,
	E_SUCCESS = 0,
	E_CTRL_D = 0,
	E_FAILURE = 1,
	E_COMMAND_NOT_FOUND = 127,
	E_NO_SUCH_FILE_OR_DIRECTORY = 127,
	E_INVALID_ARGUMENT = 128,
	E_SIGINT = 130,
	E_SIGQUIT = 131,
	E_UNKNOWN = 255,
	E_SYNTAX_ERROR = 258,
}				t_status;

typedef struct s_env
{
	char			*key;
	char			*value;
	bool			equal_sign;
	struct s_env	*next;
	struct s_env	*prev;
}				t_env;

typedef struct s_tokens
{
	char			*value;
	enum e_type		type;
	enum e_word		word;
	int				quote;
	struct s_tokens	*next;
	struct s_tokens	*prev;
	size_t			len;
}				t_tokens;

typedef struct s_redir
{
	char			*file;
	int				type;
	struct s_redir	*next;
	struct s_redir	*prev;
}				t_redir;

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

typedef struct s_minishell
{
	t_env			*env;
	int				status;
	bool			exit;
	bool			simple;
	char			*pwd;
	char			*oldpwd;
	char			*cwd;
	char			*line;
	struct s_cmds	*cmds;
}				t_minishell;

typedef struct s_builtin
{
	const char		*cmd;
	void			(*func)(t_cmds *cmds, t_minishell *minishell);
}				t_builtin;

/* BUILTINS */
// cd
char		*return_value(t_env *env, char *key);
void		if_exist_change_pwds(t_minishell *minishell, \
									char *oldpwd, char *pwd);
void		handle_cwd_error(t_minishell *minishell);
void		change_pwds(t_minishell *minishell);
void		builtin_cd(t_cmds *cmd, t_minishell *minishell);

// echo
bool		is_flag(char *arg);
void		builtin_echo(t_cmds *cmd, t_minishell *minishell);

// env
void		print_env_line(t_env *env, int fd);
void		builtin_env(t_cmds *cmd, t_minishell *minishell);

// exit
bool		semicolumn(char *str);
int			calc_status(int status);
int			convert_status(char *str);
bool		exit_check(t_cmds *cmd, t_minishell *minishell, int exit_status);
void		builtin_exit(t_cmds *cmd, t_minishell *minishell);

// export_utils
bool		equal_sign_exist(char *arg);
bool		key_exist(t_env *env, char *arg);
bool		validate_key(char *arg);
char		*get_key(char *arg);
char		*get_value(char *arg);

// export_utils2
void		add_to_env(t_minishell *minishell, char *arg, bool equal_sign);
void		set_working_dir(t_minishell *minishell, char *arg_key, char *arg);
void		adopt_wd_value_from_struct(t_minishell *minishell, char *arg_key);
void		check_for_pwd_and_oldpwd(t_minishell *minishell, \
										char *arg, bool equal_sign);

// export
void		change_value(t_env *env, char *arg);
void		print_export(t_minishell *minishell, int fd);
bool		validate_identifier_export(char *arg);
void		export_variables(t_cmds *cmd, t_minishell *minishell, int i);
void		builtin_export(t_cmds *cmd, t_minishell *minishell);

// pwd
void		builtin_pwd(t_cmds *cmd, t_minishell *minishell);

// unset
void		delete_from_env(t_minishell *minishell, t_env *key);
void		key_unset(t_minishell *minishell, char *arg);
void		builtin_unset(t_cmds *cmd, t_minishell *minishell);

/* ENVIRONMENT */
// envp
char		*split_value(char *envp);
char		*split_key(char *envp);
void		node_init(t_env **env, char *envp);
t_env		*env_init(char *envp[]);
t_minishell	*init_mini_struct(char *envp[]);

/* ERROR */
// free
void		free_list(t_tokens *list);
void		free_args(char **args);
void		free_redir(t_redir *redir);
void		free_node(t_cmds *node);
void		free_env(t_env *env);

// free_utils
void		free_double_char(char **str);
void		clean_shell(t_minishell *mini);
void		free_mini_struct(t_minishell *mini);

// fatal
void		fatal(char *str, char *pstr);
void		non_fatal(char *str, char *pstr);
void		error_message(char *cmd, char *arg, char *message);

// syntax_check
int			check_syntax(t_tokens *tokens);
int			check_pipe_syntax(t_tokens *tokens);
bool		check_redir_type(int type);
int			check_redir_syntax(t_tokens *tokens);

// wrap_func
void		*ft_malloc(size_t size);
int			ft_close(int fd);
int			ft_open(char *file, int flag, int mode);

// wrap_func2
pid_t		ft_fork(void);
void		ft_pipe(int *p);
pid_t		ft_waitpid(pid_t pid, int *status, int option);
int			ft_dup2(int fd1, int fd2);
int			ft_execve(char *path, char **argv, char **envp);

/* EXECUTOR */
// exec_utils
char		**env_to_envp(t_env *env);
void		close_fds(t_cmds *cmd);
char		*find_cmd_path(char **paths, char *cmd);
char		*get_path(char *cmd, t_env *env);
int			redirect(t_cmds *cmd);

// exec_builtin
void		exec_command_path_error(t_minishell *mini, t_cmds *cmd, char *path);
t_builtin	builtin_lookup(char *cmd);
void		exec_builtin(t_cmds *cmd, t_minishell *minishell);
char		*get_path(char *cmd, t_env *env);
int			redirect(t_cmds *cmd);

// exec
void		exec_command(t_cmds *cmd, t_minishell *minishell);
void		exec_pipe(t_cmds *cmds, t_exec *exec, t_minishell *minishell);
void		exec_simple(t_cmds *cmd, t_minishell *minishell);
void		exec_commands_loop(t_minishell *minishell);
void		executor(t_minishell *minishell);

/* PARSER */
// expander_utils
bool		check_lim(t_tokens *node);
char		*check_var(char *line);
char		*ft_replace(char *line, char *var, char *value, int start);
char		*find_var(char *var, t_env *env);

// expander
char		*expand_var(char *line, t_minishell *mini, int i);
char		*expand_exit(char *line, t_minishell *mini, int i);
char		*ft_expand(char *line, t_minishell *mini);
t_tokens	*expand(t_tokens *list, t_minishell *mini);

// redir_utils
int			handle_redir_in_loop(t_cmds *node, t_redir *tmp, t_minishell *mini);
int			handle_redir_out_loop(t_cmds *node, t_redir *tmp);

// redir
int			wait_for_heredoc(pid_t pid, t_minishell *minishell);
int			heredoc_loop(t_cmds *node, t_redir *redir, t_minishell *minishell);
int			handle_red_in(t_cmds *node, t_redir *redir, t_minishell *minishell);
int			handle_red_out(t_cmds *node, t_redir *redir);
int			handle_redir(t_cmds *node, t_minishell *minishell);

// id_tokens
void		assign_type(t_tokens *node);
t_tokens	*idtokens(t_tokens *list, t_minishell *minishell);
bool		idword_check(t_tokens *list, t_tokens *prev, bool cmd);
t_tokens	*idword(t_tokens *tokens);

// merge_add
char		**alloc_args(char **args, int size);
char		**ft_addargs(t_tokens *tokens);
void		combine_words(t_tokens *head);
t_tokens	*comp_merge(t_tokens *node);
t_tokens	*mergetokens(t_tokens *list);

// nodes_redir_utils
t_cmds		*ft_nodenew(void);
t_cmds		*ft_nodeadd(t_cmds *lst, t_cmds *new);
t_redir		*ft_redirnew(char *file, t_type type);
t_redir		*ft_rediradd(t_redir *head, t_redir *new);

// parser
t_cmds		*parse_loop(t_tokens *tokens, t_cmds *list);
t_cmds		*parse(t_tokens *tokens, t_minishell *minishell);

// quotes
int			closedquote(char *line, char quote);
t_tokens	*quotes_loop(char *line, t_tokens *list, int start, int end);
t_tokens	*quotes(char *line);

// split_utils
char		*ft_strchr_delim(const char *s);
void		ft_append(char **line, char *buf, size_t l);
int			compvalue(t_tokens *list, t_tokens *next);
t_tokens	*remove_white(t_tokens *head);

// tokenizer
t_tokens	*split_delim(t_tokens *last, char *new, char *rest);
t_tokens	*split_input(t_tokens *old);
t_tokens	*split_tokens(t_tokens *list);
t_cmds		*tokenize(t_minishell *minishell);

// tokens
t_tokens	*ft_lstnew_token(char *content, int quote);
t_tokens	*ft_lstlast_token(t_tokens *lst);
t_tokens	*ft_lstfirst(t_tokens *lst);
void		ft_lstremove(t_tokens *lst);
t_tokens	*ft_lstadd(t_tokens *lst, t_tokens *new);

// value_check
bool		ft_checkbi(char *command);
bool		ft_isabsolute(char *command);
bool		iswhspace(char *str);
bool		is_delim(char c);

/* SIGNALS */
// sig
void		signal_ctrl_d(t_minishell *minishell);
void		signal_heredoc(int signum);
void		signal_child(int signum);
void		signal_parent(int signum);
void		signals(t_signal sig);

/* UTILS */
// utils
int			ft_strcmp(const char *s1, const char *s2);
long long	str_to_num(char *str);

/* MAIN */
bool		empty_input(char *line);
void		password(void);
void		wait_for_child(t_minishell *minishell);
void		start_minishell(t_minishell *minishell);

#endif