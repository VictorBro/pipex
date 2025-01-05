/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 15:45:15 by vbronov           #+#    #+#             */
/*   Updated: 2024/12/08 03:10:39 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

t_lst_node	*create_node(char **buf)
{
	t_lst_node	*node;

	node = (t_lst_node *)malloc(sizeof(t_lst_node));
	if (!node)
	{
		free(*buf);
		*buf = NULL;
		return (NULL);
	}
	node->buf = *buf;
	node->next = NULL;
	return (node);
}

ssize_t	find_new_line(t_lst *list)
{
	int	i;

	if (!list || !list->tail)
		return (-1);
	i = 0;
	while (list->tail->buf[i] != '\0')
	{
		if (list->tail->buf[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

static void	free_helper(t_lst **list)
{
	if ((*list)->head)
	{
		free((*list)->head->buf);
		(*list)->head->buf = NULL;
		free((*list)->head);
		(*list)->head = NULL;
	}
	(*list)->tail = NULL;
	(*list)->len = 0;
	free(*list);
	*list = NULL;
}

char	*free_list(t_lst **list, int free_all)
{
	t_lst_node	*temp;

	if (!*list)
		return (NULL);
	while ((*list)->head != (*list)->tail)
	{
		temp = (*list)->head;
		(*list)->head = (*list)->head->next;
		if (temp->buf)
			free(temp->buf);
		temp->buf = NULL;
		temp->next = NULL;
		free(temp);
	}
	if (free_all)
		free_helper(list);
	return (NULL);
}

t_lst	*init_list(t_lst **list)
{
	if (!*list)
	{
		*list = (t_lst *)malloc(sizeof(t_lst));
		if (!*list)
			return (NULL);
		(*list)->head = NULL;
		(*list)->tail = NULL;
		(*list)->len = 0;
	}
	return (*list);
}
