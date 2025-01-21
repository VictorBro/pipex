/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 21:05:35 by vbronov           #+#    #+#             */
/*   Updated: 2025/01/21 22:23:03 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "pipex.h"

static int	count_args(const char *cmd)
{
	int		count;
	char	quote;

	count = 0;
	quote = '\0';
	while (*cmd)
	{
		while (is_space(*cmd))
			cmd++;
		if (*cmd == '\0')
			break ;
		count++;
		while (*cmd && (quote != '\0' || !is_space(*cmd)))
		{
			if ((*cmd == '\'' || *cmd == '"') && (!quote || *cmd == quote))
			{
				if (quote)
					quote = '\0';
				else
					quote = *cmd;
			}
			cmd++;
		}
	}
	return (count);
}

static char	*extract_arg(const char **cmd_ptr)
{
	const char	*cmd;
	char		quote_char;
	int			in_quotes;
	size_t		len;
	const char	*start;
	char		*arg;

	cmd = *cmd_ptr;
	quote_char = '\0';
	in_quotes = 0;
	len = 0;
	while (is_space(*cmd))
		cmd++;
	start = cmd;
	while (*cmd && (in_quotes || !is_space(*cmd)))
	{
		if ((*cmd == '\'' || *cmd == '"') && (in_quotes == 0 || *cmd == quote_char))
		{
			if (in_quotes)
				in_quotes = 0;
			else
			{
				in_quotes = 1;
				quote_char = *cmd;
			}
		}
		else
			len++;
		cmd++;
	}
	arg = malloc(len + 1);
	if (!arg)
		return (NULL);
	len = 0;
	while (start < cmd)
	{
		if (*start == '\'' || *start == '"')
		{
			if (in_quotes && *start == quote_char)
				in_quotes = 0;
			else if (!in_quotes)
			{
				in_quotes = 1;
				quote_char = *start;
			}
		}
		else
			arg[len++] = *start;
		start++;
	}
	arg[len] = '\0';
	*cmd_ptr = cmd;
	return (arg);
}

// Function to split a command string into arguments, handling quotes
// < in.txt cat | grep -o -E 'is | the' > out.txt
// ./pipex in.txt "cat" "grep -o -E 'is | the'" out.txt
char	**split_command(const char *cmd)
{
	int		arg_count;
	char	**args;
	int		i;

	if (!cmd)
		return (NULL);
	arg_count = count_args(cmd);
	args = (char **)malloc((arg_count + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	while (i < arg_count)
	{
		args[i] = extract_arg(&cmd);
		if (!args[i])
		{
			while (i-- > 0)
				free(args[i]);
			free(args);
			return (NULL);
		}
		i++;
	}
	args[arg_count] = NULL;
	return (args);
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
	int		i;
	char	*path;
	char	**cmd_argv;

	cmd_argv = split_command(cmd);
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
