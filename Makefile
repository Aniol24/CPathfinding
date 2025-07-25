CC=gcc
EMCC=emcc
SRC=main.c
OUT=pathfinding

# Rutas a raylib para WASM
RAYLIB_WEB_DIR=../raylib/build-web/raylib
RAYLIB_SRC_DIR=../raylib/src

all:
	$(CC) $(SRC) -o $(OUT) -lraylib -lm -ldl -lpthread -lGL -lrt -lX11

wasm:
	$(EMCC) $(SRC) -o index.html \
	  -I$(RAYLIB_SRC_DIR) \
	  -L$(RAYLIB_WEB_DIR) -lraylib \
	  -s USE_GLFW=3 -s ASYNCIFY \
	  -s TOTAL_MEMORY=64MB \
	  -s ALLOW_MEMORY_GROWTH=1 \
	  -s ENVIRONMENT=web \
	  -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
	  -O2

clean:
	rm -f $(OUT) index.html index.js index.wasm
