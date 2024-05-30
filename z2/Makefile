# Пути к исходным файлам и объектным файлам
CC = gcc
CFLAGS = -Wall -c -g

PROGRAMS = client server

LIBRARY = -lncurses -lrt -pthread

# Создание правил для компиляции каждой программы
all: $(PROGRAMS)

CLIENT_SRCDIR = create_client
CLIENT_OBJDIR = object_client
CLIENT_SOURCES = $(wildcard $(CLIENT_SRCDIR)/*.c)
CLIENT_OBJECTS = $(patsubst $(CLIENT_SRCDIR)/%.c,$(CLIENT_OBJDIR)/%.o,$(CLIENT_SOURCES))

SERVER_SRCDIR = create_server
SERVER_OBJDIR = object_server
SERVER_SOURCES = $(wildcard $(SERVER_SRCDIR)/*.c)
SERVER_OBJECTS = $(patsubst $(SERVER_SRCDIR)/%.c,$(SERVER_OBJDIR)/%.o,$(SERVER_SOURCES))

$(CLIENT_OBJDIR)/%.o: $(CLIENT_SRCDIR)/%.c
	@mkdir -p $(CLIENT_OBJDIR)
	$(CC) $(CFLAGS) $< -o $@

$(SERVER_OBJDIR)/%.o: $(SERVER_SRCDIR)/%.c
	@mkdir -p $(SERVER_OBJDIR)
	$(CC) $(CFLAGS) $< -o $@

client: $(CLIENT_OBJECTS)
	$(CC) $^ -o $@ $(LIBRARY)

server: $(SERVER_OBJECTS)
	$(CC) $^ -o $@ $(LIBRARY)

clean:
	rm -f $(CLIENT_OBJECTS) $(SERVER_OBJECTS)
	rm -f $(PROGRAMS)
	rm -rf $(CLIENT_OBJDIR) $(SERVER_OBJDIR)

.PHONY: all clean
