#include "client.h"

void	ctrl_c(int signal)
{
	if (signal == SIGINT)
	{
		close(get_current_socket(-1));
		ft_putendl_fd("client closed with ctr-c", 2);
		ft_putendl_fd("closing connexion with server", 2);
		exit(EXIT_FAILURE);
	}
}

void	signal_catcher(void)
{
	signal(SIGINT, ctrl_c);
}

int		get_current_socket(int current)
{
	static int	socket = -1;

	if (socket == -1)
		socket = current;
	return (socket);
}
