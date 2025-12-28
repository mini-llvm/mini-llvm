@a = global [3 x i32] [i32 0, i32 1, i32 2]

@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i32 @main() {
0:
  %1 = getelementptr i32, ptr @a, i32 1
  %2 = getelementptr i32, ptr %1, i64 1
  %3 = load i32, ptr %2
  %4 = call i32 (ptr, ...) @printf(ptr @format, i32 %3)
  ret i32 0
}
