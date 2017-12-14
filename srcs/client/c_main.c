#include "client.h"

void	usage(char *str) {
	ft_printf("Usage: %s <port>\n", str);
	exit(EXIT_FAILURE);
}

int		create_client(char *addr, int port) {
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (!proto)
		return (-1);
	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port); /* convert port to network endianess */
	sin.sin_addr.s_addr = inet_addr(addr); /* set the server address as requested by user */
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1) {
		ft_printf("connect error\n");
		exit(EXIT_FAILURE);
	};
	return (sock);
}

void	print_server_pwd()
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
	ft_putstr(buf);
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

	path = NULL;
	path = getcwd(path, 255);
	if (error)
		ft_printf("%s %s: > ", get_pwd_prompt(path), RED);
	else
	{
		ft_printf("%s (", get_pwd_prompt(path));
		print_server_pwd();
		ft_printf(") %s: > ", GRN);
	}
	ft_printf("%s", NRM);
	if (path)
		ft_strdel(&path);
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
	sock = get_current_socket(create_client(av[1], port));
	signal_catcher();
	user_interface(sock);
	close(sock);
	return (0);
}