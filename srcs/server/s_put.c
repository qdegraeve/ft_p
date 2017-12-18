#include "server.h"

static int	check_termination(int params, t_data *data, unsigned long size,
	const char *filename)
{
	int				file_fd;
	int				csock;
	int				ret;

	csock = (params & 0xff);
	ret = (params & 0xff00) >> 8;
	file_fd = (params & 0xff0000) >> 16;
	if (!ret)
	{
		if (size == data->data_size)
			send_success(csock, "File copied successfully");
		else
			ret = send_error(csock, "Incomplete file received", 0);
	}
	if (ret)
		unlink(filename);
	close(file_fd);
	return (ret);

}

static int	receive_file(t_data *data, int file_fd, const char *filename,
	int csock)
{
	int				ret;
	unsigned long	size;
	unsigned long	prev_part;

	ret = 0;
	size = 0;
	prev_part = 1;
	while (!ret)
	{
		rec_data(data, csock);
		if (data->part_nb == prev_part++)
			write(file_fd, data->data, data->part_size);
		else
			ret = send_error(csock, "Transmission error -- Put aborted", 0);
		size += data->part_size;
		if (ret || data->part_nb == data->total_parts)
			return (check_termination(csock | ret << 8 | (file_fd << 16), data,
				size, filename));
	}
	return (0);
}

int		exec_put(const char **cmd, int csock)
{
	t_data			data;
	int				file_fd;

	ft_bzero(&data, DATASIZE);
	if (!cmd[1])
		return (send_error(csock, "No file given", 0));
	else if ((file_fd = open(cmd[1], O_RDWR | O_CREAT | O_EXCL,
		S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
		return (send_error(csock, "File creation failed", 0));
	else
	{
		data.data_size = htonl(42);
		send(csock, &data, DATASIZE, 0);
	}
	return (receive_file(&data, file_fd, cmd[1], csock));
}
