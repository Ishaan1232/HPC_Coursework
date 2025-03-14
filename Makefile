CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2 -ftree-vectorize
HDRS = parse_cl.h box.h particle.h
OBJS = dynamics.o parse_cl.o box.o particle.o
PAR_OBJS = dynamicsPar.o parse_clPar.o boxPar.o particlePar.o 
TEST_OBJS = test.o box.o particle.o
LDLIBS = -lblas -lboost_program_options -lboost_unit_test_framework
BUILDS = md mdpar unittests

default: md

%.o : %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

md : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

%Par.o : %Par.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -fopenmp -o $@ -c $<

mdpar : $(PAR_OBJS)
	$(CXX) $(CXXFLAGS) -fopenmp -o $@ $^ $(LDLIBS)

unittests: $(TEST_OBJS) 
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

test.o: test.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

doc: Doxyfile
	doxygen $<

Doxyfile:
	doxygen -g $@

.PHONY : clean

clean : 
	-rm -f *.o $(BUILDS) -rf html latex Doxyfile