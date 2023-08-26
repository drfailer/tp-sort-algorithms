CC = g++
CFLAGS = -Wall -Wextra -Wuninitialized -g
LDFLAGS =

sort: sort.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
