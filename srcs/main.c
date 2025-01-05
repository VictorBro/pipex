/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 02:10:47 by vbronov           #+#    #+#             */
/*   Updated: 2025/01/05 02:10:51 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "pipex.h"

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

int	main(int argc, char *argv[], char *envp[])
{
	t_pipex	pa;

	if (argc != 5)
	{
		ft_fprintf(STDERR_FILENO,
			"usage: %s <infile> <cmd1> <cmd2> <outfile>\n", argv[0]);
		return (USAGE_ERROR);
	}
	if (!ft_init_pipex(&pa, argc, argv, envp))
		return (INIT_ERROR);
	pipex(&pa);
	return (OK);
}
