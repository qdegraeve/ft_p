#include "ftp.h"

void	ctrl_c(int signal)
{
	RESIZE
}

void	signal_catcher(void)
{
	signal(SIGWINCH, resize_handler);
	signal(SIGINT, ctrl_c);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTRAP, SIG_IGN);
}
