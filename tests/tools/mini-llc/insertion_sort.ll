@__a = internal global [8 x i32] [i32 5, i32 4, i32 1, i32 3, i32 8, i32 7, i32 6, i32 2]
@__format = internal global [25 x i8] c"%d %d %d %d %d %d %d %d\0A\00"

declare ptr @memcpy(ptr, ptr, i64)
declare i32 @printf(ptr, ...)

define void @insertion_sort(ptr %0, i32 %1) {
2:
  %3 = alloca ptr
  %4 = alloca i32
  %5 = alloca i32
  %6 = alloca i32
  %7 = alloca i32
  store ptr %0, ptr %3
  store i32 %1, ptr %4
  store i32 1, ptr %5
  br label %8

8:
  %9 = load i32, ptr %5
  %10 = load i32, ptr %4
  %11 = icmp slt i32 %9, %10
  br i1 %11, label %12, label %55

12:
  %13 = load ptr, ptr %3
  %14 = load i32, ptr %5
  %15 = sext i32 %14 to i64
  %16 = getelementptr i32, ptr %13, i64 %15
  %17 = load i32, ptr %16
  store i32 %17, ptr %6
  %18 = load i32, ptr %5
  store i32 %18, ptr %7
  br label %19

19:
  %20 = load i32, ptr %7
  %21 = icmp sgt i32 %20, 0
  br i1 %21, label %22, label %31

22:
  %23 = load ptr, ptr %3
  %24 = load i32, ptr %7
  %25 = sub i32 %24, 1
  %26 = sext i32 %25 to i64
  %27 = getelementptr i32, ptr %23, i64 %26
  %28 = load i32, ptr %27
  %29 = load i32, ptr %6
  %30 = icmp sgt i32 %28, %29
  br label %31

31:
  %32 = phi i1 [ false, %19 ], [ %30, %22 ]
  br i1 %32, label %33, label %46

33:
  %34 = load ptr, ptr %3
  %35 = load i32, ptr %7
  %36 = sub i32 %35, 1
  %37 = sext i32 %36 to i64
  %38 = getelementptr i32, ptr %34, i64 %37
  %39 = load i32, ptr %38
  %40 = load ptr, ptr %3
  %41 = load i32, ptr %7
  %42 = sext i32 %41 to i64
  %43 = getelementptr i32, ptr %40, i64 %42
  store i32 %39, ptr %43
  %44 = load i32, ptr %7
  %45 = add i32 %44, -1
  store i32 %45, ptr %7
  br label %19

46:
  %47 = load i32, ptr %6
  %48 = load ptr, ptr %3
  %49 = load i32, ptr %7
  %50 = sext i32 %49 to i64
  %51 = getelementptr i32, ptr %48, i64 %50
  store i32 %47, ptr %51
  br label %52

52:
  %53 = load i32, ptr %5
  %54 = add i32 %53, 1
  store i32 %54, ptr %5
  br label %8

55:
  ret void
}

define i32 @main() {
0:
  %1 = alloca [8 x i32]
  %2 = call ptr @memcpy(ptr %1, ptr @__a, i64 32)
  call void @insertion_sort(ptr %1, i32 8)
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
