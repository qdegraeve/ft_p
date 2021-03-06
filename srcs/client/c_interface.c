#include "client.h"

char		*get_server_pwd(char *new, int ask)
{
	static char		*pwd = NULL;
	char			cmd[8];
	char			buf[256];
	int				sock;
	int				i;

	ft_bzero(cmd, 8);
	ft_bzero(buf, 256);
	if (new)
	{
		ft_strdel(&pwd);
		pwd = ft_strdup(new);
	}
	else if (!pwd || ask)
	{
		sock = get_current_socket(0);
		ft_strcpy(cmd, "where\r\n");
		send(sock, cmd, ft_strlen(cmd), 0);
		i = recv(sock, buf, 255, 0);
		buf[i] = '\0';
		ft_strdel(&pwd);
		pwd = ft_strdup(buf);
	}
	return (pwd);
}

static void	prompt(int error)
{
	char	*path;
	char	*server_pwd;

	path = NULL;
	path = getcwd(path, 255);
	server_pwd = get_server_pwd(NULL, 0);
	if (error)
		ft_printf("%sl-> %s %s(s-> %s)%s: X ", CYN, extract_from_path(path),
			YEL, server_pwd, RED);
	else
	{
		ft_printf("%sl-> %s %s(s-> %s)%s: <3 ", CYN, extract_from_path(path),
			YEL, server_pwd, GRN);
	}
	ft_printf("%s", NRM);
	ft_strdel(&path);
}

void		user_interface(int sock)
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
				break ;
			}
			else if ((error = exec_cmds(sock, line)) == -1)
				ft_printf("%sCommand not found\nNew commands coming soon\n%s",
					RED, NRM);
			ft_strdel(&line);
		}
	}
}
