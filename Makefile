CXX = g++ 
OPT = -g -O3 -std=c++11

branch_and_bound: branch_and_bound.cpp
	$(CXX) $(OPT) branch_and_bound.cpp -o $@
	rm -f $*.c

max_benefit: max_benefit.cpp
	$(CXX) $(OPT) max_benefit.cpp -o $@
	rm -f $*.c

.PHONY: clean

clean:
	rm -f branch_and_bound *.o *~
