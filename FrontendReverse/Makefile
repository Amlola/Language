SANIT_FLAGS = -fsanitize=address,alignment,undefined
DEBUG = -D DUMP

all: MY_REVERSE_FRONT

MY_REVERSE_FRONT: main.o reverse_front.o tree.o name_table.o read.o
	@g++ -g $(DEBUG) $(SANIT_FLAGS) -o reverse.exe obj/main.o obj/reverse_front.o ../Tree/tree.o ../NameTable/name_table.o ../ReadingFile/read.o

main.o: src/main.cpp
	@g++ -g $(DEBUG) $(SANIT_FLAGS) -c src/main.cpp -o obj/main.o

reverse_front.o: src/reverse_front.cpp
	@g++ -g $(DEBUG) $(SANIT_FLAGS) -c src/reverse_front.cpp -o obj/reverse_front.o

tree.o: ../Tree/tree.cpp
	@g++ -g $(DEBUG) $(SANIT_FLAGS) -c ../Tree/tree.cpp -o ../Tree/tree.o

name_table.o: ../NameTable/name_table.cpp
	@g++ -g $(SANIT_FLAGS) -c ../NameTable/name_table.cpp -o ../NameTable/name_table.o

read.o:	../ReadingFile/read.cpp
	@g++ -g $(SANIT_FLAGS) -c ../ReadingFile/read.cpp -o ../ReadingFile/read.o

clean:
	rm obj/* reverse.exe ../Tree/*.o ../NameTable/*.o ../ReadingFile/*.o