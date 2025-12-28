@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i32 @fibonacci(i32 %0) {
1:
  %2 = alloca i32
  %3 = alloca i32
  %4 = alloca i32
  %5 = alloca i32
  %6 = alloca i32
  store i32 %0, ptr %2
  store i32 0, ptr %3
  store i32 1, ptr %4
  store i32 1, ptr %5
  br label %7

7:
  %8 = load i32, ptr %5
  %9 = load i32, ptr %2
  %10 = icmp sle i32 %8, %9
  br i1 %10, label %11, label %20

11:
  %12 = load i32, ptr %3
  %13 = load i32, ptr %4
  %14 = add i32 %12, %13
  store i32 %14, ptr %6
  %15 = load i32, ptr %4
  store i32 %15, ptr %3
  %16 = load i32, ptr %6
  store i32 %16, ptr %4
  br label %17

17:
  %18 = load i32, ptr %5
  %19 = add i32 %18, 1
  store i32 %19, ptr %5
  br label %7

20:
  %21 = load i32, ptr %3
  ret i32 %21
}

define i32 @main() {
0:
  %1 = call i32 @fibonacci(i32 0)
  %2 = call i32 (ptr, ...) @printf(ptr @format, i32 %1)
  %3 = call i32 @fibonacci(i32 1)
  %4 = call i32 (ptr, ...) @printf(ptr @format, i32 %3)
  %5 = call i32 @fibonacci(i32 2)
  %6 = call i32 (ptr, ...) @printf(ptr @format, i32 %5)
  %7 = call i32 @fibonacci(i32 5)
  %8 = call i32 (ptr, ...) @printf(ptr @format, i32 %7)
  %9 = call i32 @fibonacci(i32 40)
  %10 = call i32 (ptr, ...) @printf(ptr @format, i32 %9)
  ret i32 0
}
