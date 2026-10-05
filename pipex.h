/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-jia- <hho-jia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 15:18:40 by hho-jia-          #+#    #+#             */
/*   Updated: 2025/08/29 13:51:49 by hho-jia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <fcntl.h>
# include <sys/wait.h>

char	*find_cmd_path(char *cmd, char **envp);
char	**ft_split(char const *s, char c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strrchr(const char *s, int c);
char	*ft_strdup(const char *s1);
char	*ft_strjoin(char const *s1, char const *s2);
void	free_array(char **array);
size_t	ft_strlen(const char *s);
void	print_error(void);
int		is_space(int c);
int		is_empty_str(const char *str);
void	handle_error(char *err_str, char **free_arr_str, char *free_str);
char	*find_path(char **envp);
char	*find_full_path(char **paths, char *cmd);
void	error_and_exit(const char *msg);

#endif
