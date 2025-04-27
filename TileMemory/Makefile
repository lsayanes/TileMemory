CXX = clang++
CXXFLAGS = -std=c++23 -Wall -pedantic -I/opt/homebrew/include -I./ecs -I./formats -I./system
LDFLAGS = -L/opt/homebrew/lib -lSDL3 -lSDL3_image
SOURCES = MemoryApp.cpp ./system/Render.cpp Game.cpp ./ecs/EntityManager.cpp
HEADERS = Render.h Game.h Tiles.h  Entity.h EntityManger.h Image.h
OUTPUT = ./output/TileMemory

OBJS = $(SOURCES:.cpp=.o)

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OUTPUT)