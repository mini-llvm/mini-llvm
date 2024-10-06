@__a = internal global [8 x i32] [i32 5, i32 4, i32 1, i32 3, i32 8, i32 7, i32 6, i32 2]
@__format = internal global [25 x i8] c"%d %d %d %d %d %d %d %d\0A\00"

declare ptr @memcpy(ptr, ptr, i64)
declare i32 @printf(ptr, ...)

define void @mergesort(ptr %0, ptr %1, i32 %2, i32 %3) {
4:
  %5 = alloca ptr
  %6 = alloca ptr
  %7 = alloca i32
  %8 = alloca i32
  %9 = alloca i32
  %10 = alloca i32
  %11 = alloca i32
  %12 = alloca i32
  store ptr %0, ptr %5
  store ptr %1, ptr %6
  store i32 %2, ptr %7
  store i32 %3, ptr %8
  %13 = load i32, ptr %7
  %14 = load i32, ptr %8
  %15 = icmp eq i32 %13, %14
  br i1 %15, label %16, label %17

16:
  br label %135

17:
  %18 = load i32, ptr %7
  %19 = load i32, ptr %8
  %20 = add i32 %18, %19
  %21 = sdiv i32 %20, 2
  store i32 %21, ptr %9
  %22 = load ptr, ptr %5
  %23 = load ptr, ptr %6
  %24 = load i32, ptr %7
  %25 = load i32, ptr %9
  call void @mergesort(ptr %22, ptr %23, i32 %24, i32 %25)
  %26 = load ptr, ptr %5
  %27 = load ptr, ptr %6
  %28 = load i32, ptr %9
  %29 = add i32 %28, 1
  %30 = load i32, ptr %8
  call void @mergesort(ptr %26, ptr %27, i32 %29, i32 %30)
  %31 = load i32, ptr %7
  store i32 %31, ptr %10
  %32 = load i32, ptr %9
  %33 = add i32 %32, 1
  store i32 %33, ptr %11
  %34 = load i32, ptr %7
  store i32 %34, ptr %12
  br label %35

35:
  %36 = load i32, ptr %10
  %37 = load i32, ptr %9
  %38 = icmp sle i32 %36, %37
  br i1 %38, label %39, label %43

39:
  %40 = load i32, ptr %11
  %41 = load i32, ptr %8
  %42 = icmp sle i32 %40, %41
  br label %43

43:
  %44 = phi i1 [ false, %35 ], [ %42, %39 ]
  br i1 %44, label %45, label %82

45:
  %46 = load ptr, ptr %5
  %47 = load i32, ptr %10
  %48 = sext i32 %47 to i64
  %49 = getelementptr i32, ptr %46, i64 %48
  %50 = load i32, ptr %49
  %51 = load ptr, ptr %5
  %52 = load i32, ptr %11
  %53 = sext i32 %52 to i64
  %54 = getelementptr i32, ptr %51, i64 %53
  %55 = load i32, ptr %54
  %56 = icmp sle i32 %50, %55
  br i1 %56, label %57, label %69

57:
  %58 = load ptr, ptr %5
  %59 = load i32, ptr %10
  %60 = add i32 %59, 1
  store i32 %60, ptr %10
  %61 = sext i32 %59 to i64
  %62 = getelementptr i32, ptr %58, i64 %61
  %63 = load i32, ptr %62
  %64 = load ptr, ptr %6
  %65 = load i32, ptr %12
  %66 = add i32 %65, 1
  store i32 %66, ptr %12
  %67 = sext i32 %65 to i64
  %68 = getelementptr i32, ptr %64, i64 %67
  store i32 %63, ptr %68
  br label %81

69:
  %70 = load ptr, ptr %5
  %71 = load i32, ptr %11
  %72 = add i32 %71, 1
  store i32 %72, ptr %11
  %73 = sext i32 %71 to i64
  %74 = getelementptr i32, ptr %70, i64 %73
  %75 = load i32, ptr %74
  %76 = load ptr, ptr %6
  %77 = load i32, ptr %12
  %78 = add i32 %77, 1
  store i32 %78, ptr %12
  %79 = sext i32 %77 to i64
  %80 = getelementptr i32, ptr %76, i64 %79
  store i32 %75, ptr %80
  br label %81

81:
  br label %35

82:
  br label %83

83:
  %84 = load i32, ptr %10
  %85 = load i32, ptr %9
  %86 = icmp sle i32 %84, %85
  br i1 %86, label %87, label %99

87:
  %88 = load ptr, ptr %5
  %89 = load i32, ptr %10
  %90 = add i32 %89, 1
  store i32 %90, ptr %10
  %91 = sext i32 %89 to i64
  %92 = getelementptr i32, ptr %88, i64 %91
  %93 = load i32, ptr %92
  %94 = load ptr, ptr %6
  %95 = load i32, ptr %12
  %96 = add i32 %95, 1
  store i32 %96, ptr %12
  %97 = sext i32 %95 to i64
  %98 = getelementptr i32, ptr %94, i64 %97
  store i32 %93, ptr %98
  br label %83

99:
  br label %100

100:
  %101 = load i32, ptr %11
  %102 = load i32, ptr %8
  %103 = icmp sle i32 %101, %102
  br i1 %103, label %104, label %116

104:
  %105 = load ptr, ptr %5
  %106 = load i32, ptr %11
  %107 = add i32 %106, 1
  store i32 %107, ptr %11
  %108 = sext i32 %106 to i64
  %109 = getelementptr i32, ptr %105, i64 %108
  %110 = load i32, ptr %109
  %111 = load ptr, ptr %6
  %112 = load i32, ptr %12
  %113 = add i32 %112, 1
  store i32 %113, ptr %12
  %114 = sext i32 %112 to i64
  %115 = getelementptr i32, ptr %111, i64 %114
  store i32 %110, ptr %115
  br label %100

116:
  %117 = load i32, ptr %7
  store i32 %117, ptr %12
  br label %118

118:
  %119 = load i32, ptr %12
  %120 = load i32, ptr %8
  %121 = icmp sle i32 %119, %120
  br i1 %121, label %122, label %135

122:
  %123 = load ptr, ptr %6
  %124 = load i32, ptr %12
  %125 = sext i32 %124 to i64
  %126 = getelementptr i32, ptr %123, i64 %125
  %127 = load i32, ptr %126
  %128 = load ptr, ptr %5
  %129 = load i32, ptr %12
  %130 = sext i32 %129 to i64
  %131 = getelementptr i32, ptr %128, i64 %130
  store i32 %127, ptr %131
  br label %132

132:
  %133 = load i32, ptr %12
  %134 = add i32 %133, 1
  store i32 %134, ptr %12
  br label %118

135:
  ret void
}

define i32 @main() {
0:
  %1 = alloca [8 x i32]
  %2 = alloca [8 x i32]
  %3 = call ptr @memcpy(ptr %1, ptr @__a, i64 32)
  call void @mergesort(ptr %1, ptr %2, i32 0, i32 7)
  %4 = getelementptr [8 x i32], ptr %1, i64 0, i64 0
  %5 = getelementptr [8 x i32], ptr %1, i64 0, i64 1
  %6 = getelementptr [8 x i32], ptr %1, i64 0, i64 2
  %7 = getelementptr [8 x i32], ptr %1, i64 0, i64 3
  %8 = getelementptr [8 x i32], ptr %1, i64 0, i64 4
  %9 = getelementptr [8 x i32], ptr %1, i64 0, i64 5
  %10 = getelementptr [8 x i32], ptr %1, i64 0, i64 6
  %11 = getelementptr [8 x i32], ptr %1, i64 0, i64 7
  %12 = load i32, ptr %4
  %13 = load i32, ptr %5
  %14 = load i32, ptr %6
  %15 = load i32, ptr %7
  %16 = load i32, ptr %8
  %17 = load i32, ptr %9
  %18 = load i32, ptr %10
  %19 = load i32, ptr %11
  %20 = call i32 @printf(ptr @__format, i32 %12, i32 %13, i32 %14, i32 %15, i32 %16, i32 %17, i32 %18, i32 %19)
  ret i32 0
}
