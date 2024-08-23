/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kez-zoub <kez-zoub@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 20:34:24 by kez-zoub          #+#    #+#             */
/*   Updated: 2024/08/23 13:23:51 by kez-zoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	expand_args(t_param *param)
{
	char	**new_cmd_arr;
	char	**expanded;
	char	*str;
	int		i;

	i = 0;
	new_cmd_arr = array_join(NULL, NULL);
	if (!new_cmd_arr)
		return (1);
	while (param->ast->cmd->simple_cmd[i])
	{
		expanded = expand_str(param->ast->cmd->simple_cmd[i], param);
		if (!expanded)
			return (1);
		new_cmd_arr = array_join(new_cmd_arr, expanded);
		if (!new_cmd_arr)
			return (1);
		i++;
	}
	free_array(param->ast->cmd->simple_cmd);
	param->ast->cmd->simple_cmd = new_cmd_arr;
	return (0);
}

int	expand_redir(t_param *param)
{
	t_redir	*current;
	char	**expndd_arr;

	current = param->ast->cmd->redirs;
	while (current)
	{
		expndd_arr = expand_str(current->filename, param);
		if (!expndd_arr)
			return (1);
		if (!expndd_arr[0] || expndd_arr[1])
		{
			free(current->filename);
			free_array(expndd_arr);
			current->filename = NULL;
		}
		else
		{
			free(current->filename);
			current->filename = expndd_arr[0];
			free(expndd_arr);
		}
		current = current->next;
	}
	return (0);
}

int	expand_wild_args(t_param *param)
{
	char	**old_arr;
	char	**new_arr;
	int		i;

	old_arr = param->ast->cmd->simple_cmd;
	if (!old_arr[0])
		return (0);
	new_arr = array_append(NULL, NULL);
	if (!new_arr)
		return (1);
	i = 0;
	while (old_arr[i])
	{
		if (matched_names(&new_arr, old_arr[i]))
			return (free_array_int(new_arr));
		i++;
	}
	free_array(old_arr);
	param->ast->cmd->simple_cmd = new_arr;
	return (0);
}

int	expand_wild_redir(t_param *param)
{
	t_redir	*current;
	char	**arr;

	current = param->ast->cmd->redirs;
	while (current)
	{
		if (current->filename)
		{
			arr = array_append(NULL, NULL);
			if (!arr || matched_names(&arr, current->filename))
				return (1);
			free(current->filename);
			current->filename = NULL;
			if (arr[1] || !arr[0][0])
				free_array(arr);
			else
			{
				current->filename = arr[0];
				free(arr);
			}
		}
		current = current->next;
	}
	return (0);
}

int	expander(t_param *param)
{
	char	**cmd;
	int		flag;

	flag = 0;
	if (!param || !param->ast || !param->ast->cmd)
		return (1);
	if (expand_args(param) || expand_redir(param)
		|| expand_wild_args(param) || expand_wild_redir(param))
	{
		clean_ast(param->head);
		exit(EXIT_FAILURE);
	}
	return (0);
}
