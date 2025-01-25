/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 01:55:05 by vbronov           #+#    #+#             */
/*   Updated: 2025/01/25 15:18:20 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "pipex.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r');
}

char	**list_to_array(t_list *tokens)
{
	char	**arr;
	t_list	*tmp;
	int		count;
	int		i;

	count = ft_lstsize(tokens);
	arr = (char **)ft_calloc(count + 1, sizeof(char *));
	if (!arr)
	{
		ft_lstclear(&tokens, free);
		return (NULL);
	}
	i = 0;
	tmp = tokens;
	while (tmp)
	{
		arr[i] = (char *)tmp->content;
		i++;
		tmp = tmp->next;
	}
	arr[i] = NULL;
	free_lst_nodes(tokens);
	return (arr);
}

static int	builder_grow(t_builder *b, size_t needed)
{
	char	*new_str;
	size_t	new_cap;

	new_cap = b->cap;
	if (new_cap < 16)
		new_cap = 16;
	while (new_cap < b->len + needed)
		new_cap *= 2;
	new_str = (char *)ft_calloc(new_cap, sizeof(char));
	if (!new_str)
		return (FALSE);
	if (b->str && b->len > 0)
		ft_memcpy(new_str, b->str, b->len);
	free(b->str);
	b->str = new_str;
	b->cap = new_cap;
	return (TRUE);
}

int	builder_append(t_builder *b, char c)
{
	if (b->len + 1 >= b->cap)
	{
		if (!builder_grow(b, 1))
			return (FALSE);
	}
	b->str[b->len] = c;
	b->len++;
	b->str[b->len] = '\0';
	return (TRUE);
}

char	*builder_finalize(t_builder *b)
{
	char	*final_str;

	if (!b->str)
		return (NULL);
	final_str = b->str;
	b->str = NULL;
	return (final_str);
}
