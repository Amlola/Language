cd Front
make
./front.exe ../input.txt ../output.txt
cd ..

cd Backend
make
./back.exe ../output.txt ../name_table.txt

nasm -f elf64 listing.s
ld -no-pie listing.o -o main
./main