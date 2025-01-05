/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 02:10:32 by vbronov           #+#    #+#             */
/*   Updated: 2025/01/05 02:10:34 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "pipex.h"

int	main(int argc, char *argv[], char *envp[])
{
	t_pipex	pa;

	if (argc < 5)
	{
		ft_fprintf(STDERR_FILENO,
			"usage: %s (<infile> | here_doc <LIMITER>)"
			" <cmd1> ... <cmdn> <outfile>\n", argv[0]);
		return (USAGE_ERROR);
	}
	if (!ft_init_pipex(&pa, argc, argv, envp))
		return (INIT_ERROR);
	pipex(&pa);
	return (OK);
}
