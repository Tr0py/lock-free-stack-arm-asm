# Lock-free stack

QEMU translates <a href = https://en.wikipedia.org/wiki/Load-link/store-conditional>Load-Link/Store-Conditional(LL/SC)</a> to test-test-and-set, resulting in potential <a href = https://en.wikipedia.org/wiki/ABA_problem>ABA problem</a>.

This naive lock-free stack is written in **arm** assembly to provide api that can be used in C/C++.

### What is the output log

* 'begin' specifies the beginning of a Load-Link operation. There is likely more than one 'begin' in one push/pop operation as Store-Conditional fails, which causes a retry.
* If a 'suc' is followed after 'begin', the Store-Conditional op succeeds. And according to LL/SC's sematics, non push/pop op should succeed, which equals to a LL/SC success, during this period.
* Thus, if there is another 'suc' in one LL/SC peroid(from last 'begin' to 'suc'), there comes an ABA problem.


### Build

```
make
```
### Run

```
qemu-arm ./main 
or
qemu-arm ./main > log
or 
./main
```

### TODO

A script to analyze log and calculate ABA problem occurance rate.

### Related

Section 4.3 of <a href = http://www.cs.columbia.edu/~luca/research/cota_CGO17.pdf>Cross-ISA Machine Emulation for Multicores</a>

Atomics section of <a href = https://lwn.net/Articles/697265/>Multi-threaded emulation for QEMU</a>
