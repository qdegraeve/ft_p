#include "client.h"

static void	usage(char *str)
{
	ft_printf("Usage: %s <port>\n", str);
	exit(EXIT_FAILURE);
}

int			hostname_to_ip(char *hostname, char *ip)
{
	struct addrinfo	hints;
	struct addrinfo	*servinfo;
	int				rv;

	ft_memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if ((rv = getaddrinfo(hostname, "http", &hints, &servinfo)) != 0)
	{
		ft_printf("getaddrinfo: error\n");
		return (1);
	}
	ft_strcpy(ip,
		inet_ntoa(((struct sockaddr_in *)servinfo->ai_addr)->sin_addr));
	freeaddrinfo(servinfo);
	return (0);
}

int			create_client2(char *ip, int sock, int port)
{
	struct sockaddr_in	sin;
	char				buf[256];

	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(ip);
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		ft_printf("Connect error on socket : %d\n", sock);
		exit(EXIT_FAILURE);
	}
	if (recv(sock, &buf, 255, 0) > 3)
	{
		ft_putendl(buf);
		exit(EXIT_FAILURE);		
	}
	return (sock);
}

int			create_client(char *addr, int port)
{
	int					sock;
	char				ip[100];
	struct protoent		*proto;
	struct timeval		time;

	proto = getprotobyname("tcp");
	if (!proto || hostname_to_ip(addr, ip) > 0)
		return (-1);
	if ((sock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) == -1)
	{
		ft_printf("Failed to create socket");
		exit(EXIT_FAILURE);
	}
	time.tv_sec = 1;
	time.tv_usec = 0;
	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &time, sizeof(time)) == -1)
	{
		ft_printf("Failed to set timeout for socket: %d\n", sock);
		exit(EXIT_FAILURE);
	}
	return (create_client2(ip, sock, port));
}

int			main(int ac, char **av)
{
	int					port;
	int					sock;

	if (ac != 3)
		usage(av[0]);
	port = ft_atoi(av[2]);
	if ((sock = get_current_socket(create_client(av[1], port))) == -1)
		return (1);
	signal_catcher();
	user_interface(sock);
	close(sock);
	return (0);
}
