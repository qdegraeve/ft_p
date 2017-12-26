#include "client.h"

static int	send_error(int csock, char *name)
{
	t_data			data;

	ft_bzero(&data, DATASIZE);
	ft_strcpy(data.data, "Client: Transmission error : ");
	ft_strcat(data.data, name);
	data.data_size = htonl(0);
	data.return_code = htonl(1);
	send(csock, &data, DATASIZE, 0);
	return (1);
}

static int	open_file(int socket, t_data *data)
{
	int				file_fd;

	if (!rec_data(data, socket))
	{
		ft_printf("%s%s%s\n", data->return_code ? RED : GRN, data->data, NRM);
		return (-1);
	}
	if ((file_fd = open(data->data, O_WRONLY | O_CREAT | O_EXCL,
		S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
	{
		ft_putendl_fd("Failed to create file", 2);
		send_error(socket, "Failed to create file");
		rec_data(data, socket);
		ft_printf("%s%s%s\n", data->return_code ? RED : GRN, data->data, NRM);
		return (-1);
	}
	return (file_fd);
}

static int	check_termination(int params, t_data *data, unsigned long size,
	char *filename)
{
	int		error;
	int		file_fd;

	error = (params & 0xff);
	file_fd = (params & 0xffff0000) >> 16;
	if (!error && size == data->data_size)
		ft_putendl("Reception completed");
	else
	{
		ft_putendl_fd("Error during transmission : Abort", 2);
		unlink(filename);
		error = 1;
	}
	close(file_fd);
	return (error);
}

static int	receive_file(t_data *data, int file_fd, char *filename, int socket)
{
	int				ret;
	unsigned long	size;
	unsigned long	prev_part;

	ret = 0;
	size = 0;
	prev_part = 1;
	while (1)
	{
		rec_data(data, socket);
		if (!ret && data->part_nb == prev_part++)
			write(file_fd, data->data, data->part_size);
		else
			ret = 1;
		size += data->part_size;
		if (data->part_nb == data->total_parts)
			return (check_termination(ret | (file_fd << 16), data, size,
				filename));
	}
	return (0);
}

int			exec_get(char *cmd, int socket)
{
	t_data			data;
	int				file_fd;
	char			filename[256];

	ft_bzero(&data, DATASIZE);
	send(socket, cmd, ft_strlen(cmd), 0);
	send(socket, EOC, 2, 0);
	if ((file_fd = open_file(socket, &data)) == -1)
		return (1);
	ft_strcpy(filename, data.data);
	ft_strcpy(data.data, "File created successfully");
	send(socket, &data, DATASIZE, 0);
	return (receive_file(&data, file_fd, filename, socket));
}
