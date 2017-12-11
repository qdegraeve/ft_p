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
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTRAP, SIG_IGN);
}
