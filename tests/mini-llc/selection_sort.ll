@a = private constant [16 x i32] [i32 7, i32 13, i32 11, i32 10, i32 5, i32 12, i32 0, i32 8, i32 14, i32 9, i32 6, i32 2, i32 3, i32 15, i32 1, i32 4]
@format = private constant [4 x i8] c"%d\0A\00"

declare ptr @memcpy(ptr, ptr, i64) argmemonly
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

define void @selection_sort(ptr %0, i32 %1) {
2:
  %3 = alloca ptr
  %4 = alloca i32
  %5 = alloca i32
  %6 = alloca i32
  %7 = alloca i32
  store ptr %0, ptr %3
  store i32 %1, ptr %4
  store i32 0, ptr %5
  br label %8

8:
  %9 = load i32, ptr %5
  %10 = load i32, ptr %4
  %11 = sub i32 %10, 1
  %12 = icmp slt i32 %9, %11
  br i1 %12, label %13, label %51

13:
  %14 = load i32, ptr %5
  store i32 %14, ptr %6
  %15 = load i32, ptr %5
  %16 = add i32 %15, 1
  store i32 %16, ptr %7
  br label %17

17:
  %18 = load i32, ptr %7
  %19 = load i32, ptr %4
  %20 = icmp slt i32 %18, %19
  br i1 %20, label %21, label %39

21:
  %22 = load ptr, ptr %3
  %23 = load i32, ptr %7
  %24 = sext i32 %23 to i64
  %25 = getelementptr i32, ptr %22, i64 %24
  %26 = load i32, ptr %25
  %27 = load ptr, ptr %3
  %28 = load i32, ptr %6
  %29 = sext i32 %28 to i64
  %30 = getelementptr i32, ptr %27, i64 %29
  %31 = load i32, ptr %30
  %32 = icmp slt i32 %26, %31
  br i1 %32, label %33, label %35

33:
  %34 = load i32, ptr %7
  store i32 %34, ptr %6
  br label %35

35:
  br label %36

36:
  %37 = load i32, ptr %7
  %38 = add i32 %37, 1
  store i32 %38, ptr %7
  br label %17

39:
  %40 = load ptr, ptr %3
  %41 = load i32, ptr %5
  %42 = sext i32 %41 to i64
  %43 = getelementptr i32, ptr %40, i64 %42
  %44 = load ptr, ptr %3
  %45 = load i32, ptr %6
  %46 = sext i32 %45 to i64
  %47 = getelementptr i32, ptr %44, i64 %46
  call void @swap(ptr %43, ptr %47)
  br label %48

48:
  %49 = load i32, ptr %5
  %50 = add i32 %49, 1
  store i32 %50, ptr %5
  br label %8

51:
  ret void
}

define i32 @main() {
0:
  %1 = alloca [16 x i32]
  %2 = call ptr @memcpy(ptr %1, ptr @a, i64 64)
  call void @selection_sort(ptr %1, i32 16)
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
