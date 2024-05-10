/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kez-zoub <kez-zoub@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 17:32:02 by kez-zoub          #+#    #+#             */
/*   Updated: 2024/05/10 00:05:45 by kez-zoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(void)
{
	char	*buffer;
	int		syntax_error;

	syntax_error = 0;
	shell_signals();
	while (1)
	{
		if (syntax_error)
			ft_putstr_fd("\e[31m➜  \e[36mMiniShell\e[0m ", 1);
		else
			ft_putstr_fd("\e[32m➜  \e[36mMiniShell\e[0m ", 1);
		buffer = get_raw_input();
		syntax_error = parser(buffer);
		free(buffer);
	}
	return (0);
}
