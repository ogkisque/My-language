cd front_end
./front
cd ..

cd middle_end
./middle
cd ..

cd back_end
./back
cd ..

llc llvm.ll
g++ -no-pie llvm.s -o elf
