declare i32 @putchar(i32)
declare ptr @memset(ptr, i32, i64) argmemonly

define void @print_board(i32 %0, ptr %1) {
2:
  %3 = alloca i32
  %4 = alloca ptr
  %5 = alloca i32
  %6 = alloca i32
  store i32 %0, ptr %3
  store ptr %1, ptr %4
  store i32 0, ptr %5
  br label %7

7:
  %8 = load i32, ptr %5
  %9 = load i32, ptr %3
  %10 = icmp slt i32 %8, %9
  br i1 %10, label %11, label %37

11:
  store i32 0, ptr %6
  br label %12

12:
  %13 = load i32, ptr %6
  %14 = load i32, ptr %3
  %15 = icmp slt i32 %13, %14
  br i1 %15, label %16, label %32

16:
  %17 = load ptr, ptr %4
  %18 = load i32, ptr %5
  %19 = sext i32 %18 to i64
  %20 = getelementptr i32, ptr %17, i64 %19
  %21 = load i32, ptr %20
  %22 = load i32, ptr %6
  %23 = icmp eq i32 %21, %22
  br i1 %23, label %24, label %26

24:
  %25 = call i32 @putchar(i32 81)
  br label %28

26:
  %27 = call i32 @putchar(i32 46)
  br label %28

28:
  br label %29

29:
  %30 = load i32, ptr %6
  %31 = add i32 %30, 1
  store i32 %31, ptr %6
  br label %12

32:
  %33 = call i32 @putchar(i32 10)
  br label %34

34:
  %35 = load i32, ptr %5
  %36 = add i32 %35, 1
  store i32 %36, ptr %5
  br label %7

37:
  %38 = call i32 @putchar(i32 10)
  ret void
}

define void @n_queens(i32 %0, i32 %1, ptr %2, ptr %3, ptr %4, ptr %5) {
6:
  %7 = alloca i32
  %8 = alloca i32
  %9 = alloca ptr
  %10 = alloca ptr
  %11 = alloca ptr
  %12 = alloca ptr
  %13 = alloca i32
  store i32 %0, ptr %7
  store i32 %1, ptr %8
  store ptr %2, ptr %9
  store ptr %3, ptr %10
  store ptr %4, ptr %11
  store ptr %5, ptr %12
  %14 = load i32, ptr %8
  %15 = load i32, ptr %7
  %16 = icmp eq i32 %14, %15
  br i1 %16, label %17, label %20

17:
  %18 = load i32, ptr %7
  %19 = load ptr, ptr %9
  call void @print_board(i32 %18, ptr %19)
  br label %108

20:
  store i32 0, ptr %13
  br label %21

21:
  %22 = load i32, ptr %13
  %23 = load i32, ptr %7
  %24 = icmp slt i32 %22, %23
  br i1 %24, label %25, label %108

25:
  %26 = load ptr, ptr %10
  %27 = load i32, ptr %13
  %28 = sext i32 %27 to i64
  %29 = getelementptr i32, ptr %26, i64 %28
  %30 = load i32, ptr %29
  %31 = icmp ne i32 %30, 0
  br i1 %31, label %104, label %32

32:
  %33 = load ptr, ptr %11
  %34 = load i32, ptr %8
  %35 = load i32, ptr %13
  %36 = sub i32 %34, %35
  %37 = load i32, ptr %7
  %38 = add i32 %36, %37
  %39 = sub i32 %38, 1
  %40 = sext i32 %39 to i64
  %41 = getelementptr i32, ptr %33, i64 %40
  %42 = load i32, ptr %41
  %43 = icmp ne i32 %42, 0
  br i1 %43, label %104, label %44

44:
  %45 = load ptr, ptr %12
  %46 = load i32, ptr %8
  %47 = load i32, ptr %13
  %48 = add i32 %46, %47
  %49 = sext i32 %48 to i64
  %50 = getelementptr i32, ptr %45, i64 %49
  %51 = load i32, ptr %50
  %52 = icmp ne i32 %51, 0
  br i1 %52, label %104, label %53

53:
  %54 = load i32, ptr %13
  %55 = load ptr, ptr %9
  %56 = load i32, ptr %8
  %57 = sext i32 %56 to i64
  %58 = getelementptr i32, ptr %55, i64 %57
  store i32 %54, ptr %58
  %59 = load ptr, ptr %10
  %60 = load i32, ptr %13
  %61 = sext i32 %60 to i64
  %62 = getelementptr i32, ptr %59, i64 %61
  store i32 1, ptr %62
  %63 = load ptr, ptr %11
  %64 = load i32, ptr %8
  %65 = load i32, ptr %13
  %66 = sub i32 %64, %65
  %67 = load i32, ptr %7
  %68 = add i32 %66, %67
  %69 = sub i32 %68, 1
  %70 = sext i32 %69 to i64
  %71 = getelementptr i32, ptr %63, i64 %70
  store i32 1, ptr %71
  %72 = load ptr, ptr %12
  %73 = load i32, ptr %8
  %74 = load i32, ptr %13
  %75 = add i32 %73, %74
  %76 = sext i32 %75 to i64
  %77 = getelementptr i32, ptr %72, i64 %76
  store i32 1, ptr %77
  %78 = load i32, ptr %7
  %79 = load i32, ptr %8
  %80 = add i32 %79, 1
  %81 = load ptr, ptr %9
  %82 = load ptr, ptr %10
  %83 = load ptr, ptr %11
  %84 = load ptr, ptr %12
  call void @n_queens(i32 %78, i32 %80, ptr %81, ptr %82, ptr %83, ptr %84)
  %85 = load ptr, ptr %10
  %86 = load i32, ptr %13
  %87 = sext i32 %86 to i64
  %88 = getelementptr i32, ptr %85, i64 %87
  store i32 0, ptr %88
  %89 = load ptr, ptr %11
  %90 = load i32, ptr %8
  %91 = load i32, ptr %13
  %92 = sub i32 %90, %91
  %93 = load i32, ptr %7
  %94 = add i32 %92, %93
  %95 = sub i32 %94, 1
  %96 = sext i32 %95 to i64
  %97 = getelementptr i32, ptr %89, i64 %96
  store i32 0, ptr %97
  %98 = load ptr, ptr %12
  %99 = load i32, ptr %8
  %100 = load i32, ptr %13
  %101 = add i32 %99, %100
  %102 = sext i32 %101 to i64
  %103 = getelementptr i32, ptr %98, i64 %102
  store i32 0, ptr %103
  br label %104

104:
  br label %105

105:
  %106 = load i32, ptr %13
  %107 = add i32 %106, 1
  store i32 %107, ptr %13
  br label %21

108:
  ret void
}

define i32 @main() {
0:
  %1 = alloca [8 x i32]
  %2 = alloca [8 x i32]
  %3 = alloca [15 x i32]
  %4 = alloca [15 x i32]
  %5 = call ptr @memset(ptr %2, i32 0, i64 32)
  %6 = call ptr @memset(ptr %3, i32 0, i64 60)
  %7 = call ptr @memset(ptr %4, i32 0, i64 60)
  call void @n_queens(i32 8, i32 0, ptr %1, ptr %2, ptr %3, ptr %4)
  ret i32 0
}
