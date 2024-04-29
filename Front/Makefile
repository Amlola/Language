SANIT_FLAGS = -fsanitize=address,alignment,undefined
OPT_FLAG = -O2
DEBUG = -D DUMP

all: MYFRONT

MYFRONT: main.o tree.o LexemAnalys.o list.o FrontEnd.o name_table.o read.o
	@g++ -g $(DEBUG) $(SANIT_FLAGS) -o front.exe obj/main.o ../Tree/tree.o obj/LexemAnalys.o ../List/list.o obj/FrontEnd.o ../NameTable/name_table.o ../ReadingFile/read.o

main.o: src/main.cpp
	@g++ -g $(DEBUG) $(SANIT_FLAGS) -c src/main.cpp -o obj/main.o

tree.o: ../Tree/tree.cpp
	@g++ -g $(DEBUG) $(SANIT_FLAGS) -c ../Tree/tree.cpp -o ../Tree/tree.o

LexemAnalys.o: src/LexemAnalys.cpp
	@g++ -g $(DEBUG) $(SANIT_FLAGS) -c src/LexemAnalys.cpp -o obj/LexemAnalys.o

list.o: ../List/list.cpp
	@g++ -g $(DEBUG) $(SANIT_FLAGS) -c ../List/list.cpp -o ../List/list.o

FrontEnd.o: src/FrontEnd.cpp
	@g++ -g $(DEBUG) $(SANIT_FLAGS) -c src/FrontEnd.cpp -o obj/FrontEnd.o

name_table.o: ../NameTable/name_table.cpp
	@g++ -g $(SANIT_FLAGS) -c ../NameTable/name_table.cpp -o ../NameTable/name_table.o

read.o:	../ReadingFile/read.cpp
	@g++ -g $(SANIT_FLAGS) -c ../ReadingFile/read.cpp -o ../ReadingFile/read.o

clean:
	rm obj/* front.exe ../List/*.o ../Tree/*.o ../NameTable/*.o