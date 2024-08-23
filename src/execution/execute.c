/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kez-zoub <kez-zoub@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 15:07:43 by laoubaid          #+#    #+#             */
/*   Updated: 2024/08/23 13:28:24 by kez-zoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

int	command_execution(t_param *param)
{
	char	**cmd;
	int		exit_status;

	cmd = param->ast->cmd->simple_cmd;
	set_last_arg(param, NULL, cmd);
	heredoc(param);
	exit_status = builtins(param, param->ast->cmd);
	if (exit_status != -1)
		return (exit_status);
	if (!check_if_path(cmd[0]) && getpath(param->env_arr, "PATH=") != -1)
		path(&cmd, (param->env_arr)[getpath(param->env_arr, "PATH=")]);
	if (!fork())
	{
		signal(SIGINT, SIG_DFL);
		redirecte(param->ast->cmd->redirs);
		exit_status = execution_errors(cmd[0]);
		if (exit_status)
			exit(exit_status);
		execve(cmd[0], cmd, param->env_arr);
		if (cmd[0])
			write(2, cmd[0], ft_strlen(cmd[0]));
		write(2, ": command not found\n", 20);
		exit(127);
	}
	wait(&exit_status);
	if (WIFEXITED(exit_status))
		return (WEXITSTATUS(exit_status));
	if (WIFSIGNALED(exit_status))
		return (130);
	return (1);
}


int	handle_operations(t_param *param)
{
	t_ast	*tmp;

	tmp = param->ast->right;
	if (param->ast->type == OR)
	{
		param->ast = param->ast->left;
		// expander(param);
		if (execute(param) != 0)
		{
			param->ast = tmp;
			// expander(param);
			param->exit_status = execute(param);
		}
	}
	else
	{
		param->ast = param->ast->left;
		// expander(param);
		if (execute(param) == 0)
		{
			param->ast = tmp;
			// expander(param);
			param->exit_status = execute(param);
		}
	}
	return (param->exit_status);
}

int	subshell(t_param *param)
{
	int	pid;
	int	exit_status;

	pid = fork();
	if (!pid)
	{
		if (param->ast->cmd != NULL)
			redirecte(param->ast->cmd->redirs);
		param->ast = param->ast->left;
		exit(execute(param));
	}
	wait(&exit_status);
	if (WIFEXITED(exit_status))
		return (WEXITSTATUS(exit_status));
	return (1);
}

t_pipe	*pipeline(t_ast *ast, t_param *param)
{
	t_pipe	*pip;
	t_pipe	*tmp;

	if (ast->left->type == PIPE)
		pip = pipeline(ast->left, param);
	else
	{
		pip = malloc(sizeof(t_pipe));
		param->ast = ast->left;
		expander(param);
		pip->param = param;
		pip->node = NULL;
		if (ast->left->type == LPAREN)
			pip->node = param->ast;
		pip->cmd = param->ast->cmd;
		pip->next = NULL;
	}
	tmp = pip;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = malloc(sizeof(t_pipe));
	tmp = tmp->next;
	param->ast = ast->right;
	expander(param);
	tmp->param = param;
	tmp->node = NULL;
	if (ast->right->type == LPAREN)
		tmp->node = param->ast;
	return (tmp->cmd = param->ast->cmd, tmp->next = NULL, pip);
}

int openfiles(t_param *param)
{
	int	exit_status;

	expander(param);
	// there is a leak after this one
	heredoc(param);
	if (!fork())
		exit(redirecte(param->ast->cmd->redirs));
	wait(&exit_status);
	if (WIFEXITED(exit_status))
		return (WEXITSTATUS(exit_status));
	return (1);
}

int	execute(t_param *param)
{
	int		error;
	t_pipe	*pip;

	error = 0;
	if (param->ast->type == OR || param->ast->type == AND)
		param->exit_status = handle_operations(param);
	else if (param->ast->type == PIPE)
	{
		pip = pipeline(param->ast, param);
		pipe_heredoc(pip);
		param->exit_status = handle_pipe(pip, param->env_arr);
		free_pipe(pip);
	}
	else if (param->ast->type == LPAREN)
		param->exit_status = subshell(param);
	else if (param->ast->cmd->simple_cmd[0])
	{
		error = expander(param);
		if (!error)
			param->exit_status = command_execution(param);
	}
	else
		param->exit_status = openfiles(param);
	return (param->exit_status);
}
