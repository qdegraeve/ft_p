#include "server.h"

char	*get_base_path(void)
{
	static char		*base_path = NULL;

	if (!base_path)
		base_path = getcwd(base_path, 255);
	return (base_path);
}

void	handle_sigchld(int signal)
{
	pid_t	p;
	int		status;

	while (signal && (p = wait4(-1, &status, WNOHANG, NULL)) != -1)
		nb_connections(-1);
}

int		nb_connections(int action)
{
	static int		connections = 0;

	connections += action;
	return (connections);
}
