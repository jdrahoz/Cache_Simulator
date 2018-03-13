
# --------------------------------------------------- #
# julia drahozal (2441134)
# eecs 645
# homework 4 : cache simulator
#
# makefile
# compile main executable
# --------------------------------------------------- #

# executable

PROG = cache
$(PROG) :
	g++ -std=c++11 -Wall -g cache.cpp -o $(PROG)

# --------------------------------------------------- #

# test

test:	$(PROG)
	./$(PROG) test/AddressTrace_FirstIndex.bin
	./$(PROG) test/AddressTrace_LastIndex.bin
	./$(PROG) test/AddressTrace_RandomIndex.bin

# clean

clean :
	rm -f $(PROG)
	clear
