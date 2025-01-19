/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 10:45:29 by vbronov           #+#    #+#             */
/*   Updated: 2025/01/11 18:49:50 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Locates the first occurrence of the null-terminated string needle in the
 * null-terminated string haystack, where not more than len characters are
 * searched. Characters that appear after a `\0' character are not searched.
 *
 * @param haystack The string to be searched.
 * @param needle The string to search for.
 * @param len The maximum number of characters to search.
 * @return A pointer to the first occurrence of the needle in the haystack,
 * or a null pointer if needle is not part of haystack.
 */
char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	if (!*needle)
		return ((char *)haystack);
	i = 0;
	while (haystack[i] && i < len)
	{
		j = 0;
		while (haystack[i + j] == needle[j] && i + j < len)
		{
			if (!needle[j + 1])
				return ((char *)&haystack[i]);
			j++;
		}
		i++;
	}
	return (NULL);
}
