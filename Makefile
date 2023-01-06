#Makefile for ft_containers
CXX = c++

#WFLAGS = -Wall -Wextra -Werror
STDFLAGS = -std=c++98 -ferror-limit=50
DEBUGFLAGS = -g3 -fsanitize=address
LEAKSFLAGS = -g3

ifdef DEBUG
	CXXFLAGS = $(STDFLAGS) $(DEBUGFLAGS)
else ifdef LEAKS
	CXXFLAGS = $(STDFLAGS) $(LEAKSFLAGS)
else
	CXXFLAGS = $(WFLAGS) $(STDFLAGS)
endif

NAME = ft_containers

INCS_DIR = ./includes/
SRCS_DIR = ./tests/

SRCS = $(addprefix $(SRCS_DIR), $(SRCS_FILES))
OBJS = $(SRCS:.cpp=.o)

# wild card
SRCS_FILES = main.cpp\
vector_test.cpp \
vector_iterator_test.cpp \
type_traits_test.cpp \
pair_test.cpp

OBJS_FILES = $(OBJS)

COMPILE_MSG	= @echo $(BOLD)$(L_PURPLE) 📣 ${NAME} Compiled 🥳$(RESET)


.PHONY : all
all : $(NAME) $(OBJS)

$(NAME) : $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $@

%.o : %.cpp
	@$(CXX) $(CXXFLAGS) -I$(INCS_DIR) -c $< -o $@
	@echo $(BOLD)$(L_GREEN) Compiling with $(CXXFLAGS) -I$(INCS_DIR) ...$(RESET)
	@echo $(GREEN) [$(notdir $^)] to [$(notdir $@)] $(RESET)

.PHONY : clean
clean :
	@rm -f $(OBJS)
	@echo $(BOLD)$(L_RED) 🗑️ Removed object files 📁$(RESET)

.PHONY : fclean
fclean : clean
	@rm -f $(NAME)
	@echo $(BOLD)$(L_PURPLE) 🗑️ Removed $(NAME) 📚$(RESET)

.PHONY : re
re : fclean
	@make all

# 이게 최선입니까? 확실해요?
.PHONY : debug
debug : fclean
	@make DEBUG=1


.PHONY : leaks
leaks : fclean
	@make LEAKS=1


######################### Color #########################
GREEN="\033[32m"
L_GREEN="\033[1;32m"
RED="\033[31m"
L_RED="\033[1;31m"
RESET="\033[0m"
BOLD="\033[1m"
L_PURPLE="\033[1;35m"
L_CYAN="\033[1;96m"
UP = "\033[A"
CUT = "\033[K"
