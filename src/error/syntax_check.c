/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   syntax_check.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: marlou <marlou@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/28 17:31:30 by marlou        #+#    #+#                 */
/*   Updated: 2024/01/26 18:56:26 by marlou        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_syntax(t_tokens *tokens, t_minishell *mini)
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
			mini->status = 1;
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}
