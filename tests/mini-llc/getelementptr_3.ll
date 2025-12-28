@a = global [2 x i32] [i32 0, i32 1]

@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i32 @main() {
0:
  %1 = getelementptr [2 x i32], ptr @a, i32 0, i64 1
  %2 = load i32, ptr %1
  %3 = call i32 (ptr, ...) @printf(ptr @format, i32 %2)
  ret i32 0
}
