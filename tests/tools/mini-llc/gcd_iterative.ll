@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i32 @gcd(i32 %0, i32 %1) {
2:
  %3 = alloca i32
  %4 = alloca i32
  %5 = alloca i32
  store i32 %0, ptr %3
  store i32 %1, ptr %4
  br label %6

6:
  %7 = load i32, ptr %4
  %8 = icmp ne i32 %7, 0
  br i1 %8, label %9, label %15

9:
  %10 = load i32, ptr %3
  %11 = load i32, ptr %4
  %12 = srem i32 %10, %11
  store i32 %12, ptr %5
  %13 = load i32, ptr %4
  store i32 %13, ptr %3
  %14 = load i32, ptr %5
  store i32 %14, ptr %4
  br label %6

15:
  %16 = load i32, ptr %3
  ret i32 %16
}

define i32 @main() {
0:
  %1 = call i32 @gcd(i32 1, i32 0)
  %2 = call i32 (ptr, ...) @printf(ptr @format, i32 %1)
  %3 = call i32 @gcd(i32 30, i32 42)
  %4 = call i32 (ptr, ...) @printf(ptr @format, i32 %3)
  %5 = call i32 @gcd(i32 2136, i32 3456)
  %6 = call i32 (ptr, ...) @printf(ptr @format, i32 %5)
  ret i32 0
}
