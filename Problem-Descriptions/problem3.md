# Problem 3: Counting Occurrences of a Number in Files

## Problem Statement

Write a parallel C program to find out the number of occurrences of a certain number in some files.

### Parallelization Scenario

#### Master Process:

1. Create 100 files in the directory given by the user, then add the files' names to an array.
2. Fill those 100 files with 100 random numbers (from 1 to 100).
3. Take an integer number `x` from the user as input.
4. Broadcast the number `x` to all the slave processes.
5. Scatter the array of the files' names among all processes (including the master).
6. If there are any remaining files, the master handles them.
7. Receive the number of occurrences of the number `x` from all processes.
8. Calculate the total number of occurrences of `x` in all the 100 files and print it out.

#### Slave Process:

1. Receive `x` and its portion of the array.
2. Calculate the number of occurrences of `x` in each file in the portion array.
3. Find out the number of occurrences of `x` in each file.
4. Calculate the total number of occurrences of `x` in all the given files.
5. Send the total number of occurrences to the master process.

### Example

**Input:**
```
Enter the directory for the files:
D:\HPC\A2
Enter number x: 3
```

**Output:**

``` 
P0: Total number of occurrences = 6
P1: Total number of occurrences = 4
P2: Total number of occurrences = 2
P3: Total number of occurrences = 11
P4: Total number of occurrences = 5
P5: Total number of occurrences = 1
P6: Total number of occurrences = 9
P7: Total number of occurrences = 3
P8: Total number of occurrences = 0
P9: Total number of occurrences = 1
Total number of occurrences in all 100 files = 42
```
