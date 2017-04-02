CXX = g++ 
OPT = -g -O3 -std=c++11

branch_and_bound: branch_and_bound.cpp
	$(CXX) $(OPT) branch_and_bound.cpp -o $@
	rm -f $*.c

.PHONY: clean
clean:
	rm -f main *.o *~
