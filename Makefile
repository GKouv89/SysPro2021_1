FLAGS = -g -c
SKIPL = src/linkedlist.o src/skiplist.o
OBJ = 

build/%.o: src/%.c
	gcc $(FLAGS) $< -o $@

vaccineMonitor: vaccineMonitor.o $(OBJ)
	gcc $@ -o vaccineMonitor.o $(OBJ)

testbuild/%.o: test/%.c
	gcc $(FLAGS) $< -o $@

testSkipLists: testbuild/testSkipLists.o $(SKIPL)
	gcc -o testbuild/testSkipLists $< $(SKIPL)
