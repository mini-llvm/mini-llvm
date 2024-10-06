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
  store i32 %9, ptr %7
  %10 = load i32, ptr %6
  %11 = sub i32 %10, 1
  store i32 %11, ptr %8
  br label %12

12:
  br label %13

13:
  %14 = load ptr, ptr %4
  %15 = load i32, ptr %7
  %16 = sext i32 %15 to i64
  %17 = getelementptr i32, ptr %14, i64 %16
  %18 = load i32, ptr %17
  %19 = load ptr, ptr %4
  %20 = load i32, ptr %6
  %21 = sext i32 %20 to i64
  %22 = getelementptr i32, ptr %19, i64 %21
  %23 = load i32, ptr %22
  %24 = icmp slt i32 %18, %23
  br i1 %24, label %25, label %28

25:
  %26 = load i32, ptr %7
  %27 = add i32 %26, 1
  store i32 %27, ptr %7
  br label %13

28:
  br label %29

29:
  %30 = load i32, ptr %8
  %31 = load i32, ptr %5
  %32 = icmp sgt i32 %30, %31
  br i1 %32, label %33, label %45

33:
  %34 = load ptr, ptr %4
  %35 = load i32, ptr %8
  %36 = sext i32 %35 to i64
  %37 = getelementptr i32, ptr %34, i64 %36
  %38 = load i32, ptr %37
  %39 = load ptr, ptr %4
  %40 = load i32, ptr %6
  %41 = sext i32 %40 to i64
  %42 = getelementptr i32, ptr %39, i64 %41
  %43 = load i32, ptr %42
  %44 = icmp sgt i32 %38, %43
  br label %45

45:
  %46 = phi i1 [ false, %29 ], [ %44, %33 ]
  br i1 %46, label %47, label %50

47:
  %48 = load i32, ptr %8
  %49 = add i32 %48, -1
  store i32 %49, ptr %8
  br label %29

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
  %1 = alloca [8 x i32]
  %2 = call ptr @memcpy(ptr %1, ptr @__a, i64 32)
  call void @quicksort(ptr %1, i32 0, i32 7)
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
