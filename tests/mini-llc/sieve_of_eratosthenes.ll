@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define void @sieve_of_eratosthenes(i32 %0, ptr %1) {
2:
  %3 = alloca i32
  %4 = alloca ptr
  %5 = alloca i32
  %6 = alloca i32
  %7 = alloca i32
  store i32 %0, ptr %3
  store ptr %1, ptr %4
  %8 = load ptr, ptr %4
  %9 = getelementptr i32, ptr %8, i64 0
  store i32 0, ptr %9
  %10 = load ptr, ptr %4
  %11 = getelementptr i32, ptr %10, i64 1
  store i32 0, ptr %11
  store i32 2, ptr %5
  br label %12

12:
  %13 = load i32, ptr %5
  %14 = load i32, ptr %3
  %15 = icmp sle i32 %13, %14
  br i1 %15, label %16, label %24

16:
  %17 = load ptr, ptr %4
  %18 = load i32, ptr %5
  %19 = sext i32 %18 to i64
  %20 = getelementptr i32, ptr %17, i64 %19
  store i32 1, ptr %20
  br label %21

21:
  %22 = load i32, ptr %5
  %23 = add i32 %22, 1
  store i32 %23, ptr %5
  br label %12

24:
  store i32 2, ptr %6
  br label %25

25:
  %26 = load i32, ptr %6
  %27 = load i32, ptr %6
  %28 = mul i32 %26, %27
  %29 = load i32, ptr %3
  %30 = icmp sle i32 %28, %29
  br i1 %30, label %31, label %60

31:
  %32 = load ptr, ptr %4
  %33 = load i32, ptr %6
  %34 = sext i32 %33 to i64
  %35 = getelementptr i32, ptr %32, i64 %34
  %36 = load i32, ptr %35
  %37 = icmp ne i32 %36, 0
  br i1 %37, label %38, label %56

38:
  %39 = load i32, ptr %6
  %40 = load i32, ptr %6
  %41 = mul i32 %39, %40
  store i32 %41, ptr %7
  br label %42

42:
  %43 = load i32, ptr %7
  %44 = load i32, ptr %3
  %45 = icmp sle i32 %43, %44
  br i1 %45, label %46, label %55

46:
  %47 = load ptr, ptr %4
  %48 = load i32, ptr %7
  %49 = sext i32 %48 to i64
  %50 = getelementptr i32, ptr %47, i64 %49
  store i32 0, ptr %50
  br label %51

51:
  %52 = load i32, ptr %6
  %53 = load i32, ptr %7
  %54 = add i32 %53, %52
  store i32 %54, ptr %7
  br label %42

55:
  br label %56

56:
  br label %57

57:
  %58 = load i32, ptr %6
  %59 = add i32 %58, 1
  store i32 %59, ptr %6
  br label %25

60:
  ret void
}

define i32 @main() {
0:
  %1 = alloca [21 x i32]
  call void @sieve_of_eratosthenes(i32 20, ptr %1)
  %2 = getelementptr [21 x i32], ptr %1, i64 0, i64 1
  %3 = getelementptr [21 x i32], ptr %1, i64 0, i64 2
  %4 = getelementptr [21 x i32], ptr %1, i64 0, i64 3
  %5 = getelementptr [21 x i32], ptr %1, i64 0, i64 4
  %6 = getelementptr [21 x i32], ptr %1, i64 0, i64 5
  %7 = getelementptr [21 x i32], ptr %1, i64 0, i64 6
  %8 = getelementptr [21 x i32], ptr %1, i64 0, i64 7
  %9 = getelementptr [21 x i32], ptr %1, i64 0, i64 8
  %10 = getelementptr [21 x i32], ptr %1, i64 0, i64 9
  %11 = getelementptr [21 x i32], ptr %1, i64 0, i64 10
  %12 = getelementptr [21 x i32], ptr %1, i64 0, i64 11
  %13 = getelementptr [21 x i32], ptr %1, i64 0, i64 12
  %14 = getelementptr [21 x i32], ptr %1, i64 0, i64 13
  %15 = getelementptr [21 x i32], ptr %1, i64 0, i64 14
  %16 = getelementptr [21 x i32], ptr %1, i64 0, i64 15
  %17 = getelementptr [21 x i32], ptr %1, i64 0, i64 16
  %18 = getelementptr [21 x i32], ptr %1, i64 0, i64 17
  %19 = getelementptr [21 x i32], ptr %1, i64 0, i64 18
  %20 = getelementptr [21 x i32], ptr %1, i64 0, i64 19
  %21 = getelementptr [21 x i32], ptr %1, i64 0, i64 20
  %22 = load i32, ptr %2
  %23 = call i32 (ptr, ...) @printf(ptr @format, i32 %22)
  %24 = load i32, ptr %3
  %25 = call i32 (ptr, ...) @printf(ptr @format, i32 %24)
  %26 = load i32, ptr %4
  %27 = call i32 (ptr, ...) @printf(ptr @format, i32 %26)
  %28 = load i32, ptr %5
  %29 = call i32 (ptr, ...) @printf(ptr @format, i32 %28)
  %30 = load i32, ptr %6
  %31 = call i32 (ptr, ...) @printf(ptr @format, i32 %30)
  %32 = load i32, ptr %7
  %33 = call i32 (ptr, ...) @printf(ptr @format, i32 %32)
  %34 = load i32, ptr %8
  %35 = call i32 (ptr, ...) @printf(ptr @format, i32 %34)
  %36 = load i32, ptr %9
  %37 = call i32 (ptr, ...) @printf(ptr @format, i32 %36)
  %38 = load i32, ptr %10
  %39 = call i32 (ptr, ...) @printf(ptr @format, i32 %38)
  %40 = load i32, ptr %11
  %41 = call i32 (ptr, ...) @printf(ptr @format, i32 %40)
  %42 = load i32, ptr %12
  %43 = call i32 (ptr, ...) @printf(ptr @format, i32 %42)
  %44 = load i32, ptr %13
  %45 = call i32 (ptr, ...) @printf(ptr @format, i32 %44)
  %46 = load i32, ptr %14
  %47 = call i32 (ptr, ...) @printf(ptr @format, i32 %46)
  %48 = load i32, ptr %15
  %49 = call i32 (ptr, ...) @printf(ptr @format, i32 %48)
  %50 = load i32, ptr %16
  %51 = call i32 (ptr, ...) @printf(ptr @format, i32 %50)
  %52 = load i32, ptr %17
  %53 = call i32 (ptr, ...) @printf(ptr @format, i32 %52)
  %54 = load i32, ptr %18
  %55 = call i32 (ptr, ...) @printf(ptr @format, i32 %54)
  %56 = load i32, ptr %19
  %57 = call i32 (ptr, ...) @printf(ptr @format, i32 %56)
  %58 = load i32, ptr %20
  %59 = call i32 (ptr, ...) @printf(ptr @format, i32 %58)
  %60 = load i32, ptr %21
  %61 = call i32 (ptr, ...) @printf(ptr @format, i32 %60)
  ret i32 0
}
