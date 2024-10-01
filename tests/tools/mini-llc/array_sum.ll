@__a = internal global [8 x i32] [i32 5, i32 4, i32 1, i32 3, i32 8, i32 7, i32 6, i32 2]
@__format = internal global [4 x i8] c"%d\0A\00"

declare ptr @memcpy(ptr, ptr, i64)
declare i32 @printf(ptr, ...)

define i32 @array_sum(ptr %0, i32 %1) {
2:
  %3 = alloca ptr
  %4 = alloca i32
  %5 = alloca i32
  %6 = alloca i32
  store ptr %0, ptr %3
  store i32 %1, ptr %4
  store i32 0, ptr %5
  store i32 0, ptr %6
  br label %7

7:
  %8 = load i32, ptr %6
  %9 = load i32, ptr %4
  %10 = icmp slt i32 %8, %9
  br i1 %10, label %11, label %22

11:
  %12 = load ptr, ptr %3
  %13 = load i32, ptr %6
  %14 = sext i32 %13 to i64
  %15 = getelementptr i32, ptr %12, i64 %14
  %16 = load i32, ptr %15
  %17 = load i32, ptr %5
  %18 = add i32 %17, %16
  store i32 %18, ptr %5
  br label %19

19:
  %20 = load i32, ptr %6
  %21 = add i32 %20, 1
  store i32 %21, ptr %6
  br label %7

22:
  %23 = load i32, ptr %5
  ret i32 %23
}

define i32 @main() {
0:
  %1 = alloca [8 x i32]
  %2 = call ptr @memcpy(ptr %1, ptr @__a, i64 32)
  %3 = call i32 @array_sum(ptr %1, i32 8)
  %4 = call i32 @printf(ptr @__format, i32 %3)
  ret i32 0
}
