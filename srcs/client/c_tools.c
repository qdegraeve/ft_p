#include "client.h"

int		send_error(int csock, char *name)
{
	t_data			data;

	ft_bzero(&data, DATASIZE);
	ft_strcpy(data.data, "Client: Transmission error : ");
	ft_strcat(data.data, name);
	data.data_size = htonl(0);
	data.return_code = htonl(1);
	send(csock, &data, DATASIZE, 0);
	return (0);
}

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
