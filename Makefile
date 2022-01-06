NAME			=	ft_containers
CC				=	c++
FLAGS			=	-Wall -Wextra -std=c++98

PATH_SRCS		=	./
SRCS			=	main.cpp

PATH_OBJS		=	./.objs/
OBJS			=	$(SRCS:.cpp=.o)
PREFIX_OBJS		=	$(addprefix $(PATH_OBJS), $(OBJS))

all				:	$(PATH_OBJS) $(NAME)

$(PATH_OBJS)	:
					@mkdir -p $(PATH_OBJS)

$(NAME)			:	$(PREFIX_OBJS)
					@$(CC) -o $@ $(PREFIX_OBJS) $(FLAGS)

$(PATH_OBJS)%.o	:	$(PATH_SRCS)%.cpp
					@echo "Compiling "$<
					@$(CC) -o $@ -c $< $(FLAGS)

clean			:
					@rm -rf $(PATH_OBJS)

fclean			:	clean
					@rm -f $(NAME)

re				:	fclean all

.PHONY			:	all clean fclean re sanit
