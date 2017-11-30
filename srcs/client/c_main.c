#include <stdio.h>
#include "../../libft/include/ft_printf.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ncurses.h>
#include <curses.h>

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

void	output_ls(WINDOW *w)
{
	pid_t	pid;
	char	*ls[] = {"/bin/ls", "-l", NULL};
	char	*buf;
	int		fd;
	int		saved_fd;
	int		i = 3;

	if ((pid = fork()) == -1)
		return ;
	if (pid == 0)
	{
		noecho();
		fd = open("/tmp/output", O_CREAT | O_RDWR);
		saved_fd = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		execl(ls[0], ls[1]);
		close(fd);
		dup2(STDOUT_FILENO, saved_fd);
		echo();
		exit(0);
	}
	else
	{
		wait4(pid, NULL, 0, NULL);
		fd = open("/tmp/output", O_RDONLY);
		while (get_next_line(fd, &buf) > 0)
		{
			mvwaddstr(w, i++, 1, buf);
			free(buf);
		}
		close(fd);
	}
}

int		user_interface()
{
	WINDOW	*mainwin;
	WINDOW	*menuwin;
	WINDOW	*filewin;

	int		yMax;
	int		xMax;
	char	buf[1024];


	if ((mainwin = initscr()) == NULL)
		return (1);
	getmaxyx(mainwin, yMax, xMax);
	printf("yMax == %d\nxMax == %d\n", yMax, xMax);
	cbreak();

	filewin = subwin(mainwin, yMax - 8 - 10, xMax - 12, 14, 6);
	menuwin = subwin(mainwin, 8, xMax - 12, 4, 6);
	scrollok(menuwin, TRUE);
	idlok(menuwin, TRUE);
	box(filewin, 0, 0);
	box(menuwin, 0, 0);
	refresh();
	wrefresh(menuwin);
	wrefresh(filewin);

	keypad(menuwin, true);
	while (1)
	{
		mvwaddstr(menuwin, 1, 1, "ft_p: ");
		wclrtoeol(menuwin);
		box(menuwin, 0, 0);
		wgetnstr(menuwin, buf, xMax - 15 - 6);
		werase(filewin);
		output_ls(filewin);
		mvwaddstr(filewin, 1, 1, buf);
		if (ft_strcmp(buf, "exit") == 0)
			break;
		if (ft_strcmp(buf, "clear") == 0)
			werase(menuwin);
		wscrl(menuwin, -1);
		box(filewin, 0, 0);
		wrefresh(menuwin);
		wrefresh(filewin);

	}
	endwin();
	return(0);
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
	user_interface();
	close(sock);
	return (0);
}