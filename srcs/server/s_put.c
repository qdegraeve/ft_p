#include "server.h"

int		exec_put(const char **cmd, int csock)
{
	t_data			data;
	unsigned long	size;
	unsigned long	prev_part;
	int				file_fd;

	size = 0;
	prev_part = 1;
	if (!cmd[1])
		return (send_error(csock, "No file given", 1));
	else if ((file_fd = open(cmd[1], O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
		return (send_error(csock, "File creation failed", 1));
	while (1)
	{	
		recv(csock, &data, DATASIZE, 0);
		data.data_size = ntohl(data.data_size);
		data.total_parts = ntohl(data.total_parts);
		data.part_nb = ntohl(data.part_nb);
		data.part_size = ntohl(data.part_size);
		ft_printf("size == %lu -- part_size == %lu -- part == %lu/%lu\n%s\nfd == %d\n", data.data_size, data.part_size, data.part_nb, data.total_parts, data.data, file_fd);
		ft_printf("data == %50s\n", data.data);
		if (data.part_nb == prev_part++)
			write(file_fd, data.data, data.part_size);
		else
		{
			unlink(cmd[1]);
			return (send_error(csock, "Error during transmission -- Put aborted", 0));
		}
		size += data.part_size;
		if (data.part_nb == data.total_parts)
		{
			if (size == data.data_size)
				send_success(csock, "File copied successfully");
			else {
				unlink(cmd[1]);
				return (send_error(csock, "Error during transmission -- Put aborted", 0));
			}
			break ;
		}
	}
	close(file_fd);
	return (0);
}
