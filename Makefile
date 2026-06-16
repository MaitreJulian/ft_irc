NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC_DIRS = src/TCP src/Network src/IRC

SRCS = $(shell find $(SRC_DIRS) -type f -name "*.cpp")

OBJ_DIR = obj
OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS))

INCLUDES = -I include

RM = rm -f
RMDIR = rm -rf

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RMDIR) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re