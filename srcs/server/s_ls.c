#include "server.h"

int		exec_ls(const char **cmd, int csock)
{
	pid_t		pid;
	int			status;

	if ((pid = fork()) == -1)
		return (send_error(csock, "fork error before executing ls", 0));
	if (pid > 0)
	{
		printf("entered in fork parent for execution\n");
		wait4(pid, &status, 0, NULL);
		if (WEXITSTATUS(status) == 0)
			send_success(csock, "ls exited with no error");
		else
			send_error(csock, "ls exited with errors", 0);
		printf("exited parent -- Child exited with status [%d] -- [%d]\n", WEXITSTATUS(status), status);
	}
	else
	{
		dup2(csock, 1);
		dup2(csock, 2);
		if (has_operator(cmd))
		{
			send(csock, "Command format invalid: operators [&|;] not accepted\n", 100, 0);
			exit(EXIT_FAILURE);
		}
		execv("/bin/ls", (char* const*)cmd);
	}		
	return (0);
}
