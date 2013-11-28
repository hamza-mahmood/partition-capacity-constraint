# the compiler
CC = g++

#DEBUG = -g
DEBUG =
# Flags for linking
LFLAGS = -Wall -O2 -std=c++11 $(DEBUG)
# Flags for compiling
CFLAGS = -Wall -O2 -std=c++11 -c $(DEBUG)

SRC= src
# sources are all the .cpp files
SOURCES = $(shell ls $(SRC)/*.cpp)
# objects are the respective .o files
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = test

# make the executable
all: $(EXECUTABLE)

# make executable from objects
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $(EXECUTABLE)

# generate each .o from each .cpp 
# $@: target (here a .o)
# $<: first dependency (here, only one, a .cpp)
%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -fr $(SRC)/*.o $(SRC)/*~

mrproper:
	rm -fr $(SRC)/*.o $(SRC)/*~ $(EXECUTABLE)
