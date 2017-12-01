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

char	**ft_ls(char **args);


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
	// TODO: take output as argument and delete it each time
	char	*ls[] = {"-l", NULL};
	char	**output;
	int		i = 0;

	if (!(output = ft_ls(ls)))
		return;
	while (output && *output)
	{
		mvwaddstr(w, 3 + i++, 1, *output);
		output++;
	}
}

int 	rec_infos(int sock, WINDOW *win, int *i)
{
	char	buf[1024];
	int		ret;
	int		r;
	int		j;

	j = *i;			
	ret = 0;
	r = recv(sock, &buf, 1023, 0);
	buf[r] = '\0';
	char **test = ft_strsplit(buf, '\n');
	while(test && *test)
	{
		mvwaddstr(win, 3 + j++, 1, *test);
		// printf("buf == [%s] -- strcmp == %d", buf, ft_strcmp(*test, "EOF"));
		if ((ft_strcmp(*test, "EOF")) == 0)
			return (1);
		test++;
	}
	*i = j;
	return (ret);
}

int		user_interface(int sock)
{
	WINDOW	*mainwin;
	WINDOW	*menuwin;
	WINDOW	*local_win;
	WINDOW	*server_win;

	int		yMax;
	int		xMax;
	char	buf[1024];
	int		i;

	if ((mainwin = initscr()) == NULL)
		return (1);
	getmaxyx(mainwin, yMax, xMax);
	printf("yMax == %d\nxMax == %d\n", yMax, xMax);
	cbreak();

	local_win = subwin(mainwin, yMax - 8 - 10, (xMax - 6) / 2, 14, 2);
	server_win = subwin(mainwin, yMax - 8 - 10, (xMax - 6) / 2, 14, (xMax - 6) / 2 + 4);
	menuwin = subwin(mainwin, 8, xMax - 12, 4, 6);
	scrollok(menuwin, TRUE);
	idlok(menuwin, TRUE);
	box(local_win, 0, 0);
	box(server_win, 0, 0);
	box(menuwin, 0, 0);
	refresh();
	wrefresh(menuwin);
	wrefresh(local_win);
	wrefresh(server_win);
	output_ls(local_win);

	keypad(menuwin, true);
	while (1)
	{
		mvwaddstr(menuwin, 1, 1, "ft_p: ");
		wclrtoeol(menuwin);
		box(menuwin, 0, 0);
		wgetnstr(menuwin, buf, xMax - 15 - 6);
		werase(local_win);
		output_ls(local_win);
		mvwaddstr(local_win, 1, 1, buf);
		if (ft_strcmp(buf, "exit") == 0)
		{
			send(sock, "exit", 4, 0);
			break;
		}
		if (ft_strcmp(buf, "clear") == 0)
			werase(menuwin);
		if (ft_strncmp(buf, "ls", 2) == 0)
		{
			i = 0;
			send(sock, buf, ft_strlen(buf), 0);
			while (!rec_infos(sock, server_win, &i)) ;
		}
		wscrl(menuwin, -1);
		box(local_win, 0, 0);
		wrefresh(menuwin);
		wrefresh(server_win);
		wrefresh(local_win);

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
	user_interface(sock);
	close(sock);
	return (0);
}