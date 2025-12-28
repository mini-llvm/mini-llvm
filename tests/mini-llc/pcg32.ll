@format = private constant [4 x i8] c"%u\0A\00"

declare i32 @printf(ptr, ...)

define i32 @pcg32(ptr %0, i64 %1) {
2:
  %3 = alloca ptr
  %4 = alloca i64
  %5 = alloca i64
  %6 = alloca i32
  %7 = alloca i32
  store ptr %0, ptr %3
  store i64 %1, ptr %4
  %8 = load ptr, ptr %3
  %9 = load i64, ptr %8
  store i64 %9, ptr %5
  %10 = load i64, ptr %5
  %11 = mul i64 %10, 6364136223846793005
  %12 = load i64, ptr %4
  %13 = add i64 %11, %12
  %14 = load ptr, ptr %3
  store i64 %13, ptr %14
  %15 = load i64, ptr %5
  %16 = lshr i64 %15, 18
  %17 = load i64, ptr %5
  %18 = xor i64 %16, %17
  %19 = lshr i64 %18, 27
  %20 = trunc i64 %19 to i32
  store i32 %20, ptr %6
  %21 = load i64, ptr %5
  %22 = lshr i64 %21, 59
  %23 = trunc i64 %22 to i32
  store i32 %23, ptr %7
  %24 = load i32, ptr %6
  %25 = load i32, ptr %7
  %26 = lshr i32 %24, %25
  %27 = load i32, ptr %6
  %28 = load i32, ptr %7
  %29 = sub i32 0, %28
  %30 = and i32 %29, 31
  %31 = shl i32 %27, %30
  %32 = or i32 %26, %31
  ret i32 %32
}

define void @pcg32_seed(ptr %0, ptr %1, i64 %2, i64 %3) {
4:
  %5 = alloca ptr
  %6 = alloca ptr
  %7 = alloca i64
  %8 = alloca i64
  store ptr %0, ptr %5
  store ptr %1, ptr %6
  store i64 %2, ptr %7
  store i64 %3, ptr %8
  %9 = load ptr, ptr %5
  store i64 0, ptr %9
  %10 = load i64, ptr %8
  %11 = shl i64 %10, 1
  %12 = or i64 %11, 1
  %13 = load ptr, ptr %6
  store i64 %12, ptr %13
  %14 = load ptr, ptr %5
  %15 = load ptr, ptr %6
  %16 = load i64, ptr %15
  %17 = call i32 @pcg32(ptr %14, i64 %16)
  %18 = load i64, ptr %7
  %19 = load ptr, ptr %5
  %20 = load i64, ptr %19
  %21 = add i64 %20, %18
  store i64 %21, ptr %19
  %22 = load ptr, ptr %5
  %23 = load ptr, ptr %6
  %24 = load i64, ptr %23
  %25 = call i32 @pcg32(ptr %22, i64 %24)
  ret void
}

define i32 @main() {
0:
  %1 = alloca i64
  %2 = alloca i64
  call void @pcg32_seed(ptr %1, ptr %2, i64 123456789, i64 1)
  %3 = load i64, ptr %2
  %4 = call i32 @pcg32(ptr %1, i64 %3)
  %5 = call i32 (ptr, ...) @printf(ptr @format, i32 %4)
  %6 = call i32 @pcg32(ptr %1, i64 %3)
  %7 = call i32 (ptr, ...) @printf(ptr @format, i32 %6)
  %8 = call i32 @pcg32(ptr %1, i64 %3)
  %9 = call i32 (ptr, ...) @printf(ptr @format, i32 %8)
  %10 = call i32 @pcg32(ptr %1, i64 %3)
  %11 = call i32 (ptr, ...) @printf(ptr @format, i32 %10)
  %12 = call i32 @pcg32(ptr %1, i64 %3)
  %13 = call i32 (ptr, ...) @printf(ptr @format, i32 %12)
  ret i32 0
}
