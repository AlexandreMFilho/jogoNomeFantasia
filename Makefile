# Compiler
CC := gcc

# Compiler flags
CFLAGS := -Wall -Wextra -pedantic

# SDL library flags
SDL_LIBS := `sdl2-config --cflags --libs`

# Source files
SRCS := main.c

# Object files
OBJS := $(SRCS:.c=.o)

# Executable name
TARGET := jogoNomeFantasia.out

# Build target
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(SDL_LIBS) -o $(TARGET) 

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) $(SDL_LIBS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)
