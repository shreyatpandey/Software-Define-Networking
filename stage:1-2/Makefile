CC=g++
CFLAGS=-c -g -Wall -Iinc
SRCDIR=src
SOURCES=$(wildcard $(SRCDIR)/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=proja

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -Wall $(OBJECTS) -o $@

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

clear:
	rm -f *.out
