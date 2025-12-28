@a = private constant [16 x i32] [i32 7, i32 13, i32 11, i32 10, i32 5, i32 12, i32 0, i32 8, i32 14, i32 9, i32 6, i32 2, i32 3, i32 15, i32 1, i32 4]
@format = private constant [4 x i8] c"%d\0A\00"

declare ptr @memcpy(ptr, ptr, i64) argmemonly
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
  %1 = alloca [16 x i32]
  %2 = call ptr @memcpy(ptr %1, ptr @a, i64 64)
  call void @insertion_sort(ptr %1, i32 16)
  %3 = getelementptr [16 x i32], ptr %1, i64 0, i64 0
  %4 = load i32, ptr %3
  %5 = call i32 @printf(ptr @format, i32 %4)
  %6 = getelementptr [16 x i32], ptr %1, i64 0, i64 1
  %7 = load i32, ptr %6
  %8 = call i32 @printf(ptr @format, i32 %7)
  %9 = getelementptr [16 x i32], ptr %1, i64 0, i64 2
  %10 = load i32, ptr %9
  %11 = call i32 @printf(ptr @format, i32 %10)
  %12 = getelementptr [16 x i32], ptr %1, i64 0, i64 3
  %13 = load i32, ptr %12
  %14 = call i32 @printf(ptr @format, i32 %13)
  %15 = getelementptr [16 x i32], ptr %1, i64 0, i64 4
  %16 = load i32, ptr %15
  %17 = call i32 @printf(ptr @format, i32 %16)
  %18 = getelementptr [16 x i32], ptr %1, i64 0, i64 5
  %19 = load i32, ptr %18
  %20 = call i32 @printf(ptr @format, i32 %19)
  %21 = getelementptr [16 x i32], ptr %1, i64 0, i64 6
  %22 = load i32, ptr %21
  %23 = call i32 @printf(ptr @format, i32 %22)
  %24 = getelementptr [16 x i32], ptr %1, i64 0, i64 7
  %25 = load i32, ptr %24
  %26 = call i32 @printf(ptr @format, i32 %25)
  %27 = getelementptr [16 x i32], ptr %1, i64 0, i64 8
  %28 = load i32, ptr %27
  %29 = call i32 @printf(ptr @format, i32 %28)
  %30 = getelementptr [16 x i32], ptr %1, i64 0, i64 9
  %31 = load i32, ptr %30
  %32 = call i32 @printf(ptr @format, i32 %31)
  %33 = getelementptr [16 x i32], ptr %1, i64 0, i64 10
  %34 = load i32, ptr %33
  %35 = call i32 @printf(ptr @format, i32 %34)
  %36 = getelementptr [16 x i32], ptr %1, i64 0, i64 11
  %37 = load i32, ptr %36
  %38 = call i32 @printf(ptr @format, i32 %37)
  %39 = getelementptr [16 x i32], ptr %1, i64 0, i64 12
  %40 = load i32, ptr %39
  %41 = call i32 @printf(ptr @format, i32 %40)
  %42 = getelementptr [16 x i32], ptr %1, i64 0, i64 13
  %43 = load i32, ptr %42
  %44 = call i32 @printf(ptr @format, i32 %43)
  %45 = getelementptr [16 x i32], ptr %1, i64 0, i64 14
  %46 = load i32, ptr %45
  %47 = call i32 @printf(ptr @format, i32 %46)
  %48 = getelementptr [16 x i32], ptr %1, i64 0, i64 15
  %49 = load i32, ptr %48
  %50 = call i32 @printf(ptr @format, i32 %49)
  ret i32 0
}
