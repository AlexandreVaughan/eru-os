ERU OS
======

Attempt to create an operating system from scratch following https://hexgeosys.udemy.com/course/developing-a-multithreaded-kernel-from-scratch/

To compile : 
```
make
```

Execute on qemu :
```
qemu-system-x86_64 -hda ./bin/boot.bin
```

Copy on bootable disk:
```
sudo dd if=./bin/boot.bin of=/dev/sdb 
```

