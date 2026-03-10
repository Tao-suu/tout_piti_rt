# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tbez--du <tbez--du@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/27 17:19:19 by tbez--du          #+#    #+#              #
#    Updated: 2026/03/09 19:25:31 by tbez--du         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC		=	main.c \
			mlx_hook.c \
			mlx_base.c \
			vec_utils.c \
			thread.c \
			random.c
OBJ		=	$(SRC:.c=.o)
DEP		=	$(SRC:.c=.o)

SRC_DIR	=	src/
OBJ_DIR	=	obj/
DEP_DIR	=	dep/

CC		=	clang
#CFLAGS	=	-Wall -Werror -Wextra
CLINKS	=	-Lmlx/ -lmlx -lmlx_Linux -lX11 -lXext -lm -Llibft -lft
CINC	=	-Imlx/ -Iincludes/ -Ilibft

CF		=	$(CC) $(CFLAGS) $(CINC)

NAME	=	rt_poc


all:			$(NAME)

$(NAME):		$(addprefix $(OBJ_DIR), $(OBJ)) libft/libft.a
	$(CF) $(addprefix $(OBJ_DIR), $(OBJ)) $(CLINKS) -o $(NAME)

libft/libft.a:
	$(MAKE) -C libft

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c
	mkdir -p $(DEP_DIR)
	mkdir -p $(OBJ_DIR)
	$(CF) -c $< -o $@ -MMD -MP -MF $(DEP_DIR)$*.d

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C libft clean
	rm -rf $(DEP_DIR)

fclean:	clean
	rm -f $(NAME)
	rm -f libft/libft.a

re:	fclean all

.PHONY: all clean fclean re
