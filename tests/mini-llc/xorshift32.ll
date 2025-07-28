@format = private constant [4 x i8] c"%u\0A\00"

declare i32 @printf(ptr, ...)

define i32 @xorshift32(ptr %0) {
1:
  %2 = alloca ptr
  store ptr %0, ptr %2
  %3 = load ptr, ptr %2
  %4 = load i32, ptr %3
  %5 = shl i32 %4, 13
  %6 = load ptr, ptr %2
  %7 = load i32, ptr %6
  %8 = xor i32 %7, %5
  store i32 %8, ptr %6
  %9 = load ptr, ptr %2
  %10 = load i32, ptr %9
  %11 = lshr i32 %10, 17
  %12 = load ptr, ptr %2
  %13 = load i32, ptr %12
  %14 = xor i32 %13, %11
  store i32 %14, ptr %12
  %15 = load ptr, ptr %2
  %16 = load i32, ptr %15
  %17 = shl i32 %16, 5
  %18 = load ptr, ptr %2
  %19 = load i32, ptr %18
  %20 = xor i32 %19, %17
  store i32 %20, ptr %18
  %21 = load ptr, ptr %2
  %22 = load i32, ptr %21
  ret i32 %22
}

define void @xorshift32_seed(ptr %0, i32 %1) {
2:
  %3 = alloca ptr
  %4 = alloca i32
  store ptr %0, ptr %3
  store i32 %1, ptr %4
  %5 = load i32, ptr %4
  %6 = load ptr, ptr %3
  store i32 %5, ptr %6
  ret void
}

define i32 @main() {
0:
  %1 = alloca i32
  call void @xorshift32_seed(ptr %1, i32 123456789)
  %2 = call i32 @xorshift32(ptr %1)
  %3 = call i32 (ptr, ...) @printf(ptr @format, i32 %2)
  %4 = call i32 @xorshift32(ptr %1)
  %5 = call i32 (ptr, ...) @printf(ptr @format, i32 %4)
  %6 = call i32 @xorshift32(ptr %1)
  %7 = call i32 (ptr, ...) @printf(ptr @format, i32 %6)
  %8 = call i32 @xorshift32(ptr %1)
  %9 = call i32 (ptr, ...) @printf(ptr @format, i32 %8)
  %10 = call i32 @xorshift32(ptr %1)
  %11 = call i32 (ptr, ...) @printf(ptr @format, i32 %10)
  ret i32 0
}
