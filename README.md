## Environmental Setup
Install VirtualBox & Import the prebuilt virtual machine image.

## Hw1
write inline RISC-V assembly code to implement `a = a * b`.
<br>
write inline RISC-V assembly code to implement arrays multiplication.
<br>
write inline RISC-V assembly code to implement matrix multiplication.

## Hw2
Code with the following structure.
```
Hw2/
  |── answer.h
  ├── q1_baseline.c
  ├── q1.c
  ├── q1.h
  ├── q1_improved.c
  ├── q1_input.txt
  ├── q2_ans.c
  ├── q2.c
  ├── q2.h
  ├── q2_input.txt
  ├── makefile
  └── test
```

1. q1_baseline.c: write assembly code base on q1.c and count the number of executes instructions.
2. q1_improved.c: try to speed up q1_baseline.c.
3. q2_ans.c: write assembly code base on q2.c (nestes for-loop code) and improve CPU execution time.

## Hw3
Code with the following structure.
```
Hw3/
  └── benchmark/
      ├── captcha.c
      ├── nyancat.c
      └── qrcode.c
  ├── config.conf
  ├── FIFO_cachesim.h
  ├── FIFO_cachesim.cc
  ├── LRU_cachesim.h
  ├── LRU_cachesim.cc
  ├── LFU_cachesim.h
  ├── LFU_cachesim.cc
  ├── ORIG_cachesim.cc
  ├── ORIG_cachesim.h
  ├── SELF_cachesim.cc
  ├── SELF_cachesim.h
  ├── makefile
  └── test.py
```

Implemented three Cache Replacement Policies.
* First In First Out (FIFO)
* Least Recently Used (LRU)
* Least-Frequently Used (LFU)

The workflow of the data cache simulation in Spike is highlighted as follows.
<br>
![workflow](hw3/workflow.png)
<br>
Develop own cache replacement policies SELF.

###How to test
1. `make score`runs out the **config** file results
2. `./bruteforce.sh` runs all possible solution combinations (set/way/blocksize). You need to change the policy in the program according to the situation.
3. The following three commands can be used to replace 'make score' and print out the results in detail for debugging.
* `make $policy`
* `riscv64-unknown-elf-gcc -static -o qr ./benchmark/qrcode.c`
* `spike --isa=RV64GC --dc=2:4:8 /home/ubuntu/riscv/riscv64-unknown-elf/bin/pk qr`
