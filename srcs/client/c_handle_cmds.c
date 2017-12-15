#include "client.h"

static const t_client_cmds	g_commands[CMDS_NB] = {
	{ "cd", &exec_cd },
	{ "pwd", &exec_fork },
	{ "ls", &exec_fork },
	{ "get", &exec_get },
	{ "put", &exec_put },
	{ "lcd", &exec_lcd },
	{ "lls", &exec_lls },
	{ "lpwd", &exec_lpwd }
};

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

int		exec_cmds(int sock, char *cmd)
{
	int			i;
	int			j;
	t_data		data;
	int			ret;

	i = 0;
	j = -1;
	ret = 0;
	while (cmd[i] && cmd[i] != ' ')
		i++;
	while (i && ++j < CMDS_NB)
	{
		if (ft_strncmp(g_commands[j].id, cmd, i) == 0)
		{
			if (((ret = g_commands[j].f(cmd, sock)) == 0) && j < 5)
			{
				rec_data(&data, sock);
				ft_printf("%s%s%s\n", data.return_code ? RED : GRN,
					data.data, NRM);
				return (data.return_code);
			}
			return (ret);
		}
	}
	return (-1);
}
