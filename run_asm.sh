cd Front
make
./front.exe ../$1 ../$2
cd ..

cd Backend
make
./back.exe ../$2 ../name_table.txt

nasm -f elf64 listing.s
ld -no-pie listing.o -o first
./first