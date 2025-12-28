@format = private constant [10 x i8] c"%d %d %d\0A\00"

declare i32 @printf(ptr, ...)

define void @exgcd(i32 %0, i32 %1, ptr %2, ptr %3, ptr %4) {
5:
  %6 = alloca i32
  %7 = alloca i32
  %8 = alloca ptr
  %9 = alloca ptr
  %10 = alloca ptr
  store i32 %0, ptr %6
  store i32 %1, ptr %7
  store ptr %2, ptr %8
  store ptr %3, ptr %9
  store ptr %4, ptr %10
  %11 = load i32, ptr %7
  %12 = icmp eq i32 %11, 0
  br i1 %12, label %13, label %18

13:
  %14 = load i32, ptr %6
  %15 = load ptr, ptr %8
  store i32 %14, ptr %15
  %16 = load ptr, ptr %9
  store i32 1, ptr %16
  %17 = load ptr, ptr %10
  store i32 0, ptr %17
  br label %35

18:
  %19 = load i32, ptr %7
  %20 = load i32, ptr %6
  %21 = load i32, ptr %7
  %22 = srem i32 %20, %21
  %23 = load ptr, ptr %8
  %24 = load ptr, ptr %10
  %25 = load ptr, ptr %9
  call void @exgcd(i32 %19, i32 %22, ptr %23, ptr %24, ptr %25)
  %26 = load ptr, ptr %9
  %27 = load i32, ptr %26
  %28 = load i32, ptr %6
  %29 = load i32, ptr %7
  %30 = sdiv i32 %28, %29
  %31 = mul i32 %27, %30
  %32 = load ptr, ptr %10
  %33 = load i32, ptr %32
  %34 = sub i32 %33, %31
  store i32 %34, ptr %32
  br label %35

35:
  ret void
}

define i32 @main() {
0:
  %1 = alloca i32
  %2 = alloca i32
  %3 = alloca i32
  call void @exgcd(i32 9, i32 9, ptr %1, ptr %2, ptr %3)
  %4 = load i32, ptr %1
  %5 = load i32, ptr %2
  %6 = load i32, ptr %3
  %7 = call i32 (ptr, ...) @printf(ptr @format, i32 %4, i32 %5, i32 %6)
  call void @exgcd(i32 30, i32 12, ptr %1, ptr %2, ptr %3)
  %8 = load i32, ptr %1
  %9 = load i32, ptr %2
  %10 = load i32, ptr %3
  %11 = call i32 (ptr, ...) @printf(ptr @format, i32 %8, i32 %9, i32 %10)
  call void @exgcd(i32 99, i32 78, ptr %1, ptr %2, ptr %3)
  %12 = load i32, ptr %1
  %13 = load i32, ptr %2
  %14 = load i32, ptr %3
  %15 = call i32 (ptr, ...) @printf(ptr @format, i32 %12, i32 %13, i32 %14)
  ret i32 0
}
