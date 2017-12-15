#include "client.h"

int		exec_lls(char *command, int sock)
{
	pid_t		pid;
	int			status;
	char		**args;

	sock = 0;
	args = ft_strsplit(command, ' ');
	if ((pid = fork()) == -1)
	{
		ft_printf("Fork error");
		return (1);
	}
	if (pid > 0)
	{
		wait4(pid, &status, 0, NULL);
		ft_tabdel(&args);
		return (WEXITSTATUS(status));
	}
	else
		execv("/bin/ls", args);
	return (0);

}