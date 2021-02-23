FLAGS = -g -c
OBJ = 

build/%.o: src/%.c
  gcc $(FLAGS) $< -o $@

vaccineMonitor: vaccineMonitor.o $(OBJ)
	gcc $@ -o vaccineMonitor.o $(OBJ)
