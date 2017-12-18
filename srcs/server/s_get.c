#include "server.h"

int		exec_get(const char **cmd, int csock)
{
	int				file_fd;
	unsigned long	transmit_left;
	struct stat		stat;
	t_data			data;
	int				r;
	int				part_nb;

	if (!cmd[1])
		return (send_error(csock, "No file given", 0));
	else if ((file_fd = open(cmd[1], O_RDONLY, 0)) == -1)
		return (send_error(csock, "Failed to open file:", 0));
	if (fstat(file_fd, &stat) < 0)
		return (send_error(csock, "Failed to stat file -- get aborted", 0));
	ft_printf("file : %s -- size == %lu\n", cmd[1], stat.st_size);
	ft_bzero(&data, DATASIZE);
	data.data_size = htonl(stat.st_size);
	data.total_parts = htonl(stat.st_size / (BUFSIZE - 1) + 1);
	transmit_left = stat.st_size;
	part_nb = 1;
	ft_strcpy(data.data, cmd[1]);
	send(csock, &data, DATASIZE, 0);
	DEBUG
	if (!rec_data(&data, csock))
		return (send_error(csock, "Client error - get aborted", 0));
	DEBUG
	ft_printf("return client : %s\n", data.data);
	while (transmit_left > 0)
	{
		data.part_nb = htonl(part_nb++);
		r = read(file_fd, &data.data, BUFSIZE - 1);
		data.data[r] = '\0';
		data.part_size = htonl(r);
		send(csock, &data, DATASIZE, 0);
		transmit_left -= r;
	}
	close(file_fd);
	send_success(csock, "File sent successfully");
	return (0);
}
