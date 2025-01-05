/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 02:11:29 by vbronov           #+#    #+#             */
/*   Updated: 2025/01/05 02:11:32 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pipex.h"
/**
 * Frees the memory allocated for an array of strings.
 * in a case of error while doing ft_split, split is 
 * using calloc so it will free all 
 * the substrings until he encounters a NULL pointer.
 */
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
		pa->in_fd = open(pa->argv[1], O_RDONLY, 0644);
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
/**
 * Parses the command and returns an array of arguments.
 * checking that the env is splitted without errors, 
 * than verifying if command is an absolute path or starts with "./" or "../",
 * returns the original command arguments.
 * Otherwise, searches for the command in the environment paths and returns the updated command arguments.
 *if access returns 0, the command is valid, so return the updated command arguments.
 * @param pa The pipex structure containing environment paths.
 * @param cmd The command to be parsed.
 * @return The parsed command arguments.
 */

char	**ft_parse_cmd(t_pipex *pa, char *cmd)
{
	int		i;
	char	*path;
	char	**cmd_argv;

	cmd_argv = ft_split(cmd, ' ');
	if (!cmd_argv || !cmd_argv[0] || cmd_argv[0][0] == '/' ||
			!ft_strncmp(cmd_argv[0], "./", 2) ||
			!ft_strncmp(cmd_argv[0], "../", 3) ||
			!pa->env_paths)
		return (cmd_argv);
	i = 0;
	while (pa->env_paths[i])
	{
		path = ft_form_path(pa->env_paths[i], cmd_argv[0]);
		if (path && access(path, X_OK) == 0)
		{
			free(cmd_argv[0]);
			cmd_argv[0] = path;
			return (cmd_argv);
		}
		free(path);
		i++;
	}
	return (cmd_argv);
}
