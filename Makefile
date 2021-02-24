FLAGS = -g -c
SKIPL = build/linkedlist.o build/skiplist.o
# OBJ = 

build/%.o: src/%.c
	gcc $(FLAGS) $< -o $@

# vaccineMonitor: vaccineMonitor.o $(OBJ)
# 	gcc $@ -o vaccineMonitor.o $(OBJ)

testbuild/%.o: test/%.c
	gcc $(FLAGS) $< -o $@

testSkipLists: testbuild/testSkipLists.o $(SKIPL)
	gcc -o testbuild/testSkipLists $< $(SKIPL)

clean:
	rm -f testbuild/* build/*
