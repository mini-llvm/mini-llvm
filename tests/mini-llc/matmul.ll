@A = private constant [6 x i32] [i32 3, i32 8, i32 2, i32 0, i32 4, i32 7]
@B = private constant [12 x i32] [i32 6, i32 5, i32 2, i32 3, i32 1, i32 1, i32 7, i32 0, i32 7, i32 0, i32 8, i32 3]
@format = private constant [25 x i8] c"%d %d %d %d %d %d %d %d\0A\00"

declare ptr @memcpy(ptr, ptr, i64) argmemonly
declare i32 @printf(ptr, ...)

define void @matmul(ptr %0, ptr %1, ptr %2, i32 %3, i32 %4, i32 %5) {
6:
  %7 = alloca ptr
  %8 = alloca ptr
  %9 = alloca ptr
  %10 = alloca i32
  %11 = alloca i32
  %12 = alloca i32
  %13 = alloca i32
  %14 = alloca i32
  %15 = alloca i32
  %16 = alloca i32
  store ptr %0, ptr %7
  store ptr %1, ptr %8
  store ptr %2, ptr %9
  store i32 %3, ptr %10
  store i32 %4, ptr %11
  store i32 %5, ptr %12
  store i32 0, ptr %13
  br label %17

17:
  %18 = load i32, ptr %13
  %19 = load i32, ptr %10
  %20 = icmp slt i32 %18, %19
  br i1 %20, label %21, label %73

21:
  store i32 0, ptr %14
  br label %22

22:
  %23 = load i32, ptr %14
  %24 = load i32, ptr %11
  %25 = icmp slt i32 %23, %24
  br i1 %25, label %26, label %69

26:
  store i32 0, ptr %15
  store i32 0, ptr %16
  br label %27

27:
  %28 = load i32, ptr %16
  %29 = load i32, ptr %12
  %30 = icmp slt i32 %28, %29
  br i1 %30, label %31, label %56

31:
  %32 = load ptr, ptr %8
  %33 = load i32, ptr %13
  %34 = load i32, ptr %12
  %35 = mul i32 %33, %34
  %36 = load i32, ptr %16
  %37 = add i32 %35, %36
  %38 = sext i32 %37 to i64
  %39 = getelementptr i32, ptr %32, i64 %38
  %40 = load i32, ptr %39
  %41 = load ptr, ptr %9
  %42 = load i32, ptr %16
  %43 = load i32, ptr %11
  %44 = mul i32 %42, %43
  %45 = load i32, ptr %14
  %46 = add i32 %44, %45
  %47 = sext i32 %46 to i64
  %48 = getelementptr i32, ptr %41, i64 %47
  %49 = load i32, ptr %48
  %50 = mul i32 %40, %49
  %51 = load i32, ptr %15
  %52 = add i32 %51, %50
  store i32 %52, ptr %15
  br label %53

53:
  %54 = load i32, ptr %16
  %55 = add i32 %54, 1
  store i32 %55, ptr %16
  br label %27

56:
  %57 = load i32, ptr %15
  %58 = load ptr, ptr %7
  %59 = load i32, ptr %13
  %60 = load i32, ptr %11
  %61 = mul i32 %59, %60
  %62 = load i32, ptr %14
  %63 = add i32 %61, %62
  %64 = sext i32 %63 to i64
  %65 = getelementptr i32, ptr %58, i64 %64
  store i32 %57, ptr %65
  br label %66

66:
  %67 = load i32, ptr %14
  %68 = add i32 %67, 1
  store i32 %68, ptr %14
  br label %22

69:
  br label %70

70:
  %71 = load i32, ptr %13
  %72 = add i32 %71, 1
  store i32 %72, ptr %13
  br label %17

73:
  ret void
}

define i32 @main() {
0:
  %1 = alloca [6 x i32]
  %2 = alloca [12 x i32]
  %3 = alloca [8 x i32]
  %4 = call ptr @memcpy(ptr %1, ptr @A, i64 24)
  %5 = call ptr @memcpy(ptr %2, ptr @B, i64 48)
  call void @matmul(ptr %3, ptr %1, ptr %2, i32 2, i32 4, i32 3)
  %6 = getelementptr [8 x i32], ptr %3, i64 0, i64 0
  %7 = getelementptr [8 x i32], ptr %3, i64 0, i64 1
  %8 = getelementptr [8 x i32], ptr %3, i64 0, i64 2
  %9 = getelementptr [8 x i32], ptr %3, i64 0, i64 3
  %10 = getelementptr [8 x i32], ptr %3, i64 0, i64 4
  %11 = getelementptr [8 x i32], ptr %3, i64 0, i64 5
  %12 = getelementptr [8 x i32], ptr %3, i64 0, i64 6
  %13 = getelementptr [8 x i32], ptr %3, i64 0, i64 7
  %14 = load i32, ptr %6
  %15 = load i32, ptr %7
  %16 = load i32, ptr %8
  %17 = load i32, ptr %9
  %18 = load i32, ptr %10
  %19 = load i32, ptr %11
  %20 = load i32, ptr %12
  %21 = load i32, ptr %13
  %22 = call i32 (ptr, ...) @printf(ptr @format, i32 %14, i32 %15, i32 %16, i32 %17, i32 %18, i32 %19, i32 %20, i32 %21)
  ret i32 0
}
