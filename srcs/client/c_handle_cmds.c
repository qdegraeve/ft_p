#include "client.h"

static const t_client_cmds	g_commands[CMDS_NB] = {
	{ "cd", &exec_cd },
	{ "pwd", &exec_fork },
	{ "ls", &exec_fork },
	{ "get", &exec_get },
	{ "put", &exec_put }
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
				data.return_code = ntohl(data.return_code);
				data.total_parts = ntohl(data.total_parts);
				data.part_size = ntohl(data.part_size);
				data.part_nb = ntohl(data.part_nb);
				ft_printf("%s%s%s\n", data.return_code ? RED : GRN, data.data, NRM);
				return (data.return_code);
			}
			return (1);
		}
	}
	return (-1);
}
