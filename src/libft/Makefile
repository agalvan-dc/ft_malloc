# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agalvan- <agalvan-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/19 15:46:47 by caperale          #+#    #+#              #
#    Updated: 2026/06/09 17:11:26 by agalvan-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

CFLAGS = -Wall -Wextra -Werror

AR = ar

ARFLAGS = rcs

RM = rm -f

NAME = libft.a

SOURCES =  \
	ft_isalpha.c \
	ft_isdigit.c \
	ft_isalnum.c \
	ft_isascii.c \
	ft_isprint.c \
	ft_strlen.c \
	ft_memset.c \
	ft_bzero.c \
	ft_memcpy.c \
	ft_memmove.c \
	ft_strlcpy.c \
	ft_strlcat.c \
	ft_tolower.c \
	ft_toupper.c \
	ft_strchr.c \
	ft_strrchr.c \
	ft_strncmp.c \
	ft_memchr.c \
	ft_memcmp.c \
	ft_strnstr.c \
	ft_atoi.c \
	ft_calloc.c \
	ft_strdup.c \
	ft_substr.c \
	ft_strjoin.c \
	ft_itoa.c \
	ft_putchar_fd.c \
	ft_putendl_fd.c \
	ft_putstr_fd.c \
	ft_putnbr_fd.c \
	ft_strmapi.c \
	ft_striteri.c \
	ft_strtrim.c \
	ft_split.c \
	ft_lstnew.c \
	ft_lstadd_front.c \
	ft_lstsize.c \
	ft_lstadd_back.c \
	ft_lstdelone.c \
	ft_lstclear.c \
	ft_lstiter.c \
	ft_lstmap.c \
	ft_printf_utils.c \
	ft_printf.c \
	ft_sqrt.c \
	ft_strjoin_and_split.c \
	ft_atol.c \
	ft_atof.c \
	ft_free_all.c \
	get_next_line.c \
	get_next_line_utils.c \
	


OBJECTS = $(SOURCES:.c=.o)

# RULES
all: $(NAME)

$(NAME): $(OBJECTS)
	$(AR) $(ARFLAGS) $(NAME) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
