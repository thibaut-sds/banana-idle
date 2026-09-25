# ===================================
# Raylib and dependencies
# ===================================
# Raylib's folder
RAYLIB_DIR ?= /usr/local
RAYLIB_INCLUDE ?= $(RAYLIB_DIR)
RAYLIB_LIB_PATH ?= $(RAYLIB_DIR)

# ===================================
# cJSON lib
# ===================================
CJSON_DIR ?= /usr

CJSON_INCLUDE ?= $(CJSON_DIR)
CJSON_LIB_PATH ?= $(CJSON_DIR)

# ===================================
# Compilator and options
# ===================================
CC = gcc

# Ajout de -I pour Raylib ET cJSON
CFLAGS = -Iinclude -I$(RAYLIB_INCLUDE) -I$(CJSON_INCLUDE) -Wall -Wextra -std=c11

# ===================================
# Folders
# ===================================
SRC_DIR = src
BUILD_DIR = build
ASSETS_DIR = assets

# ===================================
# Sources and objects
# ===================================
SRCS = main.c $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

# ===================================
# Exec
# ===================================
EXE = $(BUILD_DIR)/banana_idle

# ===================================
# Link flags (Linker)
# ===================================
# System libs
SYS_LIBS = -lGL -lpthread -ldl -lrt -lX11
MATH_LIB = -lm

# Specials libs
RAYLIB_LIBS = -lraylib
CJSON_LIBS = -lcjson

# Construction de la commande de lien :
# On donne les chemins (-L) pour Raylib et cJSON
# On lie Raylib en STATIQUE (incluse dans l'exe)
# On lie le reste (cJSON, GL, Système) en DYNAMIQUE
LDFLAGS = -L$(RAYLIB_LIB_PATH) -L$(CJSON_LIB_PATH) \
          -Wl,-Bstatic $(RAYLIB_LIBS) \
          -Wl,-Bdynamic $(CJSON_LIBS) $(SYS_LIBS) $(MATH_LIB)

# ===================================
# default : full build
# ===================================
all: $(EXE)

# ===================================
# Compilation .o
# ===================================
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ===================================
# Build exec + copy assets
# ===================================
$(EXE): $(OBJS) | $(BUILD_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)
	cp -r $(ASSETS_DIR) $(BUILD_DIR)/

# ===================================
# Create de build directory if it doesn't exist
# ===================================
$(BUILD_DIR):
	mkdir -p $@

# ===================================
# Clean
# ===================================
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
