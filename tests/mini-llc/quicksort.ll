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

define i32 @partition(ptr %0, i32 %1, i32 %2) {
3:
  %4 = alloca ptr
  %5 = alloca i32
  %6 = alloca i32
  %7 = alloca i32
  %8 = alloca i32
  store ptr %0, ptr %4
  store i32 %1, ptr %5
  store i32 %2, ptr %6
  %9 = load i32, ptr %5
  %10 = sub i32 %9, 1
  store i32 %10, ptr %7
  %11 = load i32, ptr %6
  store i32 %11, ptr %8
  br label %12

12:
  br label %13

13:
  %14 = load i32, ptr %7
  %15 = add i32 %14, 1
  store i32 %15, ptr %7
  br label %16

16:
  %17 = load ptr, ptr %4
  %18 = load i32, ptr %7
  %19 = sext i32 %18 to i64
  %20 = getelementptr i32, ptr %17, i64 %19
  %21 = load i32, ptr %20
  %22 = load ptr, ptr %4
  %23 = load i32, ptr %6
  %24 = sext i32 %23 to i64
  %25 = getelementptr i32, ptr %22, i64 %24
  %26 = load i32, ptr %25
  %27 = icmp slt i32 %21, %26
  br i1 %27, label %13, label %28

28:
  br label %29

29:
  %30 = load i32, ptr %8
  %31 = add i32 %30, -1
  store i32 %31, ptr %8
  br label %32

32:
  %33 = load i32, ptr %8
  %34 = load i32, ptr %5
  %35 = icmp sge i32 %33, %34
  br i1 %35, label %36, label %48

36:
  %37 = load ptr, ptr %4
  %38 = load i32, ptr %8
  %39 = sext i32 %38 to i64
  %40 = getelementptr i32, ptr %37, i64 %39
  %41 = load i32, ptr %40
  %42 = load ptr, ptr %4
  %43 = load i32, ptr %6
  %44 = sext i32 %43 to i64
  %45 = getelementptr i32, ptr %42, i64 %44
  %46 = load i32, ptr %45
  %47 = icmp sgt i32 %41, %46
  br label %48

48:
  %49 = phi i1 [ false, %32 ], [ %47, %36 ]
  br i1 %49, label %29, label %50

50:
  %51 = load i32, ptr %7
  %52 = load i32, ptr %8
  %53 = icmp slt i32 %51, %52
  br i1 %53, label %54, label %63

54:
  %55 = load ptr, ptr %4
  %56 = load i32, ptr %7
  %57 = sext i32 %56 to i64
  %58 = getelementptr i32, ptr %55, i64 %57
  %59 = load ptr, ptr %4
  %60 = load i32, ptr %8
  %61 = sext i32 %60 to i64
  %62 = getelementptr i32, ptr %59, i64 %61
  call void @swap(ptr %58, ptr %62)
  br label %64

63:
  br label %65

64:
  br label %12

65:
  %66 = load ptr, ptr %4
  %67 = load i32, ptr %7
  %68 = sext i32 %67 to i64
  %69 = getelementptr i32, ptr %66, i64 %68
  %70 = load ptr, ptr %4
  %71 = load i32, ptr %6
  %72 = sext i32 %71 to i64
  %73 = getelementptr i32, ptr %70, i64 %72
  call void @swap(ptr %69, ptr %73)
  %74 = load i32, ptr %7
  ret i32 %74
}

define void @quicksort(ptr %0, i32 %1, i32 %2) {
3:
  %4 = alloca ptr
  %5 = alloca i32
  %6 = alloca i32
  %7 = alloca i32
  store ptr %0, ptr %4
  store i32 %1, ptr %5
  store i32 %2, ptr %6
  %8 = load i32, ptr %5
  %9 = load i32, ptr %6
  %10 = icmp slt i32 %8, %9
  br i1 %10, label %11, label %24

11:
  %12 = load ptr, ptr %4
  %13 = load i32, ptr %5
  %14 = load i32, ptr %6
  %15 = call i32 @partition(ptr %12, i32 %13, i32 %14)
  store i32 %15, ptr %7
  %16 = load ptr, ptr %4
  %17 = load i32, ptr %5
  %18 = load i32, ptr %7
  %19 = sub i32 %18, 1
  call void @quicksort(ptr %16, i32 %17, i32 %19)
  %20 = load ptr, ptr %4
  %21 = load i32, ptr %7
  %22 = add i32 %21, 1
  %23 = load i32, ptr %6
  call void @quicksort(ptr %20, i32 %22, i32 %23)
  br label %24

24:
  ret void
}

define i32 @main() {
0:
  %1 = alloca [16 x i32]
  %2 = call ptr @memcpy(ptr %1, ptr @a, i64 64)
  call void @quicksort(ptr %1, i32 0, i32 15)
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
