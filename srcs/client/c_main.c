#include "client.h"

void	usage(char *str) {
	ft_printf("Usage: %s <port>\n", str);
	exit(EXIT_FAILURE);
}

int hostname_to_ip(char *hostname , char *ip)
{
    struct addrinfo hints;
    struct addrinfo *servinfo;
    int rv;
 
    ft_memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if ( (rv = getaddrinfo( hostname , "http" , &hints , &servinfo)) != 0) 
    {
        ft_printf("getaddrinfo: error\n");
        return (1);
    }
    ft_strcpy(ip , inet_ntoa( ((struct sockaddr_in *)servinfo->ai_addr)->sin_addr ) );

     
    freeaddrinfo(servinfo);
    return 0;
}

int		create_client(char *addr, int port) {
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;
	struct timeval		time;
	char 				ip[100];

	proto = getprotobyname("tcp");
	if (!proto || hostname_to_ip(addr , ip) > 0)
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
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port); /* convert port to network endianess */
	sin.sin_addr.s_addr = inet_addr(ip); /* set the server address as requested by user */
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1) {
		ft_printf("Connect error on socket : %d\n", sock);
		exit(EXIT_FAILURE);
	}
	return (sock);
}

char	*get_server_pwd()
{
	char	cmd[6];
	char	buf[256];
	int		sock;
	int		i;

	sock = get_current_socket(0);
	ft_strcpy(cmd, "where");
	send(sock, cmd, ft_strlen(cmd), 0);
	i = recv(sock, buf, 41999, 0);
	buf[i] = '\0';
	return (ft_strdup(buf));
}

char	*get_pwd_prompt(char *path)
{
	int		i;

	if (!path)
		path = ft_strdup("Middle of nowhere");
	i = ft_strlen(path);
	while (i >= 0 && path[i] != '/')
		i--;
	return (path + i + 1);
}

void	prompt(int error)
{
	char	*path;
	char	*server_pwd;

	path = NULL;
	path = getcwd(path, 255);
	server_pwd = get_server_pwd();
	if (error)
		ft_printf("%sl-> %s %s(s-> %s)%s: X ", CYN, get_pwd_prompt(path), YEL, server_pwd, RED);
	else
	{
		ft_printf("%sl-> %s %s(s-> %s)%s: <3 ", CYN, get_pwd_prompt(path), YEL, server_pwd, GRN);
	}
	ft_printf("%s", NRM);
	ft_strdel(&path);
	ft_strdel(&server_pwd);
}

void	user_interface(int sock)
{
	char	*line;
	int		error;

	error = 0;
	line = NULL;
	while (1)
	{
		prompt(error);
		if (get_next_line(0, &line) > 0)
		{
			if (ft_strlen(line) == 0)
				continue ;
			if (ft_strcmp(line, "quit") == 0)
			{
				ft_strdel(&line);
				break;
			}
			else if ((error = exec_cmds(sock, line)) ==  -1)
				ft_printf("%sCommand not found\nNew commands coming soon\n%s", RED, NRM);
			ft_strdel(&line);
		}
	}
}

int		get_current_socket(int current)
{
	static int 	socket = -1;

	if (socket == -1)
		socket = current;
	return (socket);
}

int		main(int ac, char **av) {
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