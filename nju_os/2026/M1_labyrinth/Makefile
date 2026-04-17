CC = gcc
CFLAGS = -Wall -Wextra -I$(INCDIR) $(DEBUG_FLAGS)
DEBUG_FLAGS = -g

SRCDIR = src
INCDIR = include
BINDIR = bin

TARGET = $(BINDIR)/labyrinth

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:.c=.o)


all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(SRCDIR)/%.o: $(SRCDIR)/%.c $(wildcard $(INCDIR)/*.h)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR):
	mkdir -p $(BINDIR)


clean:
	rm -rf $(BINDIR) $(OBJECTS)


.PHONY: all clean