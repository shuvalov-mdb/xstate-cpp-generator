CXXFLAGS=-g -std=c++17 -Wall -pedantic

CXX=clang++ $(CXXFLAGS)
#CXX=g++ $(CXXFLAGS)

BASENAME=engineer

all: $(BASENAME)_sm.o $(BASENAME)_demo.o $(BASENAME)_demo $(BASENAME)_test

$(BASENAME)_demo.o:
	$(CXX) -c $(BASENAME)_demo.cpp

$(BASENAME)_sm.o:
	$(CXX) -c generated/$(BASENAME)_sm.cpp

$(BASENAME)_demo:
	$(CXX) -o $(BASENAME)_demo $(BASENAME)_demo.o $(BASENAME)_sm.o -lpthread

$(BASENAME)_test:
	$(CXX) -o $(BASENAME)_test generated/$(BASENAME)_test.cpp $(BASENAME)_demo.o $(BASENAME)_sm.o -lpthread -lgtest

clean:
	rm $(BASENAME)_demo $(BASENAME)_test
