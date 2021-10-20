
# University of Tehran, Parallel Programming Course Computer Assignments, Fall 1399(2020)

## CA2
In both questions, serial implementation is used as a baseline for comparison. The parallel approach is implemented using Intel SIMD instructions.

### Q1:
Find the max element of an array.

`Final Speed-Up: 1.23`

### Q2:
Calculate the mean and standard deviation of an array. 

`Final Speed-Up: 2.16`

## CA3
In both questions, serial implementation is used as a baseline for comparison. The parallel approach is implemented using Intel SIMD instructions.

### Q1:
Calculate the absolute difference between two sequential picture frames.

`Final Speed-Up: 5.12`

### Q2:
Merge two pictures to produce a new picture.

`Final Speed-Up: 7.09`

## CA4
In both questions, serial implementation is used as a baseline for comparison. The parallel approach is implemented using the OpenMP library.

### Q1:
Find the max element of an array.

`Final Speed-Up: 2.78`

### Q2:
Sort an array using Quick Sort.

`Final Speed-Up: 4.48`


## CA5
In both questions, serial implementation is used as a baseline for comparison. The parallel approach is implemented using the POSIX.

### Q1:
Find the max element of an array.

`Final Speed-Up: 4.02`
### Q2:
Sort an array using Quick Sort.

`Final Speed-Up: 2.94`


## CA6
The N-Queens problem is solved using backtracking. 

Serial implementation is used as a baseline for comparison. The parallel approach is implemented using the OpenMP library.

Intel Parallel Studio is used to find hotspots in the code, and using inspectors data races are found, and the computation balance between CPU threads is checked.  

`Final Speed-Up: 3.34`