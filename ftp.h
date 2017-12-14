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
#include <sys/types.h> 
#include <sys/stat.h> 

# define BUFSIZE	1024
# define DATASIZE	sizeof(t_data)
# define CMDS_NB	5

typedef struct			s_data
{
	unsigned long		data_size; /* if data_size == 0 --> end of command */
	unsigned long		return_code;
	unsigned long		total_parts; /* used to control integrity of file and end of transmission */
	unsigned long		part_nb;
	unsigned long		part_size;
	char				data[BUFSIZE];
}						t_data;

char		**ft_ls(char **args);
int			rec_data(t_data *data, int socket);
int			has_operator(const char **cmd);


#endif