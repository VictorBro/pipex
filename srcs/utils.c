/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 02:11:29 by vbronov           #+#    #+#             */
/*   Updated: 2025/01/25 13:32:18 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "pipex.h"

void	ft_free_strs(char **strs)
{
	int	i;

	if (!strs)
		return ;
	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		strs[i] = NULL;
		i++;
	}
	free(strs);
}

int	ft_parse_env(t_pipex *pa)
{
	int	i;

	i = 0;
	while (pa->envp[i])
	{
		if (ft_strnstr(pa->envp[i], "PATH=", 5) != NULL)
		{
			pa->env_paths = ft_split(pa->envp[i] + 5, ':');
			if (!pa->env_paths)
			{
				ft_fprintf(STDERR_FILENO, "failed to split the env paths\n");
				return (FALSE);
			}
			break ;
		}
		i++;
	}
	return (TRUE);
}

int	ft_open_files(t_pipex *pa)
{
	if (!pa->here_doc)
	{
		pa->in_fd = open(pa->argv[1], O_RDONLY);
		if (pa->in_fd < 0)
			perror(pa->argv[1]);
		pa->out_fd = open(pa->argv[pa->argc - 1],
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	else
		pa->out_fd = open(pa->argv[pa->argc - 1],
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (pa->out_fd < 0)
	{
		perror(pa->argv[pa->argc - 1]);
		if (pa->in_fd >= 0 && close(pa->in_fd) < 0)
			ft_fprintf(STDERR_FILENO, "close(%d): %s: %s\n",
				pa->in_fd, pa->argv[1], strerror(errno));
		return (FALSE);
	}
	return (TRUE);
}

char	*ft_form_path(char *path, char *cmd)
{
	char	*tmp;
	char	*ret;

	tmp = ft_strjoin(path, "/");
	if (!tmp)
	{
		ft_fprintf(STDERR_FILENO, "failed alloc path '%s/': %s\n",
			path, strerror(errno));
		return (NULL);
	}
	ret = ft_strjoin(tmp, cmd);
	if (!ret)
	{
		ft_fprintf(STDERR_FILENO, "failed alloc path '%s%s': %s\n",
			tmp, cmd, strerror(errno));
		free(tmp);
		return (NULL);
	}
	free(tmp);
	return (ret);
}

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
