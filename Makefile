CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2 -ftree-vectorize
CXXFLAGSPAR = $(CXXFLAGS) -fopenmp 
HDRS = parse_cl.h box.h particle.h

OBJS = dynamics.o parse_cl.o box.o particle.o
PAR_OBJS = dynamicsPar.o parse_clPar.o boxPar.o particlePar.o 

TEST_OBJS = test.o box.o particle.o
TEST_PAR_OBJS = testPar.o boxPar.o particlePar.o

LDLIBS = -lblas -lboost_program_options -lboost_unit_test_framework
BUILDS = md mdpar unittests unittestsPar

default: md

%.o : %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

md : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

test.o: test.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

unittests: $(TEST_OBJS) 
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

%Par.o : %Par.cpp $(HDRS)
	$(CXX) $(CXXFLAGSPAR) -o $@ -c $<

mdpar : $(PAR_OBJS)
	$(CXX) $(CXXFLAGSPAR) -o $@ $^ $(LDLIBS)

testPar.o: testPar.cpp
	$(CXX) $(CXXFLAGSPAR) -o $@ -c $<

unittestsPar: $(TEST_PAR_OBJS) 
	$(CXX) $(CXXFLAGSPAR) -o $@ $^ $(LDLIBS)

doc: Doxyfile
	doxygen $<

Doxyfile:
	doxygen -g $@

.PHONY : clean

clean : 
	-rm -f *.o $(BUILDS) -rf html latex Doxyfile