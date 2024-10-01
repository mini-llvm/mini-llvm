@__a = internal global [8 x i32] [i32 5, i32 4, i32 1, i32 3, i32 8, i32 7, i32 6, i32 2]
@__format = internal global [25 x i8] c"%d %d %d %d %d %d %d %d\0A\00"

declare ptr @memcpy(ptr, ptr, i64)
declare i32 @printf(ptr, ...)

define void @swap(ptr %0, ptr %1) {
2:
  %3 = alloca ptr
  %4 = alloca ptr
  %5 = alloca i32
  store ptr %0, ptr %3
  store ptr %1, ptr %4
  %6 = load ptr, ptr %3
  %7 = load i32, ptr %6
  store i32 %7, ptr %5
  %8 = load ptr, ptr %4
  %9 = load i32, ptr %8
  %10 = load ptr, ptr %3
  store i32 %9, ptr %10
  %11 = load i32, ptr %5
  %12 = load ptr, ptr %4
  store i32 %11, ptr %12
  ret void
}

define void @bubble_sort(ptr %0, i32 %1) {
2:
  %3 = alloca ptr
  %4 = alloca i32
  %5 = alloca i32
  %6 = alloca i32
  store ptr %0, ptr %3
  store i32 %1, ptr %4
  store i32 0, ptr %5
  br label %7

7:
  %8 = load i32, ptr %5
  %9 = load i32, ptr %4
  %10 = sub i32 %9, 1
  %11 = icmp slt i32 %8, %10
  br i1 %11, label %12, label %51

12:
  store i32 0, ptr %6
  br label %13

13:
  %14 = load i32, ptr %6
  %15 = load i32, ptr %4
  %16 = load i32, ptr %5
  %17 = sub i32 %15, %16
  %18 = sub i32 %17, 1
  %19 = icmp slt i32 %14, %18
  br i1 %19, label %20, label %47

20:
  %21 = load ptr, ptr %3
  %22 = load i32, ptr %6
  %23 = sext i32 %22 to i64
  %24 = getelementptr i32, ptr %21, i64 %23
  %25 = load i32, ptr %24
  %26 = load ptr, ptr %3
  %27 = load i32, ptr %6
  %28 = add i32 %27, 1
  %29 = sext i32 %28 to i64
  %30 = getelementptr i32, ptr %26, i64 %29
  %31 = load i32, ptr %30
  %32 = icmp sgt i32 %25, %31
  br i1 %32, label %33, label %43

33:
  %34 = load ptr, ptr %3
  %35 = load i32, ptr %6
  %36 = sext i32 %35 to i64
  %37 = getelementptr i32, ptr %34, i64 %36
  %38 = load ptr, ptr %3
  %39 = load i32, ptr %6
  %40 = add i32 %39, 1
  %41 = sext i32 %40 to i64
  %42 = getelementptr i32, ptr %38, i64 %41
  call void @swap(ptr %37, ptr %42)
  br label %43

43:
  br label %44

44:
  %45 = load i32, ptr %6
  %46 = add i32 %45, 1
  store i32 %46, ptr %6
  br label %13

47:
  br label %48

48:
  %49 = load i32, ptr %5
  %50 = add i32 %49, 1
  store i32 %50, ptr %5
  br label %7

51:
  ret void
}

define i32 @main() {
0:
  %1 = alloca [8 x i32]
  %2 = call ptr @memcpy(ptr %1, ptr @__a, i64 32)
  call void @bubble_sort(ptr %1, i32 8)
  %3 = getelementptr [8 x i32], ptr %1, i64 0, i64 0
  %4 = getelementptr [8 x i32], ptr %1, i64 0, i64 1
  %5 = getelementptr [8 x i32], ptr %1, i64 0, i64 2
  %6 = getelementptr [8 x i32], ptr %1, i64 0, i64 3
  %7 = getelementptr [8 x i32], ptr %1, i64 0, i64 4
  %8 = getelementptr [8 x i32], ptr %1, i64 0, i64 5
  %9 = getelementptr [8 x i32], ptr %1, i64 0, i64 6
  %10 = getelementptr [8 x i32], ptr %1, i64 0, i64 7
  %11 = load i32, ptr %3
  %12 = load i32, ptr %4
  %13 = load i32, ptr %5
  %14 = load i32, ptr %6
  %15 = load i32, ptr %7
  %16 = load i32, ptr %8
  %17 = load i32, ptr %9
  %18 = load i32, ptr %10
  %19 = call i32 @printf(ptr @__format, i32 %11, i32 %12, i32 %13, i32 %14, i32 %15, i32 %16, i32 %17, i32 %18)
  ret i32 0
}
