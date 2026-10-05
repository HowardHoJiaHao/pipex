/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-jia- <hho-jia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 15:18:12 by hho-jia-          #+#    #+#             */
/*   Updated: 2025/08/29 13:51:41 by hho-jia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	handle_error(char *err_str, char **free_arr_str, char *free_str)
{
	if (err_str)
		perror(err_str);
	if (free_arr_str)
		free_array(free_arr_str);
	if (free_str)
		free(free_str);
}

char	*find_path(char **envp)
{
	int		i;
	char	*path_env;

	i = 0;
	path_env = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_env = envp[i] + 5;
			return (path_env);
		}
		i++;
	}
	return (NULL);
}

char	*handle_failure_and_return_null(char **paths, char *full_path)
{
	free_array(paths);
	if (full_path)
		return (full_path);
	return (NULL);
}

char	*find_full_path(char **paths, char *cmd)
{
	int		i;
	char	*temp_path;
	char	*full_path;

	i = 0;
	temp_path = NULL;
	full_path = NULL;
	while (paths[i] != NULL)
	{
		temp_path = ft_strjoin(paths[i], "/");
		if (!temp_path)
			return (handle_failure_and_return_null(paths, NULL));
		full_path = ft_strjoin(temp_path, cmd);
		free(temp_path);
		if (!full_path)
			return (handle_failure_and_return_null(paths, NULL));
		if (access(full_path, F_OK | X_OK) == 0)
			return (handle_failure_and_return_null(paths, full_path));
		free(full_path);
		i++;
	}
	return (NULL);
}

void	error_and_exit(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}
