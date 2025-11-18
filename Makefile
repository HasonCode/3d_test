CPPFLAGS = $(shell sdl2-config --cflags) $(shell $(PKG_CONFIG) SDL2_image --cflags) $(EXTRA_CPPFLAGS) 
LDLIBS = $(shell sdl2-config --libs) $(shell $(PKG_CONFIG) SDL2_image --libs) -lGLEW $(EXTRA_LDLIBS)
EXTRA_LDLIBS ?= -lGL
PKG_CONFIG ?= pkg-config
all: main
main: shader_utils.o Shader.o Texture3D.o CubeRenderer.o main.o resourcemanager.o SpriteRenderer.o stb.o camera.o Texture2D.o AABB.o frustrum.o
	g++ -o main shader_utils.o Shader.o Texture3D.o CubeRenderer.o main.o resourcemanager.o SpriteRenderer.o stb.o camera.o Texture2D.o AABB.o frustrum.o $(LDLIBS)
clean:
	rm -f *.o triangle cube sphere
.PHONY: all clean