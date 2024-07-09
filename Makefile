CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu89

TARGET = server
SRCS = server.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(TARGET)

.PHONY: all clean
