#include "server.h"

static void	usage(char *str)
{
	ft_printf("Usage: %s <port>\n", str);
	exit(EXIT_FAILURE);
}

static int	create_server(int port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (!proto)
		return (-1);
	if ((sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) == -1)
	{
		ft_printf("Failed to create socket\n", port);
		exit(EXIT_FAILURE);
	}
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = INADDR_ANY;
	if (bind(sock, (const struct sockaddr*)&sin, sizeof(sin)) == -1)
	{
		ft_printf("Failed to bind socket on port :%d\n", port);
		exit(EXIT_FAILURE);
	}
	listen(sock, 42);
	return (sock);
}

static int	fork_for_connection(int cs)
{
	pid_t				pid;

	if ((pid = fork()) == -1)
		return (1);
	if (pid > 0)
	{
		ft_printf("connection accepted on sock: %d\n", cs);
		nb_connections(1);
		send(cs, "OK\0", 2, 0);
		signal(SIGCHLD, handle_sigchld);
		close(cs);
	}
	else
	{
		handle_cmds(cs);
		ft_putendl("closing connection");
		close(cs);
		exit(EXIT_SUCCESS);
	}
	return (0);
}

static int	handle_connections(int port)
{
	int					sock;
	int					cs;
	unsigned int		cslen;
	struct sockaddr_in	csin;

	cs = 0;
	ft_bzero(&csin, sizeof(struct sockaddr_in));
	sock = create_server(port);
	while (42)
	{
		if ((cs = accept(sock, (struct sockaddr *)&csin, &cslen)) == -1)
			continue ;
		ft_printf("number of active connections: %d\n", nb_connections(0));
		if (nb_connections(0) < 42)
			fork_for_connection(cs);
		else
		{
			ft_putendl("Too many conections. Closing connecion");
			send(cs, "Too many connections. Try again later\0", 37, 0);
			close(cs);
		}
	}
	close(sock);
	return (0);
}

int			main(int ac, char **av)
{
	int					port;

	if (ac != 2)
		usage(av[0]);
	get_base_path();
	port = ft_atoi(av[1]);
	ft_printf("%sstart of function server%s\n", RED, NRM);
	handle_connections(port);
	ft_printf("%send of function server%s\n", RED, NRM);
	return (0);
}
