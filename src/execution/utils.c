/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laoubaid <laoubaid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 11:01:39 by laoubaid          #+#    #+#             */
/*   Updated: 2024/08/20 00:20:22 by laoubaid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

char	*join_optclean(char *s1, char *s2, int flag)
{
	int		i;
	char	*join;
	char	*tmp;

	i = 0;
	tmp = s1;
	if (!s1 && !s2)
		return (0);
	join = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!join)
		return (0);
	if (s1)
		while (*s1)
			join[i++] = *s1++;
	s1 = s2;
	if (s2)
		while (*s2)
			join[i++] = *s2++;
	if (flag == 1 || flag == 3)
		free(tmp);
	if (flag == 2 || flag == 3)
		free(s1);
	return (join[i] = 0, join);
}

void	set_last_arg(t_param *param, char *str, char **arr)
{
	int	i;

	i = 0;
	if (str)
	{
		env_edit(param, "_", str, 1);
		return ;
	}
	if (arr && arr[0])
	{
		i = 1;
		while (arr[i])
			i++;
		env_edit(param, "_", arr[i - 1], 1);
	}
}
