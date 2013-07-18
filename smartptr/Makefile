CC=g++
CFLAGS=-c -std=c++11
LDFLAGS=
SOURCES=main.cpp Foo.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=smartptr

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o $(EXECUTABLE)
