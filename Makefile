CXX = g++
CXXFLAGS = -std=c++11 -Wall -g -O0
HDRS = box.h particle.h
OBJS = dynamics.o box.o particle.o
TEST_OBJS = test.o box.o particle.o
LDLIBS = -lblas -lboost_program_options -lboost_unit_test_framework
BUILDS = md unittests

default: md

%.o : %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

md : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

unittests: $(TEST_OBJS) 
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

test.o: test.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

.PHONY : clean

clean : 
	-rm -f *.o $(BUILDS)