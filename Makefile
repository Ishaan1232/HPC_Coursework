CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2 -ftree-vectorize

HDRS = parse_cl.h box.h particle.h
HDRS_CUDA = parse_cl.h box.cuh particle.h

OBJS = dynamics.o parse_cl.o box.o particle.o
PAR_OBJS = dynamics.o parse_cl.o boxPar.o particlePar.o 
CUDA_OBJS = dynamics.o parse_cl.o boxCuda.o particle.o

TEST_OBJS = test.o box.o particle.o
TEST_PAR_OBJS = testPar.o boxPar.o particlePar.o
TEST_CUDA_OBJS = testCuda.o boxCuda.o particle.o

LDLIBS = -lblas -lboost_program_options -lboost_unit_test_framework
BUILDS = md mdpar mdcuda unittests unittestsPar unittestsCuda
FILES = html latex Doxyfile

default: md

%.o : %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

md : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

unittests: $(TEST_OBJS) 
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)


doc: Doxyfile
	doxygen $<

Doxyfile:
	doxygen -g $@


mdpar : CXX += -fopenmp
mdpar : $(PAR_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

unittestsPar : CXX += -fopenmp
unittestsPar: $(TEST_PAR_OBJS) 
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)


%.o : %.cu $(HDRS_CUDA)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

mdcuda : CXX = nvcc
mdcuda : $(CUDA_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)


.PHONY : clean

clean : 
	-rm -f *.o $(BUILDS) -rf $(FILES)