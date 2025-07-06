#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <stdio.h>
# include "../libft/libft.h"

void	free_split(char **arr);
char	*find_command_path(char *cmd);
void	execute_command(char *cmd, char **envp);
void	cmd1_process(int pipefd[2], int input_fd, char *cmd, char **envp);
void	cmd2_process(int pipefd[2], int output_fd, char *cmd, char **envp);
void	pipe_process(int p_fd[2], int fd1, int fd2, char **argv, char **envp);
int		main(int argc, char **argv, char **envp);

#endif
