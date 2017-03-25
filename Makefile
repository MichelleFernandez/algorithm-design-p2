CXX = g++
OPT = -g -O3

main: max_benefit.cpp
	$(CXX) $(OPT) max_benefit.cpp -o $@
	rm -f $*.c

.PHONY: clean
clean:
	rm -f main *.o *~
