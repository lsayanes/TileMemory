CXX = clang++
CXXFLAGS = -std=c++23 -Wall -pedantic -I/opt/homebrew/include -I./ecs -I./formats
LDFLAGS = -L/opt/homebrew/lib -lSDL2 -lSDL2_image
SOURCES = MemoryApp.cpp Render.cpp
HEADERS = Render.h Entity.h Image.h
OUTPUT = TileMemory

OBJS = $(SOURCES:.cpp=.o)

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OUTPUT)