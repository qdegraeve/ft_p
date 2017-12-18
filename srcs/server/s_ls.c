#include "server.h"

static int	invalid_options(const char **cmd)
{
	int			i;
	int			j;

	i = 1;
	j = 0;
	while (cmd[i])
	{
		j = 0;
		if (cmd[i][j] == '-')
		{
			while (cmd[i][j] && cmd[i][++j] != 'R')
				;
			if (cmd[i][j])
				return (1);
		}
		else
			return (1);
		i++;
	}
	return (0);
}

int			exec_ls(const char **cmd, int csock)
{
	pid_t		pid;
	int			status;

	if ((pid = fork()) == -1)
		return (send_error(csock, "fork error before executing ls", 0));
	if (pid > 0)
	{
		wait4(pid, &status, 0, NULL);
		if (WEXITSTATUS(status) == 0)
			send_success(csock, "ls exited with no error");
		else
			send_error(csock, "ls exited with errors", 0);
	}
	else
	{
		dup2(csock, 1);
		dup2(csock, 2);
		if (has_operator(cmd) || invalid_options(cmd))
		{
			send(csock, ERROR_MSG, ERROR_LENGTH, 0);
			exit(EXIT_FAILURE);
		}
		execv("/bin/ls", (char *const*)cmd);
	}
	return (0);
}
