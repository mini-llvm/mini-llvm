@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i32 @max(i32 %0, i32 %1) {
2:
  %3 = alloca i32
  %4 = alloca i32
  store i32 %0, ptr %3
  store i32 %1, ptr %4
  %5 = load i32, ptr %3
  %6 = load i32, ptr %4
  %7 = icmp sgt i32 %5, %6
  br i1 %7, label %8, label %10

8:
  %9 = load i32, ptr %3
  br label %12

10:
  %11 = load i32, ptr %4
  br label %12

12:
  %13 = phi i32 [ %9, %8 ], [ %11, %10 ]
  ret i32 %13
}

define i32 @main() {
0:
  %1 = call i32 @max(i32 42, i32 43)
  %2 = call i32 (ptr, ...) @printf(ptr @format, i32 %1)
  %3 = call i32 @max(i32 43, i32 42)
  %4 = call i32 (ptr, ...) @printf(ptr @format, i32 %3)
  %5 = call i32 @max(i32 42, i32 -43)
  %6 = call i32 (ptr, ...) @printf(ptr @format, i32 %5)
  %7 = call i32 @max(i32 -43, i32 42)
  %8 = call i32 (ptr, ...) @printf(ptr @format, i32 %7)
  %9 = call i32 @max(i32 -42, i32 -43)
  %10 = call i32 (ptr, ...) @printf(ptr @format, i32 %9)
  %11 = call i32 @max(i32 -43, i32 -42)
  %12 = call i32 (ptr, ...) @printf(ptr @format, i32 %11)
  ret i32 0
}
