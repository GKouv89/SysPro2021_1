FLAGS = -g -c
SKIPL = build/linkedlist.o build/skiplist.o
BF = build/bloomfilter.o
LINK = -lm
FILE_PARSING = build/hashmap.o build/bucketlist.o build/citizen.o build/country.o build/virus.o
# OBJ = 

all: vaccineMonitor

tests: testSkipLists testBloomFilter testFileParsing

build/%.o: src/%.c
	gcc $(FLAGS) $< -o $@

vaccineMonitor: build/vaccineMonitor.o
	gcc -o build/$@ $^

testbuild/%.o: test/%.c
	gcc $(FLAGS) $< -o $@

testSkipLists: testbuild/testSkipLists.o $(SKIPL)
	gcc -o testbuild/testSkipLists $< $(SKIPL) $(LINK)

testBloomFilter: testbuild/testBloomFilter.o $(BF)
	gcc -o testbuild/testBloomFilter $< $(BF) -DK=2

testFileParsing: testbuild/testFileParsing.o $(FILE_PARSING) $(BF) $(SKIPL)
	gcc -o testbuild/$@ $^ $(LINK)

clean:
	rm -f testbuild/* build/*
