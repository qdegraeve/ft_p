#ifndef _SERVER_H
# define _SERVER_H

# include "ftp.h"

# define CMDS_NB	6

typedef struct			s_server_cmds
{
	char				*id;
	int					(*f)(const char **cmd, int csock);
}						t_server_cmds;


/*
**			HANDLE COMMANDS
*/
int			send_success(int csock, const char *name);
int			send_error(int csock, const char *name, int flush);
void		handle_cmds(int socket);

/*
**			COMMANDS
*/
int			exec_ls(const char **cmd, int socket);
int			exec_cd(const char **cmd, int socket);
int			exec_pwd(const char **cmd, int socket);
int			exec_get(const char **cmd, int socket);
int			exec_put(const char **cmd, int socket);
int			exec_where(const char **cmd, const int csock);
int			has_operator(const char **cmd);

/*
**			MISCS
*/
char		*get_base_path();

#endif