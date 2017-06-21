# HandleMaster

Vulnerable kernel drivers aren't a new thing. They allow regular uses to perform tasks that should be impossible from ring3. More specifically, the [CPU-Z](http://www.cpuid.com/softwares/cpu-z.html) driver allows users to read and write directly to physical memory.

HandleMaster exploits that to perform some [DKOM](https://en.wikipedia.org/wiki/Direct_kernel_object_manipulation) and change granted access rights for handles.

The idea is that you can open a handle with low access and then elevate its access rights later on when you want to use it.

This bypasses some Anti-Cheats that use ObRegisterCallbacks to strip access rights from handles at creation time *cough* BattleEye *cough*

## Processor:

- HandleMaster currently only supports Intel processors due to the way address translation works.

## Supported Windows Versions:

- Windows 7 SP1  (6.1.7601)
- Windows 8      (6.2.9200)
- Windows 8.1    (6.3.9600)
- Windows 10 TH1 (10.0.10240)
- Windows 10 TH2 (10.0.10586)
- Windows 10 Anniversary Update (10.0.14393)
- Windows 10 Creators Update (10.0.15063)

Any problems please let me know :)




