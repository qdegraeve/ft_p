#include <stdio.h>
#include "../../libft/include/ft_printf.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char	**ft_ls(char **args);


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

int		handle_connections(int port) {
	int					sock;
	int					cs = 0;
	unsigned int		cslen;
	struct sockaddr_in	csin;
	int					r;
	char				buf[1024];
	pid_t				pid;
	// int					status = 0;
	
	sock = create_server(port);
	while (42) {
		cs = accept(sock, (struct sockaddr *)&csin, &cslen);
		if ((pid = fork()) == -1)
			return (1);
		if (pid > 0) {
			close(cs);
			// wait4(pid, &status, 0, NULL);
			// printf("WIFEXITED == %d\n", WIFEXITED(status));
			// printf("WEXITSTATUS == %d\n", WEXITSTATUS(status));
		}
		else {
			
			while ((r = recv(cs, &buf, 1023, 0)) > 0) {
				buf[r] = '\0';
				ft_printf("cmp == %d\n", ft_strcmp(buf, "exit"));
				if (ft_strcmp(buf, "exit") == 0)
				{
					close(sock);
					return (0);
				}
				if (ft_strncmp(buf, "ls", 2) == 0)
				{
					char	**output;

					if (!(output = ft_ls(ft_strsplit(buf, ' '))))
						continue ;
					while (output && *output)
					{
						write(cs, *output, ft_strlen(*output));
						write(cs, "\n", 1);
						output++;
					}
					write(cs, "EOF", 3);
				}
			}
			close(cs);
		}
	}
	close(sock);
	return (0);
}

int		main(int ac, char **av) {
	int					port;
	pid_t				pid;

	if (ac != 2)
		usage(av[0]);
	port = ft_atoi(av[1]);
	if ((pid = fork()) < 0)
		return (1);
	if (pid > 0) {
		ft_printf("parent process\nchild pid == %d\n", pid);
		exit(EXIT_SUCCESS);
		wait4(pid, NULL, 0, NULL);
	}
	else {
		handle_connections(port);
	}
	ft_printf("end of function server\n");
	return (0);
}
