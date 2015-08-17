CC=g++-4.9
CFLAGS=-c -Wall
LDFLAGS=-lboost_serialization
SOURCES=use_tree.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=TreeTest
AFTERFLAGS=-std=c++14
all: $(SOURCES) $(EXECUTABLE)
		
$(EXECUTABLE): $(OBJECTS) 
		$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
		$(CC) $(CFLAGS) $< -o $@ $(AFTERFLAGS)
clean:
	[ -f *.o ] && rm *.o
	[ -f $(EXECUTABLE) ] && rm $(EXECUTABLE)
