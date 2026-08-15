@test1 = global i32 poison
@test2 = global [2 x i32] [i32 1, i32 poison]
@test3 = global [2 x [2 x i32]] [[2 x i32] [i32 1, i32 poison], [2 x i32] poison]

define i32 @main() {
0:
  ret i32 0
}
