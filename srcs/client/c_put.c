#include "client.h"

int		exec_put(char *cmd, int sock)
{
	char			**args;
	int				file_fd;
	unsigned long	transmit_left;
	struct stat		stat;
	t_data			data;
	int				r;
	int				part_nb;
	int				ret;

	ret = 0;
	ft_bzero(&data, DATASIZE);
	args = ft_strsplit(cmd, ' ');
	if ((!args || !args[1]) && (ret = 1))
		ft_printf("put: No file given\n");
	else if ((file_fd = open(args[1], O_RDONLY)) == -1 && (ret = 1))
		ft_printf("put: failed to open file: %s\n", args[1]);
	else if (fstat(file_fd, &stat) < 0 && (ret = 1))
		ft_printf("put: failed to stat file: %s\nTransfer aborted", args[1]);
	if (ret)
	{
		ft_tabdel(&args);
		return (ret);
	}
	send(sock, cmd, ft_strlen(cmd), 0);
	data.data_size = htonl(stat.st_size);
	data.total_parts = htonl(stat.st_size / (BUFSIZE - 1) + 1);
	transmit_left = stat.st_size;
	part_nb = 1;
	while (transmit_left > 0)
	{
		data.part_nb = htonl(part_nb++);
		r = read(file_fd, &data.data, BUFSIZE - 1);
		data.data[r] = '\0';
		data.part_size = htonl(r);
		send(sock, &data, DATASIZE, 0);
		transmit_left -= r;
	}
	close(file_fd);
	return (0);
}
