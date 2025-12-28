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

define void @percolate_down(ptr %0, i32 %1, i32 %2) {
3:
  %4 = alloca ptr
  %5 = alloca i32
  %6 = alloca i32
  %7 = alloca i32
  %8 = alloca i32
  store ptr %0, ptr %4
  store i32 %1, ptr %5
  store i32 %2, ptr %6
  %9 = load ptr, ptr %4
  %10 = load i32, ptr %6
  %11 = sext i32 %10 to i64
  %12 = getelementptr i32, ptr %9, i64 %11
  %13 = load i32, ptr %12
  store i32 %13, ptr %7
  br label %14

14:
  %15 = load i32, ptr %6
  %16 = load i32, ptr %5
  %17 = sdiv i32 %16, 2
  %18 = icmp slt i32 %15, %17
  br i1 %18, label %19, label %63

19:
  %20 = load i32, ptr %6
  %21 = mul i32 %20, 2
  %22 = add i32 %21, 1
  store i32 %22, ptr %8
  %23 = load i32, ptr %8
  %24 = add i32 %23, 1
  %25 = load i32, ptr %5
  %26 = icmp slt i32 %24, %25
  br i1 %26, label %27, label %43

27:
  %28 = load ptr, ptr %4
  %29 = load i32, ptr %8
  %30 = add i32 %29, 1
  %31 = sext i32 %30 to i64
  %32 = getelementptr i32, ptr %28, i64 %31
  %33 = load i32, ptr %32
  %34 = load ptr, ptr %4
  %35 = load i32, ptr %8
  %36 = sext i32 %35 to i64
  %37 = getelementptr i32, ptr %34, i64 %36
  %38 = load i32, ptr %37
  %39 = icmp sgt i32 %33, %38
  br i1 %39, label %40, label %43

40:
  %41 = load i32, ptr %8
  %42 = add i32 %41, 1
  store i32 %42, ptr %8
  br label %43

43:
  %44 = load i32, ptr %7
  %45 = load ptr, ptr %4
  %46 = load i32, ptr %8
  %47 = sext i32 %46 to i64
  %48 = getelementptr i32, ptr %45, i64 %47
  %49 = load i32, ptr %48
  %50 = icmp sge i32 %44, %49
  br i1 %50, label %51, label %52

51:
  br label %63

52:
  %53 = load ptr, ptr %4
  %54 = load i32, ptr %8
  %55 = sext i32 %54 to i64
  %56 = getelementptr i32, ptr %53, i64 %55
  %57 = load i32, ptr %56
  %58 = load ptr, ptr %4
  %59 = load i32, ptr %6
  %60 = sext i32 %59 to i64
  %61 = getelementptr i32, ptr %58, i64 %60
  store i32 %57, ptr %61
  %62 = load i32, ptr %8
  store i32 %62, ptr %6
  br label %14

63:
  %64 = load i32, ptr %7
  %65 = load ptr, ptr %4
  %66 = load i32, ptr %6
  %67 = sext i32 %66 to i64
  %68 = getelementptr i32, ptr %65, i64 %67
  store i32 %64, ptr %68
  ret void
}

define void @heapsort(ptr %0, i32 %1) {
2:
  %3 = alloca ptr
  %4 = alloca i32
  %5 = alloca i32
  %6 = alloca i32
  store ptr %0, ptr %3
  store i32 %1, ptr %4
  %7 = load i32, ptr %4
  %8 = sdiv i32 %7, 2
  %9 = sub i32 %8, 1
  store i32 %9, ptr %5
  br label %10

10:
  %11 = load i32, ptr %5
  %12 = icmp sge i32 %11, 0
  br i1 %12, label %13, label %20

13:
  %14 = load ptr, ptr %3
  %15 = load i32, ptr %4
  %16 = load i32, ptr %5
  call void @percolate_down(ptr %14, i32 %15, i32 %16)
  br label %17

17:
  %18 = load i32, ptr %5
  %19 = add i32 %18, -1
  store i32 %19, ptr %5
  br label %10

20:
  %21 = load i32, ptr %4
  %22 = sub i32 %21, 1
  store i32 %22, ptr %6
  br label %23

23:
  %24 = load i32, ptr %6
  %25 = icmp sge i32 %24, 1
  br i1 %25, label %26, label %38

26:
  %27 = load ptr, ptr %3
  %28 = getelementptr i32, ptr %27, i64 0
  %29 = load ptr, ptr %3
  %30 = load i32, ptr %6
  %31 = sext i32 %30 to i64
  %32 = getelementptr i32, ptr %29, i64 %31
  call void @swap(ptr %28, ptr %32)
  %33 = load ptr, ptr %3
  %34 = load i32, ptr %6
  call void @percolate_down(ptr %33, i32 %34, i32 0)
  br label %35

35:
  %36 = load i32, ptr %6
  %37 = add i32 %36, -1
  store i32 %37, ptr %6
  br label %23

38:
  ret void
}

define i32 @main() {
0:
  %1 = alloca [16 x i32]
  %2 = call ptr @memcpy(ptr %1, ptr @a, i64 64)
  call void @heapsort(ptr %1, i32 16)
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
