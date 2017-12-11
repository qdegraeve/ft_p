#ifndef _SERVER_H
# define _SERVER_H

# include "ftp.h"

typedef struct			s_server_cmds
{
	char				*id;
	int					(*f)(char **cmd, int csock);
}						t_server_cmds;

int		exec_ls(char **cmd, int socket);
int		exec_cd(char **cmd, int socket);
int		exec_pwd(char **cmd, int socket);
int		exec_get(char **cmd, int socket);
int		exec_put(char **cmd, int socket);

#endif