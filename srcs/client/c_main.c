#include "ftp.h"

static const char	*g_commands[CMDS_NB] = {
	"cd" ,
	"pwd" ,
	"ls" 
};

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

int		exec_cmds(int sock, char *cmd)
{
	int		i;
	int 	j;
	char	buf[42000];

	i = 0;
	j = -1;
	while (cmd[i] && cmd[i] != ' ')
		i++;
	while (i && ++j < CMDS_NB)
	{
		if (ft_strncmp(g_commands[j], cmd, i) == 0)
		{
			send(sock, cmd, ft_strlen(cmd), 0);
			i = recv(sock, buf, 41999, 0);
			buf[i] = '\0';
			write(1, buf, i);
			write(1, "\n", 1);
			return (1);
		}
	}
	return (0);
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

void	prompt()
{
	char	*path;

	path = NULL;
	path = getcwd(path, 255);
	// if (g_returned_error > 0)
	ft_printf("%s %s: > ", get_pwd_prompt(path), GRN);
	ft_printf("%s", NRM);
	// else
		// ft_printf("%s \033[1;32m: > ", get_pwd_prompt(path));
	if (path)
		ft_strdel(&path);
}

void	user_interface(int sock)
{
	char	*line;
	char	path[256];

	line = NULL;
	while (1)
	{
		prompt();
		if (get_next_line(0, &line) > 0)
		{
			if (ft_strcmp(line, "quit") == 0)
			{
				ft_strdel(&line);
				break;
			}
			else if (server_cmds(sock, line) == 0)
			{
				realpath(line, path);
				ft_printf("real path == %s\n", path);
			}
			ft_strdel(&line);
		}
	}
}

int		main(int ac, char **av) {
	int					port;
	int					sock;
	// int					r;
	// char				buf[1024];

	if (ac != 3)
		usage(av[0]);
	port = ft_atoi(av[2]);
	sock = create_client(av[1], port);
	// while (r > 0) {
	// 	buf[r] = '\0';
	// 	printf("send == %zd\n", send(sock, buf, r, 0));
	// 	if (ft_strcmp(buf, "exit\n") == 0)
	// 		break;
	// 	r = read(0, &buf, 1023);
	// }
	user_interface(sock);
	close(sock);
	return (0);
}