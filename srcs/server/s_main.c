#include "server.h"

static const t_server_cmds g_commands[CMDS_NB] = {
	{ "cd", &exec_cd },
	{ "pwd", &exec_pwd },
	{ "ls", &exec_ls },
	{ "put", &exec_put },
	{ "get", &exec_get }
};

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

void	flush_socket(int socket)
{
	t_data		data;
	while (1)
	{	
		recv(socket, &data, DATASIZE, 0);
		data.data_size = ntohl(data.data_size);
		data.total_parts = ntohl(data.total_parts);
		data.part_nb = ntohl(data.part_nb);
		data.part_size = ntohl(data.part_size);
		if (data.part_nb == data.total_parts)
			break;
	}
}

int		send_success(int csock, char *name)
{
	t_data			data;

	ft_strcpy(data.data, "CMD: Successfully ");
	ft_strcat(data.data, "received file ");
	ft_strcat(data.data, name);
	data.data_size = htonl(0);
	data.return_code = htonl(0);
	send(csock, &data, DATASIZE, 0);
	return (0);
}

int		send_error(int csock, char *name, int flush)
{
	t_data			data;

	if (flush)
		flush_socket(csock);
	ft_strcpy(data.data, "CMD: Transmission error ");
	ft_strcat(data.data, "for file ");
	ft_strcat(data.data, name);
	data.data_size = htonl(0);
	data.return_code = htonl(1);
	send(csock, &data, DATASIZE, 0);
	return (1);
}

int		count_char(char *str, char c)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = str;
	while (*str)
	{
		if (*str == c)
			i++;
		str++;
	}
	return (i);
}

int		exec_ls(char **cmd, int csock)
{
	pid_t		pid;
	int			status;

	if ((pid = fork()) == -1)
		return (send_error(csock, "cacaprout", 0));
	if (pid > 0)
	{
		printf("entered in fork parent for execution\n");
		wait4(pid, &status, 0, NULL);
		if (WEXITSTATUS(status) == 0)
			send_success(csock, "caca");
		else
			send_error(csock, "prout", 0);
		printf("exited parent -- Child exited with status [%d] -- [%d]\n", WEXITSTATUS(status), status);
	}
	else
	{
		dup2(csock, 1);
		dup2(csock, 2);
		execv("/bin/ls", cmd);
	}		
	return (0);
}

int		exec_pwd(char **cmd, int csock)
{
	pid_t		pid;
	int			status;

	if ((pid = fork()) == -1)
		return (send_error(csock, "cacaprout", 0));
	if (pid > 0)
	{
		printf("entered in fork parent for execution\n");
		wait4(pid, &status, 0, NULL);
		if (WEXITSTATUS(status) == 0)
			send_success(csock, "caca");
		else
			send_error(csock, "prout", 0);
		printf("exited parent -- Child exited with status [%d] -- [%d]\n", WEXITSTATUS(status), status);
	}
	else
	{
		dup2(csock, 1);
		dup2(csock, 2);
		execv("/bin/pwd", cmd);
	}
	return (0);
}

int		exec_cd(char **cmd, int csock)
{
	char	*path;
	char	*new_path;
	int		ret;

	ret = 0;
	path = NULL;
	new_path = NULL;
	path = getcwd(path, 255);
	if (chdir(cmd[1] ? ft_strjoin(cmd[1], "/") : "~") == 0)
	{
		new_path = getcwd(new_path, 255);
		if (count_char(new_path, '/') < count_char(path, '/'))
		{
			chdir(path);
			ft_printf("%s\n", "cd failure");
			ret = send_error(csock, "cd failure", 0);
		}
		ft_printf("%s\n", "cd success");
	}
	ft_strdel(&path);
	ft_strdel(&new_path);
	return (ret ? ret : send_success(csock, "cd success"));
}

int		exec_get(char **cmd, int csock)
{
	int				file_fd;
	unsigned long	transmit_left;
	struct stat		stat;
	t_data			data;
	int				r;
	int				part_nb;

	if (!cmd[1])
		return (send_error(csock, "no file given", 1));
	else if ((file_fd = open(cmd[1], O_RDONLY, 0)) == -1)
		return (send_error(csock, "file creation failed", 1));
	if (fstat(file_fd, &stat) < 0)
		return (send_error(csock, "failed to stat file", 0));
	ft_printf("file : %s -- size == %lu\n", cmd[1], stat.st_size);
	ft_bzero(&data, DATASIZE);
	data.data_size = htonl(stat.st_size);
	data.total_parts = htonl(stat.st_size / (BUFSIZE - 1) + 1);
	transmit_left = stat.st_size;
	part_nb = 1;
	ft_strcpy(data.data, cmd[1]);
	send(csock, &data, DATASIZE, 0);
	if (!rec_data(&data, csock))
		return (send_error(csock, "client error - get aborted", 0));
	ft_printf("return client : %s\n", data.data);
	while (transmit_left > 0)
	{
		data.part_nb = htonl(part_nb++);
		r = read(file_fd, &data.data, BUFSIZE - 1);
		data.data[r] = '\0';
		data.part_size = htonl(r); 
		send(csock, &data, DATASIZE, 0);
		ft_printf("transmit_left == %lu\n", transmit_left);
		transmit_left -= r;
	}
	close(file_fd);
	send_success(csock, cmd[1]);
	return (0);
}

int		exec_put(char **cmd, int csock)
{
	t_data			data;
	unsigned long	size;
	unsigned long	prev_part;
	int				file_fd;

	size = 0;
	prev_part = 1;
	if (!cmd[1])
		return (send_error(csock, "no file given", 1));
	else if ((file_fd = open(cmd[1], O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
		return (send_error(csock, "file creation failed", 1));
	while (1)
	{	
		recv(csock, &data, DATASIZE, 0);
		data.data_size = ntohl(data.data_size);
		data.total_parts = ntohl(data.total_parts);
		data.part_nb = ntohl(data.part_nb);
		data.part_size = ntohl(data.part_size);
		ft_printf("size == %lu -- part_size == %lu -- part == %lu/%lu\n%s\nfd == %d\n", data.data_size, data.part_size, data.part_nb, data.total_parts, data.data, file_fd);
		ft_printf("data == %50s\n", data.data);
		if (data.part_nb == prev_part++)
			write(file_fd, data.data, data.part_size);
		else
		{
			unlink(cmd[1]);
			return (send_error(csock, cmd[1], 0));
		}
		size += data.part_size;
		if (data.part_nb == data.total_parts)
		{
			if (size == data.data_size)
				send_success(csock, cmd[1]);
			else {
				unlink(cmd[1]);
				return (send_error(csock, cmd[1], 0));
			}
			break ;
		}
	}
	close(file_fd);
	return (0);
}

int		handle_connections(int port) {
	int					sock;
	int					cs = 0;
	unsigned int		cslen;
	struct sockaddr_in	csin;
	int					r;
	char				buf[BUFSIZE];
	pid_t				pid;
	char				**cmd;
	int					i;
	// int					status = 0;
	
	cmd = NULL;
	sock = create_server(port);
	while (42) {
		cs = accept(sock, (struct sockaddr *)&csin, &cslen);
		if ((pid = fork()) == -1)
			return (1);
		if (pid > 0) {
			printf("connection accepted on sock: %d\n", cs);
			close(cs);
			// wait4(pid, &status, 0, NULL);
			// printf("WIFEXITED == %d\n", WIFEXITED(status));
			// printf("WEXITSTATUS == %d\n", WEXITSTATUS(status));
		}
		else {
			while ((r = recv(cs, &buf, BUFSIZE-1, 0)) > 0) {
				ft_printf("r == %d\n", r);
				buf[r] = '\0';
				cmd = ft_strsplit(buf, ' ');
				i = 0;
				printf("command == %s\n", cmd[0]);
				while (cmd && i < CMDS_NB)
				{
					if (ft_strcmp(g_commands[i].id, cmd[0]) == 0)
					{
						g_commands[i].f(cmd, cs);
						break ;
					}
					i++;
				}
				ft_printf("end of command %s\n", cmd[0]);
				ft_tabdel(&cmd);
			}
			ft_printf("closing connection\n");
			close(cs);
			break ;
		}
	}
	close(sock);
	return (0);
}

int		main(int ac, char **av) {
	int					port;

	if (ac != 2)
		usage(av[0]);
	port = ft_atoi(av[1]);
	ft_printf("%sstart of function server%s\n", RED, NRM);
	handle_connections(port);
	ft_printf("%send of function server%s\n", RED, NRM);
	return (0);
}
