#include "ftp.h"

static char	*clean_tmp(char *tmp, int k)
{
	char *stock;

	stock = NULL;
	stock = ft_strnew(ft_strlen(tmp + k));
	ft_strncpy(stock, tmp + k, ft_strlen(tmp + k));
	ft_strdel(&tmp);
	return (stock);
}

static int	check_crln(char *s)
{
	int			i;

	i = 0;
	if (s == NULL)
		return (-1);
	while (s[i] && s[i + 1])
	{
		if (s[i] == '\r' && s[i + 1] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

int		get_next_cmd(int const fd, char **line)
{
	int			ret;
	int			k;
	static char	*tmp[256] = {NULL};
	char		*buf;

	buf = NULL;
	buf = ft_strnew(BUF_SIZE);
	if ((ret = recv(fd, buf, BUF_SIZE, 0)) == -1 || line == NULL || buf == NULL)
		return (-1);
	buf[ret] = '\0';
	tmp[fd] = new_join(tmp[fd], buf);
	k = check_crln(tmp[fd]);
	(ret == 0 && !tmp[fd][0]) ? ft_strdel(&tmp[fd]) : 0;
	if (ret == 0 && !tmp[fd])
		return (0);
	if (k >= 0)
	{
		*line = (k >= 0) ? ft_strsub(tmp[fd], 0, k) : ft_strdup(tmp[fd]);
		if (k == -1)
			ft_strdel(&tmp[fd]);
		else
			tmp[fd] = clean_tmp(tmp[fd], k + 2);
		return (1);
	}
	return (get_next_cmd(fd, line));
}
