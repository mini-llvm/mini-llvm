@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i32 @main() {
0:
  %1 = alloca [4 x i32]
  %2 = getelementptr i32, ptr %1, i32 1
  %3 = getelementptr i32, ptr %2, i32 1
  %4 = getelementptr i32, ptr %3, i32 1
  %5 = getelementptr [4 x i32], ptr %1, i32 0, i32 3
  store i32 -1, ptr %4
  %6 = load i32, ptr %5
  store i32 1, ptr %4
  %7 = load i32, ptr %5
  %8 = call i32 (ptr, ...) @printf(ptr @format, i32 %6)
  %9 = call i32 (ptr, ...) @printf(ptr @format, i32 %7)
  ret i32 0
}
