/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfassi-f <mfassi-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/12/21 19:31:48 by mfassi-f          #+#    #+#             */
/*   Updated: 2013/12/29 19:38:27 by mfassi-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <main.h>
#include <stdio.h>
#include <signal.h>

void	print_prompt(char **envp)
{
	ft_putstr("\x1B[37m(\x1B[0m");
	ft_putstr("\x1B[36m");
	ft_putstr(*find(envp, "PWD=") + 4);
	ft_putstr("\x1B[0m");
	ft_putstr("\x1B[37m)\x1B[0m");
	ft_putstr("\x1B[31m$>\x1B[0m");
}


char	*get_path(char **envp, char **cmd)
{
	char	**paths;
	char	*part_path;
	char	*path;

	paths = ft_strsplit(envp[0] + 5, ':');
	part_path = ft_strjoin(search_path(paths, cmd[0]), "/");
	path = ft_strjoin(part_path, cmd[0]);
	ft_strdel(&part_path);
	free_arr(&paths);
	return (path);
}

char	**get_cmd(void)
{
	char	*line;
	char	**cmd;

	line = get_line();
	if (ft_strlen(line) == 0)
	{
		ft_strdel(&line);
		return (NULL);
	}
	cmd = ft_strsplit(line, ' ');
	ft_strdel(&line);
	return (cmd);
}


int	implemented_function(char **cmd, char ***envp)
{
	if (!cmd)
		return (1);
	if (ft_strcmp(cmd[0], "exit") == 0)
	{
		free_arr(&cmd);
		return (-1);
	}
	else if (ft_strcmp(cmd[0], "env") == 0)
		ft_env(*envp, cmd);
	else if (ft_strcmp(cmd[0], "cd") == 0)
		ft_cd(cmd, *envp);
	else if (ft_strcmp(cmd[0], "setenv") == 0)
		*envp = ft_setenv(cmd, *envp);
	else if (ft_strcmp(cmd[0], "unsetenv") == 0)
		*envp = ft_unsetenv(cmd, *envp);
	else
		return (0);
	return (1);
}

int	main(int argc, char ** argv, char **envp)
{
	char	**cmd;
	pid_t	pid;
	int		ret;

	while (argc == 1 && argv)
	{
		print_prompt(envp);
		cmd = get_cmd();
		ret = implemented_function(cmd, &envp);
		if (ret == -1)
			exit(0);
		else if (ret == 1)
			continue ;
		pid = fork();
		if (pid == 0 && execve(get_path(envp, cmd), cmd, envp) == -1)
		{
			ft_putstr("42sh: command not found: ");
			ft_putendl(cmd[0]);
			exit(0);
		}
		if (pid < 0)
			ft_putstr("fork failed\n");
		wait(0);
	}
	return (0);
}
