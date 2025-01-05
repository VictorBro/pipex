/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printptr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 22:02:37 by vbronov           #+#    #+#             */
/*   Updated: 2024/10/19 18:17:59 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static int	print_hex(unsigned long addr, char *base, t_opt *opt)
{
	int	total;

	if (opt->error)
		return (0);
	total = 0;
	if (addr < 16)
	{
		total += print_char(base[addr], opt);
		return (total);
	}
	total += print_hex(addr / 16, base, opt);
	if (!opt->error)
		total += print_char(base[addr % 16], opt);
	return (total);
}

static int	get_hex_size(unsigned long addr)
{
	int	total;

	total = 0;
	if (addr == 0)
		return (1);
	while (addr > 0)
	{
		addr /= 16;
		total++;
	}
	return (total);
}

int	ft_printptr(void *ptr, t_opt *opt)
{
	unsigned long	addr;
	int				total;
	int				len;

	addr = (unsigned long)ptr;
	total = 0;
	if (addr == 0)
		len = ft_strlen(PRINTF_NULL_PTR);
	else
		len = get_hex_size(addr) + 2;
	while (!opt->error && len + total < opt->min_width)
		total += print_char(' ', opt);
	if (addr == 0)
		total += print_str(PRINTF_NULL_PTR, opt);
	else
	{
		total += print_str("0x", opt);
		total += print_hex(addr, "0123456789abcdef", opt);
	}
	while (!opt->error && total < opt->padding)
		total += print_char(' ', opt);
	return (total);
}
