NAME_SERVER = server
NAME_CLIENT = client

CC = clang
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -L libft -lft

LIBPATH = libft
LIB = $(LIBPATH)/libft.a

INCLUDES = includes/

VPATH = srcs/client:srcs/server:srcs/shared_functions
SRCS_SERVER = \
	   s_main.c \
	   s_get.c \
	   s_put.c \
	   s_cd.c \
	   s_ls.c \
	   s_handle_cmds.c \
	   s_pwd.c \
	   s_where.c \
	   rec_data.c \
	   fork_utils.c

SRCS_CLIENT = \
	   c_main.c \
	   c_get.c \
	   c_put.c \
	   c_signals.c \
	   c_handle_cmds.c \
	   c_lls.c \
	   c_lcd.c \
	   c_lpwd.c \
	   c_interface.c \
	   rec_data.c \
	   fork_utils.c

OBJDIR = objs/
OBJS_SERVER = $(patsubst %.c, $(OBJDIR)%.o, $(SRCS_SERVER))
OBJS_CLIENT = $(patsubst %.c, $(OBJDIR)%.o, $(SRCS_CLIENT))

default: all

all: $(OBJDIR) client server

$(LIB):
	@make -C $(LIBPATH)

client: $(LIB) $(OBJS_CLIENT)
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(NAME_CLIENT) $(OBJS_CLIENT) -I $(INCLUDES)
	@echo "\n # ft_p client : Job done $(shell pwd)/$(NAME_CLIENT)"

server: $(LIB) $(OBJS_SERVER)
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(NAME_SERVER) $(OBJS_SERVER) -I $(INCLUDES)
	@echo "\n # ft_p server : Job done $(shell pwd)/$(NAME_SERVER)"

$(OBJDIR)%.o: %.c | $(OBJDIR)
	@echo "\r + ft_p : Compiling $(OBJDIR) < $^\c"
	@$(CC) $(CFLAGS) -o $@ -c $< -I libft/include -I $(INCLUDES)

$(OBJDIR):
	$(shell\
		if [ -d $(OBJDIR) ]; then : ; \
		else\
			mkdir $(OBJDIR); \
		fi)

clean:
	make -C $(LIBPATH) clean
	@echo " $(shell\
		if [ -d $(OBJDIR) ]; then\
			echo "- ft_p : Removing $(OBJDIR) with";\
			ls $(OBJDIR) | wc -w; echo ".o elements inside";\
			rm -Rf $(OBJDIR);\
		else\
			echo "# ft_p : Nothing to clean";\
		fi)"

fclean: clean
	make -C $(LIBPATH) fclean
	@echo " $(shell\
		if [ -f $(NAME_SERVER) ]; then\
			echo "- ft_p server : Removing $ $(NAME_SERVER) ";\
			rm -f $(NAME_SERVER);\
		else\
			echo "# ft_p server : $(NAME_SERVER) not present";\
		fi)"
	@echo " $(shell\
		if [ -f $(NAME_CLIENT) ]; then\
			echo "- ft_p client : Removing $ $(NAME_CLIENT) ";\
			rm -f $(NAME_CLIENT);\
		else\
			echo "# ft_p client : $(NAME_CLIENT) not present";\
		fi)"

re: fclean all

.PHONY: default all client server $(OBJDIR) clean fclean re
