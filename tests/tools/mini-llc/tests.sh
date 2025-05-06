#!/bin/bash

set -e

tests=(
    circular_call
    irreducible_cfg
    self_loop
    self_referential_phi
    phi_lost_copy
    phi_swap
    pure_impure
    many_args
    many_regs
    many_regs_large_stack_frame
    arithmetic
    mul
    div
    rem
    relational
    casting
    select
    getelementptr
    max
    sum
    fibonacci
    power
    is_prime
    gcd
    gcd_iterative
    exgcd
    hanoi
    partition_function
    n_queens
    accumulate
    matmul
    edit_distance
    sieve_of_eratosthenes
    bubble_sort
    selection_sort
    insertion_sort
    heapsort
    quicksort
    mergesort
    dfs
    bfs
    topological_sort
    tarjan_strongly_connected_components
    dijkstra_shortest_paths
    gaussian_elimination
    xorshift32
    xorshift64
    splitmix64
    pcg32
)

for test_name in "${tests[@]}"; do
    echo "$test_name"
done
