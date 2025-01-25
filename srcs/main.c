/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 02:10:47 by vbronov           #+#    #+#             */
/*   Updated: 2025/01/25 05:59:12 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "pipex.h"

int	main(int argc, char *argv[], char *envp[])
{
	t_pipex	pa;

	if (argc != 5)
	{
		ft_fprintf(STDERR_FILENO,
			"usage: %s <infile> <cmd1> <cmd2> <outfile>\n", argv[0]);
		return (EXIT_FAILURE);
	}
	if (!ft_init_pipex(&pa, argc, argv, envp))
		return (EXIT_FAILURE);
	return (pipex(&pa));
}
