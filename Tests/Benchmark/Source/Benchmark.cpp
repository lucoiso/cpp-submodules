// Copyright Notices: [...]

#include "Containers.h"
#include "Configuration.h"

BENCHMARK_MAIN();


/*
--------------------------- Last Results ----------------------------
2023-08-20T18:29:38+01:00
Running C:\Users\User\Documents\Projects\QtDataManager\build\Modules\Tests\Benchmark\Release\Benchmark.exe
Run on (12 X 4700 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x6)
  L1 Instruction 32 KiB (x6)
  L2 Unified 1024 KiB (x6)
  L3 Unified 32768 KiB (x1)
------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations
------------------------------------------------------------------------
ContainersEmplaceCopy               1535 ns         1500 ns       448000
ContainersPushCopy                  1530 ns         1538 ns       497778
ContainersEmplaceConstruction       9179 ns         8545 ns        64000
ContainersPushConstruction          9764 ns         9766 ns        64000
ConfigurationInsertion              17.4 ns         16.7 ns     37333333
ConfigurationRemoval                9.67 ns         9.52 ns     64000000
ConfigurationContains               17.6 ns         17.6 ns     37333333
ConfigurationSaveData             151599 ns        40097 ns        28836
ConfigurationLoadData              22320 ns        21972 ns        29867
*/
