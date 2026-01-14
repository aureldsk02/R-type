# Nom de l'exécutable final
TARGET = r-type_client

# Compilateur C++
CXX = g++

# Dossiers contenant les sources (.cpp)
SRCDIRS = .

# Dossiers contenant les en-têtes (.hpp)
INCDIRS = . \
          Managers \
          UI/include \
          UI/components/include \
          UI/parsers/include

# Obtention des drapeaux de compilation pour SFML et libconfig
# Note: 'sfml-graphics' inclut déjà 'sfml-window' et 'sfml-system'
SFML_FLAGS = $(shell pkg-config --cflags sfml-graphics)
SFML_LIBS = $(shell pkg-config --libs sfml-graphics)
LIBCONFIG_FLAGS = $(shell pkg-config --cflags libconfig++)
LIBCONFIG_LIBS = $(shell pkg-config --libs libconfig++)

# Drapeaux de compilation
CXXFLAGS = -std=c++20 -g $(SFML_FLAGS) $(LIBCONFIG_FLAGS)

# Drapeaux de liaison (linking)
LDFLAGS = $(SFML_LIBS) $(LIBCONFIG_LIBS)

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
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

# Règle générique pour compiler les fichiers source (.cpp) en objet (.o)
%.o: %.cpp
	@mkdir -p $(dir $@) # Crée le répertoire objet si nécessaire
	@echo "Compiling $<..."
	$(CXX) -c $< $(CXXFLAGS) -o $@

# Règle de nettoyage
clean:
	@echo "--- NETTOYAGE DES FICHIERS OBJETS ---"
	rm -f $(OBJECTS)
	rm -f $(TARGET)

# Règle de re-compilation complète
re: clean all