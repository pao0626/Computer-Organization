1. make score 跑出 config檔案結果
2. ./bruteforce.sh 跑所有可能解的組合（set/way/blocksize），需依情況改動程式內policy
3. 可用以下三個指令取代make score，詳細印出結果，以便debug
* make $policy
* riscv64-unknown-elf-gcc -static -o qr ./benchmark/qrcode.c
* spike --isa=RV64GC --dc=2:4:8 /home/ubuntu/riscv/riscv64-unknown-elf/bin/pk qr