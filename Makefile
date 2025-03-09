CC = g++
CXXFLAGS = -std=c++11 -Wall
HDRS = box.h particle.h
OBJS = dynamics.o box.o particle.o
LDLIBS = -lblas -lboost_program_options

default: md

%.o : %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

md : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

.PHONY : clean

clean : 
	-rm -f *.o md