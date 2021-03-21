FLAGS = -g -c
SKIPL = build/linkedlist.o build/skiplist.o
BF = build/bloomfilter.o
LINK = -lm
FILE_PARSING = build/hashmap.o build/bucketlist.o build/citizen.o build/country.o build/virus.o
DATE_OPS = build/dateOps.o
OBJ = build/inputParsing.o build/commands.o $(FILE_PARSING) $(DATE_OPS) $(SKIPL) $(BF)

all: vaccineMonitor

build/%.o: src/%.c
	gcc $(FLAGS) $< -o $@

vaccineMonitor: build/vaccineMonitor.o $(OBJ)
	gcc -o $@ $^ $(LINK)

clean:
	rm -f vaccineMonitor build/*
