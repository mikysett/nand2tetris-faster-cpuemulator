SRCS	=	cpuemulator.c \
			hack_compiler.c \
			set_ui.c \
			set_instructions.c \
			init_computer.c \
			run_computer.c \
			ft_split.c \
			bin_to_dec.c \

OBJS	=	$(SRCS:.c=.o)

DEPS	=	cpuemulator.h

NAME	=	CPUEmulator

CC		=	gcc

CFLAGS	=	-Wall -Wextra -Werror

RM		=	rm -f

$(NAME):	$(DEPS) $(OBJS)
			$(CC) $(CFLAGS) -E $(OBJS)
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lncurses
			cd my_assembler && $(MAKE)
			cd my_vm && $(MAKE)

all:		$(NAME)

clean:
			$(RM) $(OBJS)
			cd my_assembler && $(MAKE) clean
			cd my_vm && $(MAKE) clean

fclean:		clean
			$(RM) $(NAME)
			cd my_assembler && $(MAKE) fclean
			cd my_vm && $(MAKE) fclean

re:			fclean all
			cd my_assembler && $(MAKE) re
			cd my_vm && $(MAKE) re

.PHONY:		all clean fclean re