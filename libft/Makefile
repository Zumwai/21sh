NAME = libft.a

COM = gcc

HEAD = libft.h get_next_line.h ft_printf/ft_printf.h

FLAGS = -Wall -Wextra -Werror -c

SOUS = ft_atoi.c ft_strndup.c ft_bzero.c ft_ischar.c ft_isalnum.c ft_isalpha.c ft_strsplit_free.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_memalloc.c ft_strsplit_len.c ft_memcpy.c ft_memccpy.c ft_memchr.c ft_memcmp.c ft_memdel.c ft_memmove.c ft_memset.c ft_putchar.c ft_putchar_fd.c ft_putendl.c ft_putendl_fd.c ft_putnbr.c ft_putnbr_fd.c ft_putstr.c ft_putstr_fd.c ft_strcat.c ft_strchr.c ft_strclr.c ft_strcmp.c ft_strcpy.c ft_strdel.c ft_strdup.c ft_strequ.c ft_striter.c ft_striteri.c ft_strjoin.c ft_strlcat.c ft_strlen.c ft_strmap.c ft_strmapi.c ft_strncat.c ft_strncmp.c ft_strncpy.c ft_strnequ.c ft_strnew.c ft_strnstr.c ft_strrchr.c ft_strsplit.c ft_strstr.c ft_strsub.c ft_strtrim.c ft_tolower.c ft_toupper.c ft_print_tab.c ft_newdim.c ft_lstnew.c ft_lstdel.c ft_lstdelone.c ft_lstadd.c ft_lstiter.c ft_lstmap.c get_next_line.c ft_isletter.c ft_splitword.c

SOUS_P = ft_printf/additional_next.c ft_printf/additional.c ft_printf/begin.c ft_printf/calc_float_next.c ft_printf/calc_float.c ft_printf/char_next.c \
		ft_printf/char.c ft_printf/count_decimal_next_n.c ft_printf/count_decimal_next.c ft_printf/count_decimal.c ft_printf/ft_printf.c ft_printf/make_flags_2.c \
		ft_printf/make_flags.c ft_printf/o_x_l.c ft_printf/o_x.c ft_printf/print_addres.c ft_printf/print_decimal_next.c ft_printf/print_decimal.c \
		ft_printf/print_float.c ft_printf/printing_nums_next.c ft_printf/printing_nums.c

SOUS_PRINT = ./ft_printf/

OBJS = $(SOUS:.c=.o) $(SOUS_P:.c=.o)

REMOVE = rm -rf


.PHONY: clean fclean re
all: $(NAME)

$(NAME): $(OBJS) $(HEAD)
	ar rc $(NAME) $(OBJS)

%.o: %.c $(HEAD)
	$(CC) $(CFLAGS) -c $<  -o $@

clean:
	$(REMOVE) $(OBJS)

fclean: clean
	$(REMOVE) $(NAME)

re: fclean all