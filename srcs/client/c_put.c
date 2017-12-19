#include "client.h"

static int	proceed_file(char *cmd, t_data *data)
{
	char			**args;
	int				ret;
	struct stat		stat;

	ret = 0;
	args = ft_strsplit(cmd, ' ');
	if ((!args || !args[1]))
	{
		ft_printf("put: No file given\n");
		ret = -1;
	}
	else if ((ret = open(args[1], O_RDONLY)) == -1)
		ft_printf("put: failed to open file: %s\n", args[1]);
	else if (fstat(ret, &stat) < 0)
	{
		ft_printf("put: failed to stat file: %s\nTransfer aborted", args[1]);
		ret = -1;
	}
	else
	{
		data->data_size = htonl(stat.st_size);
		data->total_parts = htonl(stat.st_size / (BUFSIZE - 1) + 1);
	}
	ft_tabdel(&args);
	return (ret);
}

int			exec_put(char *cmd, int sock)
{
	int				file_fd;
	t_data			data;
	int				r;
	unsigned long	part_nb;

	send(sock, cmd, ft_strlen(cmd), 0);
	ft_bzero(&data, DATASIZE);
	if (!rec_data(&data, sock)
		|| (file_fd = proceed_file(cmd, &data)) == -1)
	{
		ft_printf("%s%s%s\n", data.return_code ? RED : GRN, data.data, NRM);
		return (1);
	}
	part_nb = 1;
	while (data.part_nb < data.total_parts)
	{
		data.part_nb = htonl(part_nb++);
		r = read(file_fd, &data.data, BUFSIZE - 1);
		data.data[r] = '\0';
		data.part_size = htonl(r);
		send(sock, &data, DATASIZE, 0);
	}
	close(file_fd);
	return (0);
}
