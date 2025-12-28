@format = private constant [6 x i8] c"%llu\0A\00"

declare i32 @printf(ptr, ...)

define i64 @splitmix64(ptr %0) {
1:
  %2 = alloca ptr
  %3 = alloca i64
  store ptr %0, ptr %2
  %4 = load ptr, ptr %2
  %5 = load i64, ptr %4
  %6 = add i64 %5, -7046029254386353131
  store i64 %6, ptr %4
  store i64 %6, ptr %3
  %7 = load i64, ptr %3
  %8 = load i64, ptr %3
  %9 = lshr i64 %8, 30
  %10 = xor i64 %7, %9
  %11 = mul i64 %10, -4658895280553007687
  store i64 %11, ptr %3
  %12 = load i64, ptr %3
  %13 = load i64, ptr %3
  %14 = lshr i64 %13, 27
  %15 = xor i64 %12, %14
  %16 = mul i64 %15, -7723592293110705685
  store i64 %16, ptr %3
  %17 = load i64, ptr %3
  %18 = load i64, ptr %3
  %19 = lshr i64 %18, 31
  %20 = xor i64 %17, %19
  store i64 %20, ptr %3
  %21 = load i64, ptr %3
  %22 = load ptr, ptr %2
  store i64 %21, ptr %22
  %23 = load ptr, ptr %2
  %24 = load i64, ptr %23
  ret i64 %24
}

define void @splitmix64_seed(ptr %0, i64 %1) {
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
  call void @splitmix64_seed(ptr %1, i64 123456789)
  %2 = call i64 @splitmix64(ptr %1)
  %3 = call i32 (ptr, ...) @printf(ptr @format, i64 %2)
  %4 = call i64 @splitmix64(ptr %1)
  %5 = call i32 (ptr, ...) @printf(ptr @format, i64 %4)
  %6 = call i64 @splitmix64(ptr %1)
  %7 = call i32 (ptr, ...) @printf(ptr @format, i64 %6)
  %8 = call i64 @splitmix64(ptr %1)
  %9 = call i32 (ptr, ...) @printf(ptr @format, i64 %8)
  %10 = call i64 @splitmix64(ptr %1)
  %11 = call i32 (ptr, ...) @printf(ptr @format, i64 %10)
  ret i32 0
}
