#!/bin/bash

set -e

tests=(
    global_values
    self_loop
    irreducible_cfg
    self_referential
    lost_copy
    swap
    alias_args
    alias_gep
    alias_inttoptr
    escape_arg
    escape_global
    impure
    circular_call
    many_args
    many_regs
    many_regs_large_stack_frame
    arithmetic
    mul
    div
    rem
    relational
    casting
    getelementptr
    getelementptr_0
    select
    select_ptr
    indirect_call
    bit
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
    floyd_warshall_all_pairs_shortest_paths
    gaussian_elimination
    linear_regression
    xorshift32
    xorshift64
    splitmix64
    pcg32
)

for test_name in "${tests[@]}"; do
    echo "$test_name"
done
