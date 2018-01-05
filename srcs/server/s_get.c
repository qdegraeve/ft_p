#include "server.h"

static int	create_file(t_data *data, int csock, const char **cmd)
{
	struct stat		stat;
	int				ret;

	ret = 0;
	if (!cmd[1])
	{
		send_error(csock, "No file given", 0);
		return (-1);
	}
	if ((ret = open(extract_from_path((char*)cmd[1]), O_RDONLY, 0)) == -1)
		send_error(csock, "Failed to open file", 0);
	if (ret != -1 && fstat(ret, &stat) < 0)
	{
		send_error(csock, "Failed to stat file -- get aborted", 0);
		close(ret);
		return (-1);
	}
	if (ret != -1)
	{
		ft_bzero(data, DATASIZE);
		data->data_size = htonl(stat.st_size);
		data->total_parts = htonl(stat.st_size / (BUFSIZE - 1) + 1);
	}
	return (ret);
}

int		exec_get(const char **cmd, int csock)
{
	int				file_fd;
	t_data			data;
	int				r;
	int				part_nb;

	if ((file_fd = create_file(&data, csock, cmd)) == -1)
		return (1);
	ft_strcpy(data.data, cmd[1]);
	part_nb = 1;
	send(csock, &data, DATASIZE, 0);
	if (!rec_data(&data, csock))
		return (send_error(csock, "Client error - get aborted", 0));
	while (data.part_nb < data.total_parts)
	{
		data.part_nb = htonl(part_nb++);
		r = read(file_fd, &data.data, BUFSIZE - 1);
		data.data[r] = '\0';
		data.part_size = htonl(r);
		send(csock, &data, DATASIZE, 0);
	}
	close(file_fd);
	send_success(csock, "File sent successfully");
	return (0);
}
