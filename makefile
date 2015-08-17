CC=g++-4.8
CFLAGS=-c -Wall
LDFLAGS=-lboost_serialization -lcppunit
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
	rm *.o
	rm $(EXECUTABLE)
