/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 14:24:34 by vbronov           #+#    #+#             */
/*   Updated: 2025/01/25 14:29:37 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "pipex.h"

void	free_lst_nodes(t_list *tokens)
{
	t_list	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens);
		tokens = tmp;
	}
}

int	ft_wait_pid(int last_pid)
{
	int	pid_tmp;
	int	status;
	int	res;

	while (TRUE)
	{
		pid_tmp = waitpid(-1, &status, 0);
		if (pid_tmp == -1)
		{
			if (errno == ECHILD)
				break ;
			res = errno;
			ft_fprintf(STDERR_FILENO, "waitpid: %s\n", strerror(errno));
			break ;
		}
		if (pid_tmp != last_pid)
			continue ;
		if (WIFEXITED(status))
			res = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			res = 128 + WTERMSIG(status);
	}
	return (res);
}
