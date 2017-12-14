#include "server.h"

int			exec_cd(const char **cmd, const int csock)
{
	char	*path;
	char	*new_path;
	int		ret;

	ret = 0;
	path = NULL;
	new_path = NULL;
	path = get_base_path();
	if (chdir(cmd[1] ? ft_strjoin(cmd[1], "/") : path) == 0)
	{
		new_path = getcwd(new_path, 255);
		if (ft_strncmp(new_path, path, ft_strlen(path)) != 0)
		{
			chdir(path);
			ft_printf("cd : %s is before %s. Access denied\n", new_path, path);
			ret = send_error(csock, "cd : Access denied", 0);
		}
		ft_printf("cd : moved to %s\n", new_path);
	}
	else 
	{
		ft_printf("%s\n", "cd : chdir failed");
		ret = send_error(csock, "cd : Access denied", 0);
	}		
	ft_strdel(&new_path);
	return (ret ? ret : send_success(csock, "cd : Success"));
}
