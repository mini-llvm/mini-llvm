@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i32 @sum(i32 %0) {
1:
  %2 = alloca i32
  %3 = alloca i32
  store i32 0, ptr %2
  store i32 1, ptr %3
  br label %4

4:
  %5 = load i32, ptr %3
  %6 = icmp sle i32 %5, %0
  br i1 %6, label %7, label %11

7:
  %8 = load i32, ptr %2
  %9 = add i32 %8, %5
  store i32 %9, ptr %2
  %10 = add i32 %5, 1
  store i32 %10, ptr %3
  br label %4

11:
  %12 = load i32, ptr %2
  ret i32 %12
}

define i32 @main() {
0:
  %1 = call i32 @sum(i32 0)
  %2 = call i32 (ptr, ...) @printf(ptr @format, i32 %1)
  %3 = call i32 @sum(i32 1)
  %4 = call i32 (ptr, ...) @printf(ptr @format, i32 %3)
  %5 = call i32 @sum(i32 100)
  %6 = call i32 (ptr, ...) @printf(ptr @format, i32 %5)
  ret i32 0
}
