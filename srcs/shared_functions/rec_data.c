# include "ftp.h"

int		rec_data(t_data *data, int socket)
{
	ft_bzero(data, DATASIZE);
	recv(socket, data, DATASIZE, 0);
	data->data_size = ntohl(data->data_size);
	data->return_code = ntohl(data->return_code);
	data->total_parts = ntohl(data->total_parts);
	data->part_nb = ntohl(data->part_nb);
	data->part_size = ntohl(data->part_size);
	return (data->data_size);
}
