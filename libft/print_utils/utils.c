/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 17:59:11 by vbronov           #+#    #+#             */
/*   Updated: 2025/01/21 21:29:41 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

void	init_opt(t_opt *opt, int fd)
{
	opt->sharp = 0;
	opt->space = 0;
	opt->plus = 0;
	opt->minus = 0;
	opt->min_width = 0;
	opt->dot = 0;
	opt->dot_width = 0;
	opt->zero = 0;
	opt->zero_width = 0;
	opt->padding = 0;
	opt->error = 0;
	opt->fd = fd;
}

int	count_digits(unsigned int n, int base)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	while (n > 0)
	{
		n /= base;
		count++;
	}
	return (count);
}

int	print_padding(int count, char pad_char, t_opt *opt)
{
	int	total;

	if (opt->error || count <= 0)
		return (0);
	total = 0;
	while (count-- > 0)
		total += print_char(pad_char, opt);
	return (total);
}

int	print_digits(unsigned int n, char *base, unsigned int base_len, t_opt *opt)
{
	int	total;

	if (opt->error)
		return (0);
	total = 0;
	if (n < base_len)
	{
		total += print_char(base[n], opt);
		return (total);
	}
	total += print_digits(n / base_len, base, base_len, opt);
	if (!opt->error)
		total += print_char(base[n % base_len], opt);
	return (total);
}

int	ft_max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}
