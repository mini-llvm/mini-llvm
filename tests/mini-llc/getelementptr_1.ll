@a = global [2 x [3 x [4 x i32]]] [
  [3 x [4 x i32]] [
    [4 x i32] [i32 0, i32 1, i32 2, i32 3],
    [4 x i32] [i32 4, i32 5, i32 6, i32 7],
    [4 x i32] [i32 8, i32 9, i32 10, i32 11]
  ],
  [3 x [4 x i32]] [
    [4 x i32] [i32 12, i32 13, i32 14, i32 15],
    [4 x i32] [i32 16, i32 17, i32 18, i32 19],
    [4 x i32] [i32 20, i32 21, i32 22, i32 23]
  ]
]

@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i32 @main() {
0:
  %1 = getelementptr [2 x [3 x [4 x i32]]], ptr @a, i32 0, i32 1, i32 2, i32 3
  %2 = load i32, ptr %1
  %3 = call i32 (ptr, ...) @printf(ptr @format, i32 %2)
  %4 = getelementptr [2 x [3 x [4 x i32]]], ptr @a, i32 0, i32 1
  %5 = getelementptr [3 x [4 x i32]], ptr %4, i32 0, i32 2
  %6 = getelementptr [4 x i32], ptr %5, i32 0, i32 3
  %7 = load i32, ptr %6
  %8 = call i32 (ptr, ...) @printf(ptr @format, i32 %7)
  ret i32 0
}
