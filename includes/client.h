#ifndef _CLIENT_H
# define _CLIENT_H

# include "ftp.h"
# include <sys/time.h>

# define CMDS_NB	8

typedef struct			s_client_cmds
{
	char				*id;
	int					length;
	int					(*f)(char *cmd, int csock);
}						t_client_cmds;

/*
**			USER INTERFACE
*/
void					user_interface(int sock);
char					*get_server_pwd(char *new, int ask);

/*
**			COMMANDS HANDLERS
*/
int						exec_cmds(int sock, char *cmd);
int						exec_cd(char *cmd, int sock);
int						exec_fork(char *cmd, int sock);
int						exec_get(char *cmd, int sock);
int						exec_put(char *cmd, int sock);
int						exec_lcd(char *command, int sock);
int						exec_lls(char *command, int sock);
int						exec_lpwd(char *command, int sock);

/*
**			TOOLS
*/
int						get_current_socket(int current);
void					signal_catcher(void);

#endif
