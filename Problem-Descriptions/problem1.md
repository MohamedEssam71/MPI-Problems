# Problem 1: Counting Prime Numbers

## Problem Statement

Write a parallel C program for "Counting Primes" using the following two methods: `MPI_Send` and `MPI_Receive` ONLY.

### Input

- Lower bound number `x`
- Upper bound number `y`

### Output

- Count of prime numbers occurring between `x` and `y`
- Count of prime numbers occurring in each process

### Example

**Input:**
X = 1
Y = 100
**Output:**

### Parallelization Scenario

#### Master Process:

- Calculate the sub-range size `r = (y - x) / p` (where `p` is the number of processes).
- Broadcast `x` and `r` to each slave process using `MPI_Send`.
- Receive the sub-count from each slave process using `MPI_Receive`.
- Print the total count of primes between `x` and `y`.

#### Slave Process:

- Receive `x` and `r` through `MPI_Receive`.
- Calculate the lower bound `a` and upper bound `b` according to its rank.
- Count primes in its sub-range (between `a` and `b`).
- Print the partial count.
- Send this partial count to the master process using `MPI_Send`.
