/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 15:40:04 by vbronov           #+#    #+#             */
/*   Updated: 2024/12/08 03:10:39 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H
# include <stddef.h>
# include <fcntl.h>
# include <sys/types.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include "../libft.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif
# ifndef OPEN_MAX
#  define OPEN_MAX 2048
# endif
# define FALSE 0
# define TRUE 1

typedef struct s_node
{
	char			*buf;
	struct s_node	*next;
}				t_lst_node;

typedef struct s_lst
{
	t_lst_node	*head;
	t_lst_node	*tail;
	ssize_t		len;
}			t_lst;

int			prepare_next_line(int fd, t_lst *list);
char		*copy_new_line(t_lst *list);
void		cleanse(t_lst **list);
int			append_to_list(t_lst *list, ssize_t bytes_read, char *buf);
t_lst_node	*create_node(char **buf);
ssize_t		find_new_line(t_lst *list);
char		*free_list(t_lst **list, int free_all);
t_lst		*init_list(t_lst **list);

#endif
