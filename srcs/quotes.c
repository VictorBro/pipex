/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 02:03:09 by vbronov           #+#    #+#             */
/*   Updated: 2025/01/25 02:04:29 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	handle_backslash(const char *s, int *i, t_builder *b, int in_dquote)
{
	char	next;

	next = s[(*i) + 1];
	if (in_dquote && (next == '\\' || next == '\"' || next == '$'
			|| next == '`' || next == '\n'))
	{
		(*i)++;
		if (!builder_append(b, s[*i]))
			return (FALSE);
		(*i)++;
		return (TRUE);
	}
	if (!builder_append(b, '\\'))
		return (FALSE);
	(*i)++;
	return (TRUE);
}

static int	handle_single_quote(const char *s, int *i, t_builder *b)
{
	(*i)++;
	while (s[*i] && s[*i] != '\'')
	{
		if (!builder_append(b, s[*i]))
			return (FALSE);
		(*i)++;
	}
	if (s[*i] == '\'')
		(*i)++;
	return (TRUE);
}

static int	handle_double_quote(const char *s, int *i, t_builder *b)
{
	(*i)++;
	while (s[*i] && s[*i] != '\"')
	{
		if (s[*i] == '\\')
		{
			if (!handle_backslash(s, i, b, 1))
				return (FALSE);
			continue ;
		}
		if (!builder_append(b, s[*i]))
			return (FALSE);
		(*i)++;
	}
	if (s[*i] == '\"')
		(*i)++;
	return (TRUE);
}

int	handle_special_char(const char *s, int *i, t_builder *b)
{
	if (is_space(s[*i]))
		return (2);
	if (s[*i] == '\'')
	{
		if (!handle_single_quote(s, i, b))
			return (-1);
		return (1);
	}
	if (s[*i] == '\"')
	{
		if (!handle_double_quote(s, i, b))
			return (-1);
		return (1);
	}
	if (s[*i] == '\\')
	{
		if (!handle_backslash(s, i, b, 0))
			return (-1);
		return (1);
	}
	return (0);
}
