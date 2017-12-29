#ifndef _SERVER_H
# define _SERVER_H

# include "ftp.h"

# define CMDS_NB		6
# define ERROR_MSG_BEG	"Command format invalid: operators [&|;], option -R and"
# define ERROR_MSG_END	" files or folder names not accepted\n"
# define ERROR_MSG		ERROR_MSG_BEG ERROR_MSG_END
# define ERROR_LENGTH	sizeof(ERROR_MSG)

typedef struct			s_server_cmds
{
	char				*id;
	int					(*f)(const char **cmd, int csock);
}						t_server_cmds;

/*
**			HANDLE COMMANDS
*/
int						send_success(int csock, const char *name);
int						send_error(int csock, const char *name, int flush);
void					handle_cmds(int socket);

/*
**			COMMANDS
*/
int						exec_ls(const char **cmd, int socket);
int						exec_cd(const char **cmd, int socket);
int						exec_pwd(const char **cmd, int socket);
int						exec_get(const char **cmd, int socket);
int						exec_put(const char **cmd, int socket);
int						exec_where(const char **cmd, const int csock);
int						has_operator(const char **cmd);

/*
**			MISCS
*/
char					*get_base_path();
void					handle_sigchld(int signal);
int						nb_connections(int action);

#endif
