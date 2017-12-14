#include "ftp.h"

int			has_operator(const char **cmd)
{
	int 	i;

	i = 0;
	if (!cmd)
		return (0);
	while (cmd[i])
	{
		if (cmd[i][0] == '&' || cmd[i][0] == ';' || cmd[i][0] == '|')
			return (1);
		i++;
	}
	return (0);
}