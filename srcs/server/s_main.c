#include "server.h"

void	usage(char *str) {
	ft_printf("Usage: %s <port>\n", str);
	exit(EXIT_FAILURE);
}

int		create_server(int port) {
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (!proto)
		return (-1);
	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port); /* convert port to network endianess */
	sin.sin_addr.s_addr = INADDR_ANY; /* bind on all available address */
	bind(sock, (const struct sockaddr*)&sin, sizeof(sin));
	listen(sock, 42); /* can receive 42 simultaneous connections */
	return (sock);
}

int		handle_connections(int port)
{
	int					sock;
	int					cs;
	unsigned int		cslen;
	struct sockaddr_in	csin;
	pid_t				pid;
	
	cs = 0;
	sock = create_server(port);
	while (42) {
		cs = accept(sock, (struct sockaddr *)&csin, &cslen);
		if ((pid = fork()) == -1)
			return (1);
		if (pid > 0) {
			ft_printf("connection accepted on sock: %d\n", cs);
			signal(SIGCHLD, SIG_IGN);
			close(cs);
		}
		else {
			handle_cmds(cs);
			ft_putendl("closing connection");
			close(cs);
			break ;
		}
	}
	close(sock);
	return (0);
}

char	*get_base_path()
{
	static char		*base_path = NULL;

	if (!base_path)
		base_path = getcwd(base_path, 255);
	return (base_path);
}

int		main(int ac, char **av) {
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
