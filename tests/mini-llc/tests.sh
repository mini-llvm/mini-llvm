#!/bin/bash

set -e

tests=(
    alias_args
    alias_gep
    alias_inttoptr
    arithmetic
    casting
    circular_call
    div
    escape_arg
    escape_global
    getelementptr
    getelementptr_0
    global_values
    impure
    indirect_call
    irreducible_cfg
    many_args
    many_regs
    many_regs_large_stack_frame
    mul
    phi_lost_copy
    phi_swap
    relational
    rem
    select
    select_ptr
    self_loop
    self_referential

    accumulate
    bfs
    bit
    bubble_sort
    dfs
    dijkstra_shortest_paths
    edit_distance
    exgcd
    fibonacci
    floyd_warshall_all_pairs_shortest_paths
    gaussian_elimination
    gcd
    gcd_iterative
    hanoi
    heapsort
    insertion_sort
    is_prime
    linear_regression
    matmul
    max
    mergesort
    n_queens
    partition_function
    pcg32
    power
    quicksort
    selection_sort
    sieve_of_eratosthenes
    splitmix64
    sum
    tarjan_strongly_connected_components
    topological_sort
    xorshift32
    xorshift64
)

for test_name in "${tests[@]}"; do
    echo "$test_name"
done
