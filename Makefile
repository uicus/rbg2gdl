CXX = g++
EXEC = rbg2gdl
INC = -IrbgParser/src
CXXFLAGS = -Wall -Wextra -Wpedantic -std=c++11 -O3 -flto -s $(INC)
CXXFLAGSDEBUG = -Wall -Wextra -Wpedantic -std=c++11 -g $(INC)
OBJECTS = src/translator.o src/gdl_constants.o src/state.o src/edge.o src/automaton.o src/automaton_builder.o src/pure_moves_printer.o

all: $(OBJECTS)
	cd rbgParser && make lib
	$(CXX) $(CXXFLAGS) -o $(EXEC) src/main.cpp rbgParser/rbgParser.so $(OBJECTS)

debug: $(OBJECTS)
	cd rbgParser && make lib
	$(CXX) $(CXXFLAGSDEBUG) -o $(EXEC) src/main.cpp rbgParser/rbgParser.so $(OBJECTS)

clean:
	rm -f $(OBJECTS)
	cd rbgParser && make clean

distclean: clean
	rm -f $(EXEC)
	cd rbgParser && make distclean
