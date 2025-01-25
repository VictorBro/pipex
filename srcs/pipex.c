/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 02:11:08 by vbronov           #+#    #+#             */
/*   Updated: 2025/01/25 13:54:48 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include "pipex.h"

void	ft_clean_pipex(t_pipex *pa)
{
	ft_free_strs(pa->env_paths);
	pa->env_paths = NULL;
	ft_free_pipes(pa);
	if (pa->in_fd >= 0 && close(pa->in_fd) < 0)
		ft_fprintf(STDERR_FILENO, "close(%d): %s: %s\n",
			pa->in_fd, pa->argv[1], strerror(errno));
	pa->in_fd = -1;
	if (pa->out_fd >= 0 && close(pa->out_fd) < 0)
		ft_fprintf(STDERR_FILENO, "close(%d): %s: %s\n",
			pa->out_fd, pa->argv[pa->argc - 1], strerror(errno));
	pa->out_fd = -1;
}

int	ft_init_pipex(t_pipex *pa, int argc, char *argv[], char *envp[])
{
	pa->in_fd = -1;
	pa->out_fd = -1;
	pa->env_paths = NULL;
	pa->fd_pipe = NULL;
	pa->cmd_count = argc - 3;
	pa->argc = argc;
	pa->argv = argv;
	pa->envp = envp;
	pa->err = 0;
	pa->here_doc = FALSE;
	if (ft_strncmp(pa->argv[1], "here_doc", 9) == 0)
		pa->here_doc = TRUE;
	if (!ft_open_files(pa))
		return (FALSE);
	if (!ft_parse_env(pa))
	{
		ft_clean_pipex(pa);
		return (FALSE);
	}
	if (!ft_open_pipes(pa))
	{
		ft_clean_pipex(pa);
		return (FALSE);
	}
	return (TRUE);
}

/**
 * Executes the child process for pipex.
 * @param pa The pipex structure containing necessary information.
 * @param i The index of the current command in the command list.
 * in a case of cmd_argv[0] == NULL (the command passed is empty "")
 * execve("", (char *const []){"", NULL}, pa->envp) will be executed.
 */
void	ft_child(t_pipex *pa, int i)
{
	char	**cmd_argv;

	if (i == 0)
		ft_first_cmd(pa);
	else if (i == pa->cmd_count - 1)
		ft_last_cmd(pa);
	else
		ft_middle_cmd(pa, i);
	cmd_argv = ft_parse_cmd(pa, pa->argv[i + 2]);
	if (pa->err)
	{
		ft_free_strs(cmd_argv);
		ft_clean_pipex(pa);
		exit(pa->err);
	}
	execve(cmd_argv[0], cmd_argv, pa->envp);
	perror(pa->argv[i + 2]);
	ft_free_strs(cmd_argv);
	ft_clean_pipex(pa);
	exit(EXIT_FAILURE);
}

void	process_fd_and_pipes(t_pipex *pa)
{
	if (pa->out_fd >= 0 && close(pa->out_fd) < 0)
		ft_fprintf(STDERR_FILENO, "close(%d): %s: %s\n",
			pa->out_fd, pa->argv[pa->argc - 1], strerror(errno));
	pa->out_fd = -1;
	if (dup2(pa->fd_pipe[0][PIPE_IN], STDOUT_FILENO) < 0)
	{
		ft_fprintf(STDERR_FILENO, "dup2(%d, %d): %s\n",
			pa->fd_pipe[0][PIPE_IN], STDOUT_FILENO, strerror(errno));
		ft_clean_pipex(pa);
		exit(EXIT_FAILURE);
	}
	ft_free_pipes(pa);
}

int	pipex(t_pipex *pa)
{
	int		i;
	int		pid;

	i = 0;
	while (i < pa->cmd_count)
	{
		pid = fork();
		if (pid == 0)
		{
			if (!pa->here_doc || i > 0)
				ft_child(pa, i);
			else
				ft_heredoc_child(pa);
		}
		else if (pid < 0)
		{
			perror(pa->argv[i + 2]);
			ft_clean_pipex(pa);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	ft_clean_pipex(pa);
	return (ft_wait_pid(pid));
}
