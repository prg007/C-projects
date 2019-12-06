CXX=g++-8
CXXFLAGS=-Wall -Wextra -pedantic -Werror -std=c++17 -O3
LDFLAGS=$(CXXFLAGS)
OBJ=$(SRC:.cc=.o)

all:  tsp

tsp: tsp.o chromosome.o deme.o cities.o climb_chromosome.o tournament_deme.o
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cc %.hh
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -c -o $@ $<

clean:
	rm -rf *.o tsp
