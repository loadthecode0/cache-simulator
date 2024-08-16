Cache Simulator (to record Hit and Miss times and rates) made as a part of COL216 (Computer Architecture) course at IIT Delhi, Semester 2, 2023-24.

To compile into executable:
`make`

To clean old executables:
`make clean`

To run example tests:
`make test`

To run simulation:
First `make`, then run 
```
./cacheSim <number of sets> <associativity> <block size in bytes> <write-allocate/no-write-allocate> <write-through/write-back> <lru/fifo> < <tracefile>
```

For analysis, edit and run experiments.py and observations.py:
`python3 experiments.py`
`python3 observations.py`

You can change your interpreter to `python` if applicable.

Notes: The report outlines the explanations for the observed trends and aims to determine the best configuration for the trace.
