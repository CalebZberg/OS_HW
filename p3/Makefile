CC = gcc
CFLAGS = -pthread -Wall -Wextra -O2
TARGET = p3

all: $(TARGET)

$(TARGET): p3.c
	$(CC) $(CFLAGS) -o $(TARGET) p3.c

clean:
	rm -f $(TARGET)
