/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 10:06:38 by vbronov           #+#    #+#             */
/*   Updated: 2024/10/03 10:28:39 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_nul_present(const char *str, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		if (str[i] == '\0')
			return (1);
		i++;
	}
	return (0);
}

size_t	ft_strlcat(char *dst, const char *src, size_t dsize)
{
	size_t	src_len;
	size_t	dest_len;
	size_t	i;
	size_t	j;

	src_len = ft_strlen(src);
	if (dsize == 0)
		return (src_len);
	if (!is_nul_present(dst, dsize))
		return (src_len + dsize);
	dest_len = ft_strlen(dst);
	i = 0;
	j = dest_len;
	while (src[i] != '\0' && j < dsize - 1)
		dst[j++] = src[i++];
	dst[j] = '\0';
	return (src_len + dest_len);
}
