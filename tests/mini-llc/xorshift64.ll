@format = private constant [6 x i8] c"%llu\0A\00"

declare i32 @printf(ptr, ...)

define i64 @xorshift64(ptr %0) {
1:
  %2 = alloca ptr
  store ptr %0, ptr %2
  %3 = load ptr, ptr %2
  %4 = load i64, ptr %3
  %5 = shl i64 %4, 13
  %6 = load ptr, ptr %2
  %7 = load i64, ptr %6
  %8 = xor i64 %7, %5
  store i64 %8, ptr %6
  %9 = load ptr, ptr %2
  %10 = load i64, ptr %9
  %11 = lshr i64 %10, 7
  %12 = load ptr, ptr %2
  %13 = load i64, ptr %12
  %14 = xor i64 %13, %11
  store i64 %14, ptr %12
  %15 = load ptr, ptr %2
  %16 = load i64, ptr %15
  %17 = shl i64 %16, 17
  %18 = load ptr, ptr %2
  %19 = load i64, ptr %18
  %20 = xor i64 %19, %17
  store i64 %20, ptr %18
  %21 = load ptr, ptr %2
  %22 = load i64, ptr %21
  ret i64 %22
}

define void @xorshift64_seed(ptr %0, i64 %1) {
2:
  %3 = alloca ptr
  %4 = alloca i64
  store ptr %0, ptr %3
  store i64 %1, ptr %4
  %5 = load i64, ptr %4
  %6 = load ptr, ptr %3
  store i64 %5, ptr %6
  ret void
}

define i32 @main() {
0:
  %1 = alloca i64
  call void @xorshift64_seed(ptr %1, i64 123456789)
  %2 = call i64 @xorshift64(ptr %1)
  %3 = call i32 (ptr, ...) @printf(ptr @format, i64 %2)
  %4 = call i64 @xorshift64(ptr %1)
  %5 = call i32 (ptr, ...) @printf(ptr @format, i64 %4)
  %6 = call i64 @xorshift64(ptr %1)
  %7 = call i32 (ptr, ...) @printf(ptr @format, i64 %6)
  %8 = call i64 @xorshift64(ptr %1)
  %9 = call i32 (ptr, ...) @printf(ptr @format, i64 %8)
  %10 = call i64 @xorshift64(ptr %1)
  %11 = call i32 (ptr, ...) @printf(ptr @format, i64 %10)
  ret i32 0
}
