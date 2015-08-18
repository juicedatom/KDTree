CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lboost_serialization -lcppunit
SOURCES=runUnitTests.cpp unitTests.cpp
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
