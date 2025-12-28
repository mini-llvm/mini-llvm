@format = private constant [10 x i8] c"%d %c %c\0A\00"

declare i32 @printf(ptr, ...)

define void @move(i32 %0, i8 %1, i8 %2) {
3:
  %4 = alloca i32
  %5 = alloca i8
  %6 = alloca i8
  store i32 %0, ptr %4
  store i8 %1, ptr %5
  store i8 %2, ptr %6
  %7 = load i32, ptr %4
  %8 = load i8, ptr %5
  %9 = sext i8 %8 to i32
  %10 = load i8, ptr %6
  %11 = sext i8 %10 to i32
  %12 = call i32 (ptr, ...) @printf(ptr @format, i32 %7, i32 %9, i32 %11)
  ret void
}

define void @hanoi(i32 %0, i8 %1, i8 %2, i8 %3) {
4:
  %5 = alloca i32
  %6 = alloca i8
  %7 = alloca i8
  %8 = alloca i8
  store i32 %0, ptr %5
  store i8 %1, ptr %6
  store i8 %2, ptr %7
  store i8 %3, ptr %8
  %9 = load i32, ptr %5
  %10 = icmp eq i32 %9, 1
  br i1 %10, label %11, label %14

11:
  %12 = load i8, ptr %6
  %13 = load i8, ptr %7
  call void @move(i32 1, i8 %12, i8 %13)
  br label %28

14:
  %15 = load i32, ptr %5
  %16 = sub i32 %15, 1
  %17 = load i8, ptr %6
  %18 = load i8, ptr %8
  %19 = load i8, ptr %7
  call void @hanoi(i32 %16, i8 %17, i8 %18, i8 %19)
  %20 = load i32, ptr %5
  %21 = load i8, ptr %6
  %22 = load i8, ptr %7
  call void @move(i32 %20, i8 %21, i8 %22)
  %23 = load i32, ptr %5
  %24 = sub i32 %23, 1
  %25 = load i8, ptr %8
  %26 = load i8, ptr %7
  %27 = load i8, ptr %6
  call void @hanoi(i32 %24, i8 %25, i8 %26, i8 %27)
  br label %28

28:
  ret void
}

define i32 @main() {
0:
  call void @hanoi(i32 6, i8 65, i8 66, i8 67)
  ret i32 0
}
