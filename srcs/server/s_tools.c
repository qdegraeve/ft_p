#include "server.h"

char	*get_base_path(void)
{
	static char		*base_path = NULL;

	if (!base_path)
		base_path = getcwd(base_path, 255);
	return (base_path);
}
