#include "server.h"

int		exec_pwd(const char **cmd, int csock)
{
	pid_t		pid;
	int			status;

	if ((pid = fork()) == -1)
		return (send_error(csock, "fork error before executing pwd", 0));
	if (pid > 0)
	{
		wait4(pid, &status, 0, NULL);
		if (WEXITSTATUS(status) == 0)
			send_success(csock, "pwd : success");
		else
			send_error(csock, "pwd : error", 0);
	}
	else
	{
		dup2(csock, 1);
		dup2(csock, 2);
		if (has_operator(cmd))
		{
			send(csock, ERROR_MSG, ERROR_LENGTH, 0);
			exit(EXIT_FAILURE);
		}
		execv("/bin/pwd", (char* const*)cmd);
	}
	return (0);
}
