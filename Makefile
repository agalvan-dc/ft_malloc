# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agalvan- <agalvan-@student.42madrid.com>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/09/22 12:17:16 by agalvan-          #+#    #+#              #
#    Updated: 2026/09/22 15:45:07 by agalvan-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so

CC = cc

CFLAGS = -Wall -Wextra -Werror -O2 -fno-strict-aliasing -Wno-array-bounds -Wno-maybe-uninitialized -fPIC -g -pthread -ftls-model=initial-exec

INCLUDES	= -I. -I./src/libft

LIBFT_DIR	= src/libft
LIBFT		= $(LIBFT_DIR)/libft.a

SRC			= ft_malloc.c \
			  ft_free.c \
			  ft_realloc.c \
			  ft_calloc.c \
			  ft_show_alloc_mem.c \
			  src/alloc_core.c \
			  src/arena.c \
			  src/arena_ops.c \
			  src/chunk.c \
			  src/coalesce.c \
			  src/debug.c \
			  src/free_utils.c \
			  src/large.c \
			  src/malloc_arena.c \
			  src/tcache.c \
			  src/utils.c \

OBJS		= $(SRC:.c=.o)

MAKEFLAGS += -s

all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -s -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -shared $(OBJS) $(LIBFT) -o $(NAME)
	@ln -sf $(NAME) libft_malloc.so
	@printf "\033[0;32m[BUILD OK] $(NAME) compiled successfully\033[0m\n"

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean: 
	@rm -rf $(OBJS)
	@$(MAKE) -s clean -C $(LIBFT_DIR) 2>/dev/null || true

fclean: clean
	@rm -rf libft_malloc_*.so libft_malloc.so
	@$(MAKE) -s  fclean -C $(LIBFT_DIR) 2>/dev/null || true

re: fclean all

.PHONY: all clean fclean re 


