#ifndef FTP_H
# define FTP_H

# include <stdio.h>
# include "../libft/include/ft_printf.h"
# include <stdlib.h>
# include <unistd.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/stat.h>

# define BUFSIZE	1024
# define DATASIZE	sizeof(t_data)
# define EOC		"\r\n"

typedef struct			s_data
{
	unsigned long		data_size;
	unsigned long		return_code;
	unsigned long		total_parts;
	unsigned long		part_nb;
	unsigned long		part_size;
	char				data[BUFSIZE];
}						t_data;

char					**ft_ls(char **args);
int						rec_data(t_data *data, int socket);
int						get_next_cmd(int const fd, char **line);
char					*extract_from_path(char *path);

#endif
