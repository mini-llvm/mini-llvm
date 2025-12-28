@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define void @backtrack(i32 %0, i32 %1, ptr %2) {
3:
  %4 = alloca i32
  %5 = alloca i32
  %6 = alloca ptr
  %7 = alloca i32
  store i32 %0, ptr %4
  store i32 %1, ptr %5
  store ptr %2, ptr %6
  %8 = load i32, ptr %4
  %9 = icmp eq i32 %8, 0
  br i1 %9, label %10, label %14

10:
  %11 = load ptr, ptr %6
  %12 = load i32, ptr %11
  %13 = add i32 %12, 1
  store i32 %13, ptr %11
  br label %29

14:
  %15 = load i32, ptr %5
  store i32 %15, ptr %7
  br label %16

16:
  %17 = load i32, ptr %7
  %18 = load i32, ptr %4
  %19 = icmp sle i32 %17, %18
  br i1 %19, label %20, label %29

20:
  %21 = load i32, ptr %4
  %22 = load i32, ptr %7
  %23 = sub i32 %21, %22
  %24 = load i32, ptr %7
  %25 = load ptr, ptr %6
  call void @backtrack(i32 %23, i32 %24, ptr %25)
  br label %26

26:
  %27 = load i32, ptr %7
  %28 = add i32 %27, 1
  store i32 %28, ptr %7
  br label %16

29:
  ret void
}

define i32 @partition(i32 %0) {
1:
  %2 = alloca i32
  %3 = alloca i32
  store i32 %0, ptr %2
  store i32 0, ptr %3
  %4 = load i32, ptr %2
  call void @backtrack(i32 %4, i32 1, ptr %3)
  %5 = load i32, ptr %3
  ret i32 %5
}

define i32 @main() {
0:
  %1 = call i32 @partition(i32 0)
  %2 = call i32 (ptr, ...) @printf(ptr @format, i32 %1)
  %3 = call i32 @partition(i32 1)
  %4 = call i32 (ptr, ...) @printf(ptr @format, i32 %3)
  %5 = call i32 @partition(i32 5)
  %6 = call i32 (ptr, ...) @printf(ptr @format, i32 %5)
  %7 = call i32 @partition(i32 10)
  %8 = call i32 (ptr, ...) @printf(ptr @format, i32 %7)
  %9 = call i32 @partition(i32 40)
  %10 = call i32 (ptr, ...) @printf(ptr @format, i32 %9)
  ret i32 0
}
