/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbronov <vbronov@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 02:07:05 by vbronov           #+#    #+#             */
/*   Updated: 2025/01/25 15:18:27 by vbronov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"

# define FALSE		0
# define TRUE		1
# define PIPE_OUT	0
# define PIPE_IN	1
# define COMMAND_NOT_FOUND	127
# define PERMISSION_DENIED	126

typedef struct s_builder
{
	char	*str;
	size_t	len;
	size_t	cap;
}				t_builder;

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
	int		err;
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
int		pipex(t_pipex *pa);
void	process_fd_and_pipes(t_pipex *pa);
void	ft_heredoc_child(t_pipex *pa);
int		is_space(char c);
int		builder_append(t_builder *b, char c);
char	*builder_finalize(t_builder *b);
int		handle_special_char(const char *s, int *i, t_builder *b);
int		ft_wait_pid(int last_pid);
void	free_lst_nodes(t_list *tokens);
char	**list_to_array(t_list *tokens);

#endif