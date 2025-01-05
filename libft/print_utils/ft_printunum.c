/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printunum.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 22:03:25 by vbronov           #+#    #+#             */
/*   Updated: 2025/01/05 00:49:02 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static int	handle_num(unsigned int n, t_opt *opt, int num_digits, int padding)
{
	int	total;

	total = 0;
	if (!opt->minus && (!opt->zero || opt->dot))
		total += print_padding(padding, ' ', opt);
	if (opt->zero && !opt->dot && !opt->minus)
		total += print_padding(padding, '0', opt);
	if (opt->dot)
		total += print_padding(opt->dot_width - num_digits, '0', opt);
	total += print_digits(n, "0123456789", 10, opt);
	if (opt->minus)
		total += print_padding(opt->min_width - total, ' ', opt);
	return (total);
}

int	ft_printunum(unsigned int n, t_opt *opt)
{
	int	len;
	int	padding;
	int	num_digits;

	opt->min_width = ft_max(opt->min_width, opt->dot_width);
	opt->min_width = ft_max(opt->min_width, opt->zero_width);
	opt->min_width = ft_max(opt->min_width, opt->padding);
	if (n == 0 && opt->dot && opt->dot_width == 0)
		return (print_padding(opt->min_width, ' ', opt));
	num_digits = count_digits(n, 10);
	len = ft_max(opt->dot_width, num_digits);
	padding = opt->min_width - len;
	if (opt->minus)
		opt->zero = 0;
	return (handle_num(n, opt, num_digits, padding));
}

int	process_normal(const char *str, va_list *params, t_opt *opt)
{
	int	total;

	total = 0;
	if (str[1] == 'c')
		total += ft_printchar(va_arg(*params, int), opt);
	else if (str[1] == 's')
		total += ft_printstr(va_arg(*params, char *), opt);
	else if (str[1] == 'p')
		total += ft_printptr(va_arg(*params, void *), opt);
	else if (str[1] == 'd' || str[1] == 'i')
		total += ft_printnum(va_arg(*params, int), opt);
	else if (str[1] == 'u')
		total += ft_printunum(va_arg(*params, unsigned int), opt);
	else if (str[1] == 'x')
		total += ft_printhex(va_arg(*params, unsigned int), opt, 0);
	else if (str[1] == 'X')
		total += ft_printhex(va_arg(*params, unsigned int), opt, 1);
	else if (str[1] == '%')
		total += print_char('%', opt);
	return (total);
}
