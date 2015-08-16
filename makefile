CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lboost_serialization
SOURCES=use_tree.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=TreeTest
AFTERFLAGS=-std=c++11
all: $(SOURCES) $(EXECUTABLE)
		
$(EXECUTABLE): $(OBJECTS) 
		$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
		$(CC) $(CFLAGS) $< -o $@ $(AFTERFLAGS)
clean:
	rm *.o $(EXECUTABLE)
