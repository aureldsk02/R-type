# Nom de l'exécutable final
TARGET = r-type_server

# Compilateur C++
CXX = g++

# Dossiers contenant les sources (.cpp)
SRCDIRS = .

# Dossiers contenant les en-têtes (.hpp)
INCDIRS = . \
          Managers \
          Auth/include \
          Network/include 

# Obtention des drapeaux de compilation pour SFML et libconfig
# Note: 'sfml-graphics' inclut déjà 'sfml-window' et 'sfml-system'
#LIBSODIUM_FLAGS = -lsodium
#LIBSODIUM_LIBS = $(shell pkg-config --libs sodium)
#SQLITE_FLAGS = -lsqlite3
#SQLITE_LIBS = $(shell pkg-config --libs sqlite3)

# Drapeaux de compilation
CXXFLAGS = -std=c++20 -g $(LIBSODIUM_FLAGS) $(SQLITE_FLAGS)

# Drapeaux de liaison (linking)
#LDFLAGS = $(LIBSODIUM_LIBS) $(SQLITE_LIBS)

# Création de la liste de tous les fichiers sources (.cpp)
# La commande 'find' est utilisée pour parcourir récursivement les dossiers
SOURCES = $(shell find $(SRCDIRS) -name "*.cpp")

# Conversion de la liste des sources (.cpp) en fichiers objets (.o)
OBJECTS = $(SOURCES:.cpp=.o)

# Ajout des chemins d'inclusion (-I) aux CXXFLAGS
INC = $(patsubst %, -I%, $(INCDIRS))
CXXFLAGS += $(INC)

.PHONY: all clean re

# Règle par défaut : construire l'exécutable
all: $(TARGET)

# Règle pour lier les objets et créer l'exécutable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -lsodium -lsqlite3 -o $@

# Règle générique pour compiler les fichiers source (.cpp) en objet (.o)
%.o: %.cpp
	@mkdir -p $(dir $@) # Crée le répertoire objet si nécessaire
	@echo "Compiling $<..."
	$(CXX) -c $< $(CXXFLAGS) -o $@

# Règle de nettoyage
clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)

# Règle de re-compilation complète
re: clean all