TARGET = tipe
CC = gcc

# Flags de compilation
CFLAGS = -Wall -Wextra -I/opt/homebrew/include/igraph
# Flags de l'éditeur de liens
LDFLAGS = -L/opt/homebrew/lib -ligraph

# Fichiers source et objets
SRC = main.c csv.c simulation.c graphe.c stations.c kmedian.c
OBJ = $(SRC:.c=.o)

# Règle principale
all: $(TARGET)

# Création de l'exécutable
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compilation des fichiers .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	rm -f $(OBJ) $(TARGET)
	rm -f *.dot *.png
	clear

# Recompilation complète
rebuild: clean all