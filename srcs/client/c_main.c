#include "ftp.h"

int		exec_cd(char *cmd, int sock);
int		exec_fork(char *cmd, int sock);
int		exec_get(char *cmd, int sock);
int		exec_put(char *cmd, int sock);

static const t_client_cmds	g_commands[CMDS_NB] = {
	{ "cd", &exec_cd },
	{ "pwd", &exec_fork },
	{ "ls", &exec_fork },
	{ "get", &exec_get },
	{ "put", &exec_put }
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

int		exec_cd(char *cmd, int sock)
{
	send(sock, cmd, ft_strlen(cmd), 0);
	return (0);
}

int		exec_fork(char *cmd, int sock)
{
	int			i;
	char		buf[42000];

	send(sock, cmd, ft_strlen(cmd), 0);
	i = recv(sock, buf, 41999, 0);
	buf[i] = '\0';
	write(1, buf, i);	
	return (0);
}

int		exec_get(char *cmd, int sock)
{
	send(sock, cmd, ft_strlen(cmd), 0);
	return (0);
}

int		exec_put(char *cmd, int sock)
{
	char			**args;
	int				file_fd;
	unsigned long	transmit_left;
	struct stat		stat;
	t_data			data;
	int				r;
	int				part_nb;

	ft_bzero(&data, DATASIZE);
	args = ft_strsplit(cmd, ' ');
	if (!args || !args[1])
	{
		ft_printf("put: No file given\n");
		return (1);
	}
	if ((file_fd = open(args[1], O_RDONLY)) == -1)
	{
		ft_printf("put: failed to open file: %s\n", args[1]);
		return (1);
	}
	if (fstat(file_fd, &stat) < 0)
	{
		ft_printf("put: failed to stat file: %s\nTransfer aborted", args[1]);
		return (1);
	}
	send(sock, cmd, ft_strlen(cmd), 0);
	data.data_size = htonl(stat.st_size);
	data.total_parts = htonl(stat.st_size / (BUFSIZE - 1) + 1);
	transmit_left = stat.st_size;
	part_nb = 1;
	while (transmit_left > 0)
	{
		data.part_nb = htonl(part_nb++);
		r = read(file_fd, &data.data, BUFSIZE - 1);
		data.data[r] = '\0';
		data.part_size = htonl(r); 
		send(sock, &data, DATASIZE, 0);
		printf("transmit_left == %lu\n", transmit_left);
		transmit_left -= r;
	}
	close(file_fd);
	return (0);
}

int		exec_cmds(int sock, char *cmd)
{
	int			i;
	int 		j;
	t_data		data;

	i = 0;
	j = -1;
	while (cmd[i] && cmd[i] != ' ')
		i++;
	while (i && ++j < CMDS_NB)
	{
		if (ft_strncmp(g_commands[j].id, cmd, i) == 0)
		{
			if (g_commands[j].f(cmd, sock) == 0)
			{
				recv(sock, &data, DATASIZE, 0);
				data.data_size = ntohl(data.data_size);
				data.return_code = ntohs(data.return_code);
				data.total_parts = ntohl(data.total_parts);
				data.part_size = ntohl(data.part_size);
				data.part_nb = ntohl(data.part_nb);
				if (data.data_size > 1)
					ft_printf("size == [%ld], parts == [%ld], part_nb == [%ld]\ndata == %s\n", data.data_size, data.total_parts, data.part_nb, data.data);
				else
					ft_printf("size == [%ld], return_code == [%ld]\ndata == %s\n", data.data_size, data.return_code, data.data);
				return (1);
			}
			return (0);
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
			else if (exec_cmds(sock, line) == 0)
			{
				realpath(line, path);
				ft_printf("real path == %s\n", path);
				ft_printf("t_data size == [%d]\n", DATASIZE);
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