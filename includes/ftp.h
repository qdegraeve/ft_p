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

# define CMDS_NB	3

typedef struct			s_commands
{
	char				*id;
	int					(*f)(char **cmd);
}						t_commands;


char	**ft_ls(char **args);
void	prompt();

int		exec_ls(char **cmd);
int		exec_cd(char **cmd);
int		exec_pwd(char **cmd);

#endif