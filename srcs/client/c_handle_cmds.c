#include "client.h"

static const t_client_cmds	g_commands[CMDS_NB] = {
	{ "cd", 2, &exec_cd },
	{ "pwd", 3, &exec_fork },
	{ "ls", 2, &exec_fork },
	{ "get", 3, &exec_get },
	{ "put", 3, &exec_put },
	{ "lcd", 3, &exec_lcd },
	{ "lls", 3, &exec_lls },
	{ "lpwd", 4, &exec_lpwd }
};

int		exec_cd(char *cmd, int sock)
{
	send(sock, cmd, ft_strlen(cmd), 0);
	send(sock, EOC, 2, 0);
	return (0);
}

int		exec_fork(char *cmd, int sock)
{
	int			i;
	char		buf[42000];

	send(sock, cmd, ft_strlen(cmd), 0);
	send(sock, EOC, 2, 0);
	i = recv(sock, buf, 41999, 0);
	buf[i] = '\0';
	write(1, buf, i);
	return (0);
}

int		exec_cmds(int sock, char *cmd)
{
	int			i;
	t_data		data;
	int			ret;

	i = -1;
	ret = 0;
	while (++i < CMDS_NB)
	{
		if (ft_strncmp(g_commands[i].id, cmd, g_commands[i].length) == 0)
		{
			if (((ret = g_commands[i].f(cmd, sock)) == 0) && i < 5)
			{
				rec_data(&data, sock);
				ft_printf("%s%s%s\n", data.return_code ? RED : GRN,
					data.data, NRM);
				return (data.return_code);
				if (i == 0 && data.return_code == 0)
					get_server_pwd(NULL, 1);
			}
			return (ret);
		}
	}
	return (-1);
}
