@dist = private constant [25 x i32] [
  i32 0, i32 3, i32 8, i32 999, i32 -4,
  i32 999, i32 0, i32 999, i32 1, i32 7,
  i32 999, i32 4, i32 0, i32 999, i32 999,
  i32 2, i32 999, i32 -5, i32 0, i32 999,
  i32 999, i32 999, i32 999, i32 6, i32 0
]

@format = private constant [16 x i8] c"%d %d %d %d %d\0A\00"

declare ptr @memcpy(ptr, ptr, i64) argmemonly
declare i32 @printf(ptr, ...)

define i32 @min(i32 %0, i32 %1) {
2:
  %3 = alloca i32
  %4 = alloca i32
  store i32 %0, ptr %3
  store i32 %1, ptr %4
  %5 = load i32, ptr %3
  %6 = load i32, ptr %4
  %7 = icmp slt i32 %5, %6
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

define void @floyd_warshall(ptr %0, i32 %1) {
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
  %11 = icmp slt i32 %9, %10
  br i1 %11, label %12, label %71

12:
  store i32 0, ptr %6
  br label %13

13:
  %14 = load i32, ptr %6
  %15 = load i32, ptr %4
  %16 = icmp slt i32 %14, %15
  br i1 %16, label %17, label %67

17:
  store i32 0, ptr %7
  br label %18

18:
  %19 = load i32, ptr %7
  %20 = load i32, ptr %4
  %21 = icmp slt i32 %19, %20
  br i1 %21, label %22, label %63

22:
  %23 = load ptr, ptr %3
  %24 = load i32, ptr %6
  %25 = load i32, ptr %4
  %26 = mul i32 %24, %25
  %27 = load i32, ptr %7
  %28 = add i32 %26, %27
  %29 = sext i32 %28 to i64
  %30 = getelementptr i32, ptr %23, i64 %29
  %31 = load i32, ptr %30
  %32 = load ptr, ptr %3
  %33 = load i32, ptr %6
  %34 = load i32, ptr %4
  %35 = mul i32 %33, %34
  %36 = load i32, ptr %5
  %37 = add i32 %35, %36
  %38 = sext i32 %37 to i64
  %39 = getelementptr i32, ptr %32, i64 %38
  %40 = load i32, ptr %39
  %41 = load ptr, ptr %3
  %42 = load i32, ptr %5
  %43 = load i32, ptr %4
  %44 = mul i32 %42, %43
  %45 = load i32, ptr %7
  %46 = add i32 %44, %45
  %47 = sext i32 %46 to i64
  %48 = getelementptr i32, ptr %41, i64 %47
  %49 = load i32, ptr %48
  %50 = add i32 %40, %49
  %51 = call i32 @min(i32 %31, i32 %50)
  %52 = load ptr, ptr %3
  %53 = load i32, ptr %6
  %54 = load i32, ptr %4
  %55 = mul i32 %53, %54
  %56 = load i32, ptr %7
  %57 = add i32 %55, %56
  %58 = sext i32 %57 to i64
  %59 = getelementptr i32, ptr %52, i64 %58
  store i32 %51, ptr %59
  br label %60

60:
  %61 = load i32, ptr %7
  %62 = add i32 %61, 1
  store i32 %62, ptr %7
  br label %18

63:
  br label %64

64:
  %65 = load i32, ptr %6
  %66 = add i32 %65, 1
  store i32 %66, ptr %6
  br label %13

67:
  br label %68

68:
  %69 = load i32, ptr %5
  %70 = add i32 %69, 1
  store i32 %70, ptr %5
  br label %8

71:
  ret void
}

define i32 @main() {
0:
  %1 = alloca [25 x i32]
  %2 = call ptr @memcpy(ptr %1, ptr @dist, i64 100)
  %3 = getelementptr [25 x i32], ptr %2, i64 0, i64 0
  call void @floyd_warshall(ptr %3, i32 5)
  %4 = getelementptr [25 x i32], ptr %2, i64 0, i64 0
  %5 = load i32, ptr %4
  %6 = getelementptr [25 x i32], ptr %2, i64 0, i64 1
  %7 = load i32, ptr %6
  %8 = getelementptr [25 x i32], ptr %2, i64 0, i64 2
  %9 = load i32, ptr %8
  %10 = getelementptr [25 x i32], ptr %2, i64 0, i64 3
  %11 = load i32, ptr %10
  %12 = getelementptr [25 x i32], ptr %2, i64 0, i64 4
  %13 = load i32, ptr %12
  %14 = call i32 (ptr, ...) @printf(ptr @format, i32 %5, i32 %7, i32 %9, i32 %11, i32 %13)
  %15 = getelementptr [25 x i32], ptr %2, i64 0, i64 5
  %16 = load i32, ptr %15
  %17 = getelementptr [25 x i32], ptr %2, i64 0, i64 6
  %18 = load i32, ptr %17
  %19 = getelementptr [25 x i32], ptr %2, i64 0, i64 7
  %20 = load i32, ptr %19
  %21 = getelementptr [25 x i32], ptr %2, i64 0, i64 8
  %22 = load i32, ptr %21
  %23 = getelementptr [25 x i32], ptr %2, i64 0, i64 9
  %24 = load i32, ptr %23
  %25 = call i32 (ptr, ...) @printf(ptr @format, i32 %16, i32 %18, i32 %20, i32 %22, i32 %24)
  %26 = getelementptr [25 x i32], ptr %2, i64 0, i64 10
  %27 = load i32, ptr %26
  %28 = getelementptr [25 x i32], ptr %2, i64 0, i64 11
  %29 = load i32, ptr %28
  %30 = getelementptr [25 x i32], ptr %2, i64 0, i64 12
  %31 = load i32, ptr %30
  %32 = getelementptr [25 x i32], ptr %2, i64 0, i64 13
  %33 = load i32, ptr %32
  %34 = getelementptr [25 x i32], ptr %2, i64 0, i64 14
  %35 = load i32, ptr %34
  %36 = call i32 (ptr, ...) @printf(ptr @format, i32 %27, i32 %29, i32 %31, i32 %33, i32 %35)
  %37 = getelementptr [25 x i32], ptr %2, i64 0, i64 15
  %38 = load i32, ptr %37
  %39 = getelementptr [25 x i32], ptr %2, i64 0, i64 16
  %40 = load i32, ptr %39
  %41 = getelementptr [25 x i32], ptr %2, i64 0, i64 17
  %42 = load i32, ptr %41
  %43 = getelementptr [25 x i32], ptr %2, i64 0, i64 18
  %44 = load i32, ptr %43
  %45 = getelementptr [25 x i32], ptr %2, i64 0, i64 19
  %46 = load i32, ptr %45
  %47 = call i32 (ptr, ...) @printf(ptr @format, i32 %38, i32 %40, i32 %42, i32 %44, i32 %46)
  %48 = getelementptr [25 x i32], ptr %2, i64 0, i64 20
  %49 = load i32, ptr %48
  %50 = getelementptr [25 x i32], ptr %2, i64 0, i64 21
  %51 = load i32, ptr %50
  %52 = getelementptr [25 x i32], ptr %2, i64 0, i64 22
  %53 = load i32, ptr %52
  %54 = getelementptr [25 x i32], ptr %2, i64 0, i64 23
  %55 = load i32, ptr %54
  %56 = getelementptr [25 x i32], ptr %2, i64 0, i64 24
  %57 = load i32, ptr %56
  %58 = call i32 (ptr, ...) @printf(ptr @format, i32 %49, i32 %51, i32 %53, i32 %55, i32 %57)
  ret i32 0
}
