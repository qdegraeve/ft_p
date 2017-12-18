#include "client.h"

static char	*get_server_pwd()
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

static char	*get_pwd_prompt(char *path)
{
	int		i;

	if (!path)
		path = ft_strdup("Middle of nowhere");
	i = ft_strlen(path);
	while (i >= 0 && path[i] != '/')
		i--;
	return (path + i + 1);
}

static void	prompt(int error)
{
	char	*path;
	char	*server_pwd;

	path = NULL;
	path = getcwd(path, 255);
	server_pwd = get_server_pwd();
	if (error)
		ft_printf("%sl-> %s %s(s-> %s)%s: X ", CYN, get_pwd_prompt(path),
			YEL, server_pwd, RED);
	else
	{
		ft_printf("%sl-> %s %s(s-> %s)%s: <3 ", CYN, get_pwd_prompt(path),
			YEL, server_pwd, GRN);
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
				ft_printf("%sCommand not found\nNew commands coming soon\n%s",
					RED, NRM);
			ft_strdel(&line);
		}
	}
}