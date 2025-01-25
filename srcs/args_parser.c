/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 21:05:35 by vbronov           #+#    #+#             */
/*   Updated: 2025/01/25 15:18:04 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "pipex.h"

static char	*parse_one_token(const char *s, int *i)
{
	t_builder	b;
	char		c;
	int			res;

	b.str = NULL;
	b.len = 0;
	b.cap = 0;
	while (s[*i])
	{
		res = handle_special_char(s, i, &b);
		if (res == 2)
			break ;
		if (res == 1)
			continue ;
		if (res == -1)
			break ;
		c = s[*i];
		if (!builder_append(&b, c))
			break ;
		(*i)++;
	}
	return (builder_finalize(&b));
}

// Function to split a command string into arguments, handling quotes
// < in.txt cat | grep -o -E 'is | the' > out.txt
// ./pipex in.txt "cat" "grep -o -E 'is | the'" out.txt
static char	**split_command(char *cmd)
{
	t_list	*tokens;
	char	*token;
	int		i;
	t_list	*new;

	tokens = NULL;
	i = 0;
	while (cmd && cmd[i])
	{
		while (is_space(cmd[i]))
			i++;
		if (!cmd[i])
			break ;
		token = parse_one_token(cmd, &i);
		if (!token)
			break ;
		new = ft_lstnew(token);
		if (!new)
		{
			free(token);
			break ;
		}
		ft_lstadd_back(&tokens, new);
	}
	return (list_to_array(tokens));
}

static char	**handle_absolute_path(t_pipex *pa, char **cmd_argv)
{
	if (access(cmd_argv[0], X_OK) != 0)
	{
		ft_fprintf(STDERR_FILENO, "%s: %s\n", cmd_argv[0], strerror(errno));
		pa->err = PERMISSION_DENIED;
		return (cmd_argv);
	}
	return (cmd_argv);
}

static char	**search_in_env_paths(t_pipex *pa, char **cmd_argv)
{
	int		i;
	char	*path;

	i = 0;
	while (pa->env_paths && pa->env_paths[i])
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
	ft_fprintf(STDERR_FILENO, "%s: command not found\n", cmd_argv[0]);
	pa->err = COMMAND_NOT_FOUND;
	return (cmd_argv);
}

/**
 * Parses the command and returns an array of arguments.
 * checking that the env is splitted without errors, 
 * than verifying if command is an absolute path or starts with "./" or "../",
 * returns the original command arguments.
 * Otherwise, searches for the command in the environment paths and returns 
 * the updated command arguments.
 * if access returns 0, the command is valid,
 * so return the updated command arguments.
 * @param pa The pipex structure containing environment paths.
 * @param cmd The command to be parsed.
 * @return The parsed command arguments.
 */
char	**ft_parse_cmd(t_pipex *pa, char *cmd)
{
	char	**cmd_argv;

	cmd_argv = split_command(cmd);
	if (!cmd_argv || !cmd_argv[0])
	{
		ft_fprintf(STDERR_FILENO, "'': command not found\n");
		pa->err = COMMAND_NOT_FOUND;
		return (cmd_argv);
	}
	if (cmd_argv[0][0] == '/' || !ft_strncmp(cmd_argv[0], "./", 2) ||
			!ft_strncmp(cmd_argv[0], "../", 3))
		return (handle_absolute_path(pa, cmd_argv));
	return (search_in_env_paths(pa, cmd_argv));
}
