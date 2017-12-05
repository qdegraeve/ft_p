#include "ftp.h"

static const t_commands g_commands[CMDS_NB] = {
	{ "cd", &exec_cd },
	{ "pwd", &exec_pwd },
	{ "ls", &exec_ls }
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

int		exec_cmd(int index, char **cmd, int csock)
{
	pid_t	pid;
	int		status;

	if ((pid = fork()) == -1)
		return (-1);
	if (pid > 0)
	{
		printf("entered in fork parent for execution\n");
		wait4(pid, &status, 0, NULL);
		printf("exited parent -- Child exited with status [%d] -- [%d]\n", WEXITSTATUS(status), status);
	}
	else
	{
		dup2(csock, 1);
		dup2(csock, 2);
		g_commands[index].f(cmd);
	}
	return (WEXITSTATUS(status));

}

int		exec_ls(char **cmd)
{
	execv("/bin/ls", cmd);
	// char	**output;

	// output = ft_ls(cmd);
	// while (output && *output)
	// {
	// 	send(cs, *output, ft_strlen(*output), 0);
	// 	send(cs, "\n", 1, 0);
	// 	output++;
	// }
	// send(cs, "EOT", 3, 0);
	return (0);
}

int		exec_pwd(char **cmd)
{
	execv("/bin/pwd", cmd);
	return (0);
}

int		exec_cd(char **cmd)
{
	char	*path;
	char	*new_path;

	path = NULL;
	new_path = NULL;
	path = getcwd(path, 255);
	if (chdir(cmd[1] ? ft_cjoin(cmd[1], "/") : "~") == 0)
	{
		new_path = getcwd(new_path, 255);
		if (count_char(new_path, '/') < count_char(path, '/'))
			chdir(path);
		ft_printf("%s\n", "cd failure");
		exit(EXIT_FAILURE);
	}
	ft_strdel(&path);
	if (new_path)
	ft_strdel(&new_path);
	ft_printf("%s\n", "cd success");
	exit(EXIT_SUCCESS);
	return (0);
}

int		handle_connections(int port) {
	int					sock;
	int					cs = 0;
	unsigned int		cslen;
	struct sockaddr_in	csin;
	int					r;
	char				buf[1024];
	pid_t				pid;
	char				**cmd;
	int					i;
	// int					status = 0;
	
	cmd = NULL;
	sock = create_server(port);
	while (42) {
		DEBUG
		cs = accept(sock, (struct sockaddr *)&csin, &cslen);
		DEBUG
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
			while ((r = recv(cs, &buf, 1023, 0)) > 0) {
				buf[r] = '\0';
				cmd = ft_strsplit(buf, ' ');
				i = 0;
				printf("command == %s\n", cmd[0]);
				while (i < CMDS_NB)
				{
					if (ft_strcmp(g_commands[i].id, cmd[0]) == 0)
						exec_cmd(i, cmd, cs);
					i++;
				}
				ft_tabdel(&cmd);
			}
			close(cs);
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
