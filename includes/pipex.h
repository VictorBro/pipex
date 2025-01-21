/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 02:07:05 by vbronov           #+#    #+#             */
/*   Updated: 2025/01/21 21:08:12 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"

# define FALSE		0
# define TRUE		1
# define PIPE_OUT	0
# define PIPE_IN	1

enum {
	OK,
	USAGE_ERROR,
	INIT_ERROR,
};

typedef struct s_pipex
{
	int		in_fd;
	int		out_fd;
	char	**env_paths;
	int		(*fd_pipe)[2];
	int		cmd_count;
	int		argc;
	char	**argv;
	char	**envp;
	int		here_doc;
}				t_pipex;

void	ft_free_strs(char **strs);
int		ft_parse_env(t_pipex *pa);
int		ft_open_files(t_pipex *pa);
char	*ft_form_path(char *path, char *cmd);
char	**ft_parse_cmd(t_pipex *pa, char *cmd);
void	ft_free_pipes(t_pipex *pa);
int		ft_open_pipes(t_pipex *pa);
void	ft_first_cmd(t_pipex *pa);
void	ft_middle_cmd(t_pipex *pa, int i);
void	ft_last_cmd(t_pipex *pa);
void	ft_clean_pipex(t_pipex *pa);
int		ft_init_pipex(t_pipex *pa, int argc, char *argv[], char *envp[]);
void	pipex(t_pipex *pa);
void	process_fd_and_pipes(t_pipex *pa);
void	ft_heredoc_child(t_pipex *pa);
int		is_space(char c);

#endif