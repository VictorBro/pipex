/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 02:10:20 by vbronov           #+#    #+#             */
/*   Updated: 2025/01/21 21:48:45 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

void	ft_heredoc_child(t_pipex *pa)
{
	char	*line;
	char	*limiter;

	limiter = pa->argv[2];
	process_fd_and_pipes(pa);
	while (TRUE)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line || (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
				&& line[ft_strlen(limiter)] == '\n'))
		{
			free(line);
			break ;
		}
		if (write(STDOUT_FILENO, line, ft_strlen(line)) < 0)
		{
			ft_fprintf(STDERR_FILENO, "write: %s\n", strerror(errno));
			free(line);
			ft_clean_pipex(pa);
			exit(EXIT_FAILURE);
		}
		free(line);
	}
	ft_clean_pipex(pa);
	exit(EXIT_SUCCESS);
}
