CC=g++
CFLAGS=-c -g -Wall
SOURCES=main.cpp base.cpp primaryrouter.cpp secondaryrouter.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=proja


$(EXECUTABLE):	$(OBJECTS)
			$(CC) -Wall $(OBJECTS) -o $@

.cpp.o:
		$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o

clear:
	rm -f *.out
