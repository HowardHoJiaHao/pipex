/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-jia- <hho-jia-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 15:18:25 by hho-jia-          #+#    #+#             */
/*   Updated: 2025/08/29 15:22:28 by hho-jia-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	exec_cmd(char *cmd_str, char **envp)
{
	char	**cmd_args;
	char	*cmd_path;

	if (is_empty_str(cmd_str))
		error_and_exit("ft_split");
	cmd_args = ft_split(cmd_str, ' ');
	if (!cmd_args)
	{
		handle_error("ft_split", cmd_args, NULL);
		exit(EXIT_FAILURE);
	}
	cmd_path = find_cmd_path(cmd_args[0], envp);
	if (!cmd_path)
	{
		handle_error("command not found", cmd_args, cmd_path);
		exit(127);
	}
	execve(cmd_path, cmd_args, envp);
	handle_error("execve failed", cmd_args, cmd_path);
	exit(EXIT_FAILURE);
}

void	child_one(char **argv, char **envp, int *pipefd)
{
	int	infile_fd;

	infile_fd = open(argv[1], O_RDONLY);
	if (infile_fd < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		error_and_exit ("open infile");
	}
	if (dup2(infile_fd, STDIN_FILENO) < 0)
		perror(" dup2 file");
	if (dup2(pipefd[1], STDOUT_FILENO) < 0)
		perror(" dup2 pipe write");
	close(infile_fd);
	close(pipefd[0]);
	close(pipefd[1]);
	exec_cmd(argv[2], envp);
}

void	child_two(char **argv, char **envp, int *pipefd)
{
	int	outfile_fd;

	outfile_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile_fd < 0)
		error_and_exit("open outfile_fd");
	if (dup2(pipefd[0], STDIN_FILENO) < 0)
		error_and_exit("dup2 file");
	if (dup2(outfile_fd, STDOUT_FILENO) < 0)
		error_and_exit("dup2 pipe write");
	close(outfile_fd);
	close(pipefd[0]);
	exec_cmd(argv[3], envp);
}

void	run_pipex(char **argv, char **envp)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	if (pipe(pipefd) < 0)
		error_and_exit("pipe");
	pid1 = fork();
	if (pid1 < 0)
		error_and_exit("fork");
	if (pid1 == 0)
		child_one (argv, envp, pipefd);
	close(pipefd[1]);
	pid2 = fork();
	if (pid2 < 0)
		error_and_exit("fork");
	if (pid2 == 0)
		child_two (argv, envp, pipefd);
	close(pipefd[0]);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		exit(WEXITSTATUS(status));
	else
		exit(EXIT_FAILURE);
}

int	main(int argc, char **argv, char **env)
{
	if (argc != 5)
		print_error();
	run_pipex(argv, env);
	return (0);
}
