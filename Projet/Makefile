CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS_SERVER = -lpthread
LIBS_SH13 = -lpthread -lSDL2 -lSDL2_image -lSDL2_ttf

# Fichiers sources
SRC_SERVER = server.c
SRC_SH13 = sh13.c

# Fichiers objets
OBJ_SERVER = $(SRC_SERVER:.c=.o)
OBJ_SH13 = $(SRC_SH13:.c=.o)

# Exécutables
EXEC_SERVER = server
EXEC_SH13 = sh13

all: $(EXEC_SERVER) $(EXEC_SH13)

# Compilation de server
$(EXEC_SERVER): $(OBJ_SERVER)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS_SERVER)

# Compilation de sh13
$(EXEC_SH13): $(OBJ_SH13)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS_SH13)

# Règle générique pour compiler les fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -f $(OBJ_SERVER) $(OBJ_SH13) $(EXEC_SERVER) $(EXEC_SH13)

.PHONY: all clean