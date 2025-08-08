CXXFLAGS=-DPLATFORM_OSX -I/opt/homebrew/include -Iinclude
LDFLAGS=-L/opt/homebrew/lib -lglfw -lGLEW -framework OpenGL

# List of source files
SOURCES = src/main.cpp src/geometry.cpp src/shaders.cpp src/input.cpp src/game.cpp src/arm.cpp src/camera.cpp src/loadObj.cpp

main: $(SOURCES)
	g++ $(CXXFLAGS) $(SOURCES) -o $@ $(LDFLAGS)


