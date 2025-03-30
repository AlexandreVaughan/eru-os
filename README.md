ERU OS
======

Attempt to create an operating system from scratch following https://hexgeosys.udemy.com/course/developing-a-multithreaded-kernel-from-scratch/

Preparation
------------

* Install nasm : `sudo apt install nasm`
* Install qemu : `sudo apt install qemu-system-x86`
* Install cross compiler by following the instructions here : https://wiki.osdev.org/GCC_Cross-Compiler

Compilation
-----------

To compile : 
```
./build.sh
```

To clean :
```
make clean
```

Execution
----------

Execute on qemu :
```
qemu-system-x86_64 -hda ./bin/os.bin
```

Copy on bootable disk:
```
sudo dd if=./bin/os.bin of=/dev/sdb 
```

