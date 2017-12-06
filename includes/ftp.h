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
# include <ncurses.h>
# include <curses.h>
#include <sys/types.h> 
#include <sys/stat.h> 

# define BUFSIZE	1024
# define DATASIZE	sizeof(t_data)
# define CMDS_NB	5

typedef struct			s_server_cmds
{
	char				*id;
	int					(*f)(char **cmd, int csock);
}						t_server_cmds;

typedef struct 			s_client_cmds
{
	char				*id;
	int					(*f)(char *cmd, int csock);
}						t_client_cmds;

typedef struct			s_data
{
	unsigned short		return_code;
	unsigned long		data_size; // if data_size == 0 --> end of command
	unsigned long		total_parts;
	unsigned long		part_nb;
	unsigned long		part_size;
	char				data[BUFSIZE];
}						t_data;

char	**ft_ls(char **args);
void	prompt();

#endif