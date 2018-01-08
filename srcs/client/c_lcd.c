#include "client.h"

int		exec_lcd(char *command, int sock)
{
	char	*path;
	char	**args;

	sock = 0;
	path = NULL;
	args = ft_strsplit(command, ' ');
	if (ft_tablen(args) != 2)
	{
		ft_putendl_fd("Usage: cd [path]", 2);
		ft_tabdel(&args);
		return (1);
	}
	if (chdir((args[1] = ft_cjoin(args[1], "/"))) == -1)
	{
		ft_putendl_fd("cd : Access denied", 2);
		return (1);
	}
	ft_strdel(&path);
	ft_tabdel(&args);
	return (0);
}
