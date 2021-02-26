FLAGS = -g -c
SKIPL = build/linkedlist.o build/skiplist.o
BF = build/bloomfilter.o
# OBJ = 

all: testBloomFilter

build/%.o: src/%.c
	gcc $(FLAGS) $< -o $@

# vaccineMonitor: vaccineMonitor.o $(OBJ)
# 	gcc $@ -o vaccineMonitor.o $(OBJ)

testbuild/%.o: test/%.c
	gcc $(FLAGS) $< -o $@

testSkipLists: testbuild/testSkipLists.o $(SKIPL)
	gcc -o testbuild/testSkipLists $< $(SKIPL)

testBloomFilter: testbuild/testBloomFilter.o $(BF)
	gcc -o testbuild/testBloomFilter $< $(BF) -DK=2

clean:
	rm -f testbuild/* build/*
