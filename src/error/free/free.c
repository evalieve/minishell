/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   free.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/08 17:55:46 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/26 18:57:51 by marlou        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	free_all(t_minishell *mini)
{
	if (mini->line)
		free(mini->line);
	if (mini->pwd)
		free(mini->pwd);
	if (mini->oldpwd)
		free(mini->oldpwd);
	if (mini->cmds)
		free_node(mini->cmds);
	if (mini->env)
		free_env(mini->env);
}
