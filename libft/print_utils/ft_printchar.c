/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printchar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 17:59:50 by vbronov           #+#    #+#             */
/*   Updated: 2025/01/05 00:53:11 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

int	print_char(char c, t_opt *opt)
{
	int	n;

	if (opt->error)
		return (0);
	n = write(opt->fd, &c, 1);
	if (n != 1)
	{
		opt->error = 1;
		return (0);
	}
	return (1);
}

int	ft_printchar(char c, t_opt *opt)
{
	int	cur;

	cur = 0;
	while (!opt->error && cur + 1 < opt->min_width)
		cur += print_char(' ', opt);
	if (!opt->error)
		cur += print_char(c, opt);
	while (!opt->error && cur < opt->padding)
		cur += print_char(' ', opt);
	return (cur);
}

int	is_valid(const char *str)
{
	int	n;
	int	count;

	if (!str || !*str)
		return (1);
	n = ft_strlen(str);
	if (str[n - 1] != '%')
		return (1);
	count = 0;
	while (n > 0 && ft_strchr("0123456789# +-.%", str[n - 1]) != NULL)
	{
		if (str[n - 1] == '%')
			count++;
		n--;
	}
	if (count % 2 == 1)
		return (0);
	return (1);
}
