# ---- NAME ---- #

NAME = IrcBot

# ---- COMPLILATION ---- #

CPP = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

# ---- SRC / OBJS ---- #

SRC =	Bot.cpp \
		TreatingMessage.cpp \
		Help.cpp \
		Commande.cpp
#
OBJS = $(SRC:.cpp=.o)

# ---- COMPILATION ---- #

all : $(NAME)

$(NAME) : $(OBJS)
	@$(CPP) $(CXXFLAGS) $(OBJS) -o $(NAME) \

# ---- CLEAN ---- #

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

# ---- RE ---- #

re : fclean all