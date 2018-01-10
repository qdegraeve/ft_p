#include "server.h"

static const t_server_cmds g_commands[CMDS_NB] = {
	{ "cd", &exec_cd },
	{ "pwd", &exec_pwd },
	{ "ls", &exec_ls },
	{ "put", &exec_put },
	{ "get", &exec_get },
	{ "where", &exec_where }
};

static void	flush_socket(int socket)
{
	t_data			data;

	ft_bzero(&data, DATASIZE);
	while (1)
	{
		recv(socket, &data, DATASIZE, 0);
		data.data_size = ntohl(data.data_size);
		data.total_parts = ntohl(data.total_parts);
		data.part_nb = ntohl(data.part_nb);
		data.part_size = ntohl(data.part_size);
		ft_printf("part_nb == %lu / %lu\n", data.part_nb, data.total_parts);
		if (data.part_nb == data.total_parts)
			break ;
	}
}

int			send_success(int csock, const char *name)
{
	t_data			data;

	ft_bzero(&data, DATASIZE);
	ft_strcpy(data.data, "Server: ");
	ft_strcat(data.data, name);
	data.data_size = htonl(0);
	data.return_code = htonl(0);
	send(csock, &data, DATASIZE, 0);
	return (0);
}

int			send_error(int csock, const char *name, int flush)
{
	t_data			data;

	ft_bzero(&data, DATASIZE);
	if (flush)
		flush_socket(csock);
	ft_strcpy(data.data, "Server: ");
	ft_strcat(data.data, name);
	data.data_size = htonl(0);
	data.return_code = htonl(1);
	send(csock, &data, DATASIZE, 0);
	return (1);
}

void		handle_cmds(int socket)
{
	int				i;
	char			*line;
	char			**cmd;

	cmd = NULL;
	line = NULL;
	i = 0;
	while (get_next_cmd(socket, &line) > 0)
	{
		cmd = ft_strsplit(line, ' ');
		ft_printf("command == [%s]\n", cmd[0]);
		i = 0;
		while (cmd && i < CMDS_NB)
		{
			if (ft_strcmp(g_commands[i].id, cmd[0]) == 0)
			{
				g_commands[i].f((const char**)cmd, socket);
				break ;
			}
			i++;
		}
		ft_printf("end of command [%s]\n", cmd[0]);
		ft_tabdel(&cmd);
	}
}
