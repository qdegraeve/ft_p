 # **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: afillion <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/11/30 15:37:28 by afillion          #+#    #+#              #
#    Updated: 2017/11/23 18:19:27 by qdegraev         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

VPATH = srcs:srcs/libft:srcs/ft_printf:srcs/ft_ls

INCLUDES = include/

CC = clang

CFLAGS = -g -Wall -Wextra -Werror

SRCS =  ft_strlcat.c \
	ft_lstiter.c \
	ft_memalloc.c \
	ft_lstmap.c \
	ft_lstadd_back.c \
	ft_putendl_fd.c \
	ft_atoi.c \
	ft_itoa.c \
	ft_putnbr_fd.c \
	ft_strjoin.c \
	ft_memdel.c \
	ft_putstr_fd.c \
	ft_strclr.c \
	ft_memmove.c \
	ft_memset.c \
	ft_strncpy.c \
	ft_strsplit.c \
	ft_putnbr.c \
	ft_toupper.c \
	ft_memchr.c \
	ft_strtrim.c \
	ft_isalnum.c \
	ft_putstr.c \
	ft_isascii.c \
	ft_lstdelone.c \
	ft_strdel.c \
	ft_isalpha.c \
	ft_isprint.c \
	ft_strsub.c \
	ft_striteri.c \
	ft_striter.c \
	ft_lstadd.c \
	ft_memcpy.c \
	ft_strchr.c \
	ft_putbase.c \
	ft_bzero.c \
	ft_strncmp.c \
	ft_strcmp.c \
	ft_strmap.c \
	ft_strnequ.c \
	ft_putchar.c \
	ft_memccpy.c \
	ft_iswhitespace.c \
	ft_strrchr.c \
	ft_strstr.c \
	ft_strequ.c \
	ft_putchar_fd.c \
	ft_isdigit.c \
	ft_memcmp.c \
	ft_strmapi.c \
	ft_strncat.c \
	ft_tolower.c \
	ft_lstdel.c \
	ft_strdup.c \
	ft_strlen.c \
	ft_strnstr.c \
	ft_putendl.c \
	ft_lstnew.c \
	ft_strcat.c \
	ft_strnew.c \
	ft_strcpy.c \
	get_next_line.c \
	ft_getnbr.c \
	ft_abs.c \
	ft_itoa_base.c \
	ft_cleanjoin.c \
	ft_printf.c \
	types.c \
	arg_length.c \
	flags.c \
	ft_putnbru.c \
	ft_utoa_base.c \
	type_d_i.c \
	type_s.c \
	type_x.c \
	type_percent.c \
	type_f.c \
	type_c.c \
	type_u.c \
	type_o.c \
	type_p.c \
	ft_wchar.c \
	ft_tablen.c \
	ft_tabdel.c \
	ft_concat.c

OBJDIR = objs/
OBJS = $(patsubst %.c, $(OBJDIR)%.o, $(SRCS))

all: $(NAME)
	@echo " # libft : Job done $(shell pwd)/$(NAME)"

$(NAME): $(OBJS)
	@ar rc $(NAME) $(OBJS)
	@ranlib $(NAME)
	@echo "\n + libft : Creating Library $(shell pwd)/$(NAME)"

$(OBJDIR)%.o: %.c $(OBJDIR)
	@echo  "\r + libft : Compiling $(OBJDIR) < $^\c"
	@$(CC) $(CFLAGS) -o $@ -c $(SOURCES)$< -I $(INCLUDES)

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	@rm -f $(OBJ)
	@echo " $(shell\
		if [ -d $(OBJDIR) ]; then\
			echo "- libft : Removing $(OBJDIR) with";\
			ls $(OBJDIR) | wc -w; echo ".o elements inside";\
			rm -Rf $(OBJDIR);\
		else\
			echo "# libft : Nothing to clean";\
		fi)"

fclean: clean
	@rm -f $(NAME)
	@echo " $(shell\
		if [ -f $(NAME) ]; then\
			echo "- libft : Removing $ $(NAME) ";\
			rm -f $(NAME);\
		else\
			echo "# libft : Nothing to fclean";\
		fi)"

re: fclean all
