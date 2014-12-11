#Compilo y lo paso a elf32
nasm ZOMBIEGUARDIA1.asm -f elf32 -o Zomb1.o
#Lo linkeo y lo paso a binario
ld -s -static -m elf_i386 --oformat binary -b elf32-i386 -e task -Ttext 0x08000000 Zomb1.o -o ZombLinkeado1.o
# armas un archivo de ceros de 4k
dd if=/dev/zero of=ArchivoDe4K1 count=1 bs=4k
# copias el compilado al archivo
dd if=ZombLinkeado1.o of=ArchivoDe4K1 count=1 bs=4k conv=notrunc

#Compilo y lo paso a elf32
nasm ZOMBIEGUARDIA2.asm -f elf32 -o Zomb2.o
#Lo linkeo y lo paso a binario
ld -s -static -m elf_i386 --oformat binary -b elf32-i386 -e task -Ttext 0x08000000 Zomb2.o -o ZombLinkeado2.o
# armas un archivo de ceros de 4k
dd if=/dev/zero of=ArchivoDe4K2 count=1 bs=4k
# copias el compilado al archivo
dd if=ZombLinkeado2.o of=ArchivoDe4K2 count=1 bs=4k conv=notrunc

#Compilo y lo paso a elf32
nasm ZOMBIEGUARDIA3.asm -f elf32 -o Zomb3.o
#Lo linkeo y lo paso a binario
ld -s -static -m elf_i386 --oformat binary -b elf32-i386 -e task -Ttext 0x08000000 Zomb3.o -o ZombLinkeado3.o
# armas un archivo de ceros de 4k
dd if=/dev/zero of=ArchivoDe4K3 count=1 bs=4k
# copias el compilado al archivo
dd if=ZombLinkeado3.o of=ArchivoDe4K3 count=1 bs=4k conv=notrunc

#make

#bochs -q