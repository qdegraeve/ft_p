#include "client.h"

int		send_error(int csock, char *name)
{
	t_data			data;

	ft_strcpy(data.data, "Client: Transmission error : ");
	ft_strcat(data.data, name);
	data.data_size = htonl(0);
	data.return_code = htonl(1);
	send(csock, &data, DATASIZE, 0);
	return (1);
}

int		open_file(int socket, t_data *data)
{
	int				file_fd;

	if (!rec_data(data, socket))
	{
		ft_putendl_fd(data->data, 2);
		return (-1);
	}
	if ((file_fd = open(data->data, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
	{
		ft_putendl_fd("Failed to create file", 2);
		return (-1);
	}
	return (file_fd);
}

int		exec_get(char *cmd, int socket)
{
	t_data			data;
	unsigned long	size;
	unsigned long	prev_part;
	int				file_fd;
	int				ret;
	char			filename[256];

	size = 0;
	ret = 0;
	prev_part = 1;
	send(socket, cmd, ft_strlen(cmd), 0);
	if ((file_fd = open_file(socket, &data)) == -1)
		return (1);
	ft_strcpy(filename, data.data);
	ft_strcpy(data.data, "File created successfully");
	send(socket, &data, DATASIZE, 0);
	while (1)
	{	
		rec_data(&data, socket);
		ft_printf("size == %lu -- part_size == %lu -- part == %lu/%lu\n%s\nfd == %d\n", data.data_size, data.part_size, data.part_nb, data.total_parts, data.data, file_fd);
		if (!ret && data.part_nb == prev_part++)
			write(file_fd, data.data, data.part_size);
		else
			ret = 1;
		size += data.part_size;
		if (data.part_nb == data.total_parts)
		{
			if (!ret && size == data.data_size)
				ft_printf("Reception of file %s completed\n", filename);
			else {
				ft_putendl_fd("Error during transmission : Abort", 2);
				unlink(filename);
				ret = 1;
			}
			break ;
		}
	}
	close(file_fd);
	return (ret);
}
