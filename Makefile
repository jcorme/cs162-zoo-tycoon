CC=g++
CXXFLAGS=-Wall -std=c++0x
EXE_FILE=ZooTycoon

objects:=$(patsubst %.cpp,%.o,$(wildcard *.cpp))
objects:=$(filter-out $(EXE_FILE).o,$(objects))

all: $(EXE_FILE)

$(EXE_FILE): $(objects) $(wildcard *.h) $(EXE_FILE).cpp
	$(CC) $(CXXFLAGS) $(EXE_FILE).cpp $(objects) -o $@

$(objects): %.o: %.cpp %.h
	$(CC) -c $(CXXFLAGS) $< -o $@

clean:
	rm -f *.o $(EXE_FILE)
