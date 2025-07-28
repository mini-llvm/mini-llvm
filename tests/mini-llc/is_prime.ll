@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i32 @is_prime(i32 %0) {
1:
  %2 = alloca i32
  %3 = alloca i32
  %4 = alloca i32
  store i32 %0, ptr %3
  %5 = load i32, ptr %3
  %6 = icmp slt i32 %5, 2
  br i1 %6, label %7, label %8

7:
  store i32 0, ptr %2
  br label %26

8:
  store i32 2, ptr %4
  br label %9

9:
  %10 = load i32, ptr %4
  %11 = load i32, ptr %4
  %12 = mul i32 %10, %11
  %13 = load i32, ptr %3
  %14 = icmp sle i32 %12, %13
  br i1 %14, label %15, label %25

15:
  %16 = load i32, ptr %3
  %17 = load i32, ptr %4
  %18 = srem i32 %16, %17
  %19 = icmp eq i32 %18, 0
  br i1 %19, label %20, label %21

20:
  store i32 0, ptr %2
  br label %26

21:
  br label %22

22:
  %23 = load i32, ptr %4
  %24 = add i32 %23, 1
  store i32 %24, ptr %4
  br label %9

25:
  store i32 1, ptr %2
  br label %26

26:
  %27 = load i32, ptr %2
  ret i32 %27
}

define i32 @main() {
0:
  %1 = call i32 @is_prime(i32 1)
  %2 = call i32 (ptr, ...) @printf(ptr @format, i32 %1)
  %3 = call i32 @is_prime(i32 2)
  %4 = call i32 (ptr, ...) @printf(ptr @format, i32 %3)
  %5 = call i32 @is_prime(i32 3)
  %6 = call i32 (ptr, ...) @printf(ptr @format, i32 %5)
  %7 = call i32 @is_prime(i32 4)
  %8 = call i32 (ptr, ...) @printf(ptr @format, i32 %7)
  %9 = call i32 @is_prime(i32 5)
  %10 = call i32 (ptr, ...) @printf(ptr @format, i32 %9)
  %11 = call i32 @is_prime(i32 6)
  %12 = call i32 (ptr, ...) @printf(ptr @format, i32 %11)
  %13 = call i32 @is_prime(i32 7)
  %14 = call i32 (ptr, ...) @printf(ptr @format, i32 %13)
  %15 = call i32 @is_prime(i32 8)
  %16 = call i32 (ptr, ...) @printf(ptr @format, i32 %15)
  %17 = call i32 @is_prime(i32 9)
  %18 = call i32 (ptr, ...) @printf(ptr @format, i32 %17)
  %19 = call i32 @is_prime(i32 10)
  %20 = call i32 (ptr, ...) @printf(ptr @format, i32 %19)
  %21 = call i32 @is_prime(i32 11)
  %22 = call i32 (ptr, ...) @printf(ptr @format, i32 %21)
  %23 = call i32 @is_prime(i32 12)
  %24 = call i32 (ptr, ...) @printf(ptr @format, i32 %23)
  %25 = call i32 @is_prime(i32 13)
  %26 = call i32 (ptr, ...) @printf(ptr @format, i32 %25)
  %27 = call i32 @is_prime(i32 14)
  %28 = call i32 (ptr, ...) @printf(ptr @format, i32 %27)
  %29 = call i32 @is_prime(i32 15)
  %30 = call i32 (ptr, ...) @printf(ptr @format, i32 %29)
  %31 = call i32 @is_prime(i32 16)
  %32 = call i32 (ptr, ...) @printf(ptr @format, i32 %31)
  %33 = call i32 @is_prime(i32 17)
  %34 = call i32 (ptr, ...) @printf(ptr @format, i32 %33)
  %35 = call i32 @is_prime(i32 18)
  %36 = call i32 (ptr, ...) @printf(ptr @format, i32 %35)
  %37 = call i32 @is_prime(i32 19)
  %38 = call i32 (ptr, ...) @printf(ptr @format, i32 %37)
  %39 = call i32 @is_prime(i32 20)
  %40 = call i32 (ptr, ...) @printf(ptr @format, i32 %39)
  ret i32 0
}
