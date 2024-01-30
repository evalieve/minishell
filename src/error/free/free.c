/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   free.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/08 17:55:46 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/30 18:45:19 by evalieve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	free_double_char(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	clean_shell(t_minishell *mini)
{
	if (mini->line)
	{
		free(mini->line);
		mini->line = NULL;
	}
	if (mini->cmds)
	{
		free_node(mini->cmds);
		mini->cmds = NULL;
	}
}

void	free_mini_struct(t_minishell *mini)
{
	if (mini->line)
		free(mini->line);
	if (mini->pwd)
		free(mini->pwd);
	if (mini->oldpwd)
		free(mini->oldpwd);
	if (mini->cwd)
		free(mini->cwd);
	if (mini->cmds)
		free_node(mini->cmds);
	if (mini->env)
		free_env(mini->env);
	if (mini)
		free(mini);
}
