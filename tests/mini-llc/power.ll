@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i32 @power(i32 %0, i32 %1) {
2:
  %3 = alloca i32
  %4 = alloca i32
  %5 = alloca i32
  store i32 %0, ptr %3
  store i32 %1, ptr %4
  store i32 1, ptr %5
  br label %6

6:
  %7 = load i32, ptr %4
  %8 = icmp ne i32 %7, 0
  br i1 %8, label %9, label %23

9:
  %10 = load i32, ptr %4
  %11 = and i32 %10, 1
  %12 = icmp ne i32 %11, 0
  br i1 %12, label %13, label %17

13:
  %14 = load i32, ptr %3
  %15 = load i32, ptr %5
  %16 = mul i32 %15, %14
  store i32 %16, ptr %5
  br label %17

17:
  %18 = load i32, ptr %3
  %19 = load i32, ptr %3
  %20 = mul i32 %19, %18
  store i32 %20, ptr %3
  %21 = load i32, ptr %4
  %22 = ashr i32 %21, 1
  store i32 %22, ptr %4
  br label %6

23:
  %24 = load i32, ptr %5
  ret i32 %24
}

define i32 @main() {
0:
  %1 = call i32 @power(i32 2, i32 0)
  %2 = call i32 (ptr, ...) @printf(ptr @format, i32 %1)
  %3 = call i32 @power(i32 3, i32 5)
  %4 = call i32 (ptr, ...) @printf(ptr @format, i32 %3)
  %5 = call i32 @power(i32 4, i32 10)
  %6 = call i32 (ptr, ...) @printf(ptr @format, i32 %5)
  ret i32 0
}
