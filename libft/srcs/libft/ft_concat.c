#include "libft.h"

char		*ft_concat(int n, ...)
{
	va_list	ap;
	char	*junk;
	char	*res;

	va_start(ap, n);
	res = NULL;
	while (--n >= 0)
	{
		junk = res;
		res = ft_strjoin(res, va_arg(ap, char*));
		if (junk != NULL)
			ft_strdel(&junk);
	}
	va_end(ap);
	return (res);
}
