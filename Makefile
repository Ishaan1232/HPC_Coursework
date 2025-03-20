# Compiler settings
CXX = g++
NVCC = nvcc

# Compilation flags
CXXFLAGS = -std=c++11 -Wall -O2 -ftree-vectorize
NVCCFLAGS = -arch=sm_80 -std=c++11 -O2 

# Header files
HDRS = parse_cl.h box.h particle.h
HDRS_CUDA = parse_cl.h boxCuda.cuh particle.h

# Object files
OBJS = dynamics.o parse_cl.o box.o particle.o
PAR_OBJS = dynamics.o parse_cl.o boxPar.o particlePar.o 
CUDA_OBJS = dynamics.o parse_cl.o boxCuda.o particle.o

TEST_OBJS = test.o box.o particle.o
TEST_PAR_OBJS = testPar.o boxPar.o particlePar.o
TEST_CUDA_OBJS = testCuda.o boxCuda.o particle.o

# Libraries
LDLIBS = -lblas -lboost_program_options -lboost_unit_test_framework

# Build Targets
BUILDS = md mdpar mdcuda unittests unittestsPar unittestsCuda
FILES = html latex Doxyfile

# Default target
default: md

# Compile C++ files
%.o : %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# Build serial version
md : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

# Build parallel OpenMP version
mdpar : CXXFLAGS += -fopenmp
mdpar : $(PAR_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

# Compile CUDA files
%.o : %.cu $(HDRS_CUDA)
	$(NVCC) $(NVCCFLAGS) -o $@ -c $<

# Build CUDA version
mdcuda : $(CUDA_OBJS)
	$(NVCC) $(NVCCFLAGS) -o $@ $^ $(LDLIBS)

# Build unit tests
unittests: $(TEST_OBJS) 
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

unittestsPar : CXXFLAGS += -fopenmp
unittestsPar: $(TEST_PAR_OBJS) 
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

# Documentation
doc: Doxyfile
	doxygen $<

Doxyfile:
	doxygen -g $@

# Clean up
.PHONY : clean
clean : 
	-rm -f *.o $(BUILDS) -rf $(FILES)
