#ifndef _CLIENT_H
# define _CLIENT_H

# include "ftp.h"

typedef struct 			s_client_cmds
{
	char				*id;
	int					(*f)(char *cmd, int csock);
}						t_client_cmds;

void	prompt();
int		exec_cd(char *cmd, int sock);
int		exec_fork(char *cmd, int sock);
int		exec_get(char *cmd, int sock);
int		exec_put(char *cmd, int sock);
int		get_current_socket(int current);
void	signal_catcher(void);

#endif