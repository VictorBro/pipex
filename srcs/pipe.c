/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 02:10:57 by vbronov           #+#    #+#             */
/*   Updated: 2025/01/05 02:11:01 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "pipex.h"

void	ft_free_pipes(t_pipex *pa)
{
	int	i;

	if (!pa->fd_pipe)
		return ;
	i = 0;
	while (i < pa->cmd_count - 1)
	{
		if (pa->fd_pipe[i][PIPE_OUT] >= 0 &&
				close(pa->fd_pipe[i][PIPE_OUT]) < 0)
			ft_fprintf(STDERR_FILENO, "close(%d): %s: %s\n",
				pa->fd_pipe[i][PIPE_OUT], "pipe", strerror(errno));
		if (pa->fd_pipe[i][PIPE_IN] >= 0 &&
				close(pa->fd_pipe[i][PIPE_IN]) < 0)
			ft_fprintf(STDERR_FILENO, "close(%d): %s: %s\n",
				pa->fd_pipe[i][PIPE_IN], "pipe", strerror(errno));
		i++;
	}
	free(pa->fd_pipe);
	pa->fd_pipe = NULL;
}

int	ft_open_pipes(t_pipex *pa)
{
	int	i;

	pa->fd_pipe = malloc((pa->cmd_count - 1) * sizeof(int [2]));
	if (!pa->fd_pipe)
	{
		ft_fprintf(STDERR_FILENO, "failed alloc pipes: %s\n", strerror(errno));
		return (FALSE);
	}
	i = 0;
	while (i < pa->cmd_count - 1)
	{
		pa->fd_pipe[i][PIPE_OUT] = -1;
		pa->fd_pipe[i++][PIPE_IN] = -1;
	}
	i = 0;
	while (i < pa->cmd_count - 1)
	{
		if (pipe(pa->fd_pipe[i]) < 0)
		{
			ft_fprintf(STDERR_FILENO, "pipe %d: %s\n", i, strerror(errno));
			return (FALSE);
		}
		i++;
	}
	return (TRUE);
}

void	ft_first_cmd(t_pipex *pa)
{
	if (pa->out_fd >= 0 && close(pa->out_fd) < 0)
		ft_fprintf(STDERR_FILENO, "close(%d): %s: %s\n",
			pa->out_fd, pa->argv[pa->argc - 1], strerror(errno));
	pa->out_fd = -1;
	if (dup2(pa->in_fd, STDIN_FILENO) < 0)
	{
		ft_fprintf(STDERR_FILENO, "dup2(%d, %d): %s\n",
			pa->in_fd, STDIN_FILENO, strerror(errno));
		ft_clean_pipex(pa);
		exit(EXIT_FAILURE);
	}
	if (pa->in_fd >= 0 && close(pa->in_fd) < 0)
		ft_fprintf(STDERR_FILENO, "close(%d): %s: %s\n",
			pa->in_fd, pa->argv[1], strerror(errno));
	pa->in_fd = -1;
	if (dup2(pa->fd_pipe[0][PIPE_IN], STDOUT_FILENO) < 0)
	{
		ft_fprintf(STDERR_FILENO, "dup2(%d, %d): %s\n",
			pa->fd_pipe[0][PIPE_IN], STDOUT_FILENO, strerror(errno));
		ft_clean_pipex(pa);
		exit(EXIT_FAILURE);
	}
	ft_free_pipes(pa);
}

void	ft_last_cmd(t_pipex *pa)
{
	if (pa->in_fd >= 0 && close(pa->in_fd) < 0)
		ft_fprintf(STDERR_FILENO, "close(%d): %s: %s\n",
			pa->in_fd, pa->argv[1], strerror(errno));
	pa->in_fd = -1;
	if (dup2(pa->fd_pipe[pa->cmd_count - 2][PIPE_OUT],
		STDIN_FILENO) < 0)
	{
		ft_fprintf(STDERR_FILENO, "dup2(%d, %d): %s\n",
			pa->fd_pipe[pa->cmd_count - 2][PIPE_OUT],
			STDIN_FILENO, strerror(errno));
		ft_clean_pipex(pa);
		exit(EXIT_FAILURE);
	}
	ft_free_pipes(pa);
	if (dup2(pa->out_fd, STDOUT_FILENO) < 0)
	{
		ft_fprintf(STDERR_FILENO, "dup2(%d, %d): %s\n",
			pa->out_fd, STDOUT_FILENO, strerror(errno));
		ft_clean_pipex(pa);
		exit(EXIT_FAILURE);
	}
	if (pa->out_fd >= 0 && close(pa->out_fd) < 0)
		ft_fprintf(STDERR_FILENO, "close(%d): %s: %s\n",
			pa->out_fd, pa->argv[pa->argc - 1], strerror(errno));
	pa->out_fd = -1;
}

void	ft_middle_cmd(t_pipex *pa, int i)
{
	if (pa->in_fd >= 0 && close(pa->in_fd) < 0)
		ft_fprintf(STDERR_FILENO, "close(%d): %s: %s\n",
			pa->in_fd, pa->argv[1], strerror(errno));
	pa->in_fd = -1;
	if (pa->out_fd >= 0 && close(pa->out_fd) < 0)
		ft_fprintf(STDERR_FILENO, "close(%d): %s: %s\n",
			pa->out_fd, pa->argv[pa->argc - 1], strerror(errno));
	pa->out_fd = -1;
	if (dup2(pa->fd_pipe[i - 1][PIPE_OUT], STDIN_FILENO) < 0)
	{
		ft_fprintf(STDERR_FILENO, "dup2(%d, %d): %s\n",
			pa->fd_pipe[i - 1][PIPE_OUT], STDIN_FILENO, strerror(errno));
		ft_clean_pipex(pa);
		exit(EXIT_FAILURE);
	}
	if (dup2(pa->fd_pipe[i][PIPE_IN], STDOUT_FILENO) < 0)
	{
		ft_fprintf(STDERR_FILENO, "dup2(%d, %d): %s\n",
			pa->fd_pipe[i][PIPE_IN], STDOUT_FILENO, strerror(errno));
		ft_clean_pipex(pa);
		exit(EXIT_FAILURE);
	}
	ft_free_pipes(pa);
}
