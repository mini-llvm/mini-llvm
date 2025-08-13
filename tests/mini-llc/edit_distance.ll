@str1 = private constant [7 x i8] c"kitten\00"
@str2 = private constant [8 x i8] c"sitting\00"
@str3 = private constant [10 x i8] c"intention\00"
@str4 = private constant [10 x i8] c"execution\00"
@format = private constant [4 x i8] c"%d\0A\00"

declare ptr @malloc(i64) inaccessiblememonly
declare void @free(ptr) inaccessiblemem_or_argmemonly
declare i32 @printf(ptr, ...)

define i32 @min(i32 %0, i32 %1, i32 %2) {
3:
  %4 = alloca i32
  %5 = alloca i32
  %6 = alloca i32
  %7 = alloca i32
  store i32 %0, ptr %5
  store i32 %1, ptr %6
  store i32 %2, ptr %7
  %8 = load i32, ptr %5
  %9 = load i32, ptr %6
  %10 = icmp slt i32 %8, %9
  br i1 %10, label %11, label %17

11:
  %12 = load i32, ptr %5
  %13 = load i32, ptr %7
  %14 = icmp slt i32 %12, %13
  br i1 %14, label %15, label %17

15:
  %16 = load i32, ptr %5
  store i32 %16, ptr %4
  br label %25

17:
  %18 = load i32, ptr %6
  %19 = load i32, ptr %7
  %20 = icmp slt i32 %18, %19
  br i1 %20, label %21, label %23

21:
  %22 = load i32, ptr %6
  store i32 %22, ptr %4
  br label %25

23:
  %24 = load i32, ptr %7
  store i32 %24, ptr %4
  br label %25

25:
  %26 = load i32, ptr %4
  ret i32 %26
}

define i32 @edit_distance(ptr %0, ptr %1, i32 %2, i32 %3) {
4:
  %5 = alloca ptr
  %6 = alloca ptr
  %7 = alloca i32
  %8 = alloca i32
  %9 = alloca [2 x ptr]
  %10 = alloca i32
  %11 = alloca i32
  %12 = alloca i32
  %13 = alloca i32
  store ptr %0, ptr %5
  store ptr %1, ptr %6
  store i32 %2, ptr %7
  store i32 %3, ptr %8
  %14 = load i32, ptr %8
  %15 = add i32 %14, 1
  %16 = sext i32 %15 to i64
  %17 = mul i64 %16, 4
  %18 = call ptr @malloc(i64 %17)
  %19 = getelementptr [2 x ptr], ptr %9, i64 0, i64 0
  store ptr %18, ptr %19
  %20 = load i32, ptr %8
  %21 = add i32 %20, 1
  %22 = sext i32 %21 to i64
  %23 = mul i64 %22, 4
  %24 = call ptr @malloc(i64 %23)
  %25 = getelementptr [2 x ptr], ptr %9, i64 0, i64 1
  store ptr %24, ptr %25
  store i32 0, ptr %10
  br label %26

26:
  %27 = load i32, ptr %10
  %28 = load i32, ptr %8
  %29 = icmp sle i32 %27, %28
  br i1 %29, label %30, label %40

30:
  %31 = load i32, ptr %10
  %32 = getelementptr [2 x ptr], ptr %9, i64 0, i64 0
  %33 = load ptr, ptr %32
  %34 = load i32, ptr %10
  %35 = sext i32 %34 to i64
  %36 = getelementptr i32, ptr %33, i64 %35
  store i32 %31, ptr %36
  br label %37

37:
  %38 = load i32, ptr %10
  %39 = add i32 %38, 1
  store i32 %39, ptr %10
  br label %26

40:
  store i32 1, ptr %11
  br label %41

41:
  %42 = load i32, ptr %11
  %43 = load i32, ptr %7
  %44 = icmp sle i32 %42, %43
  br i1 %44, label %45, label %143

45:
  %46 = load i32, ptr %11
  %47 = load i32, ptr %11
  %48 = srem i32 %47, 2
  %49 = sext i32 %48 to i64
  %50 = getelementptr [2 x ptr], ptr %9, i64 0, i64 %49
  %51 = load ptr, ptr %50
  %52 = getelementptr i32, ptr %51, i64 0
  store i32 %46, ptr %52
  store i32 1, ptr %12
  br label %53

53:
  %54 = load i32, ptr %12
  %55 = load i32, ptr %8
  %56 = icmp sle i32 %54, %55
  br i1 %56, label %57, label %139

57:
  %58 = load ptr, ptr %5
  %59 = load i32, ptr %11
  %60 = sub i32 %59, 1
  %61 = sext i32 %60 to i64
  %62 = getelementptr i8, ptr %58, i64 %61
  %63 = load i8, ptr %62
  %64 = sext i8 %63 to i32
  %65 = load ptr, ptr %6
  %66 = load i32, ptr %12
  %67 = sub i32 %66, 1
  %68 = sext i32 %67 to i64
  %69 = getelementptr i8, ptr %65, i64 %68
  %70 = load i8, ptr %69
  %71 = sext i8 %70 to i32
  %72 = icmp eq i32 %64, %71
  br i1 %72, label %73, label %93

73:
  %74 = load i32, ptr %11
  %75 = sub i32 %74, 1
  %76 = srem i32 %75, 2
  %77 = sext i32 %76 to i64
  %78 = getelementptr [2 x ptr], ptr %9, i64 0, i64 %77
  %79 = load ptr, ptr %78
  %80 = load i32, ptr %12
  %81 = sub i32 %80, 1
  %82 = sext i32 %81 to i64
  %83 = getelementptr i32, ptr %79, i64 %82
  %84 = load i32, ptr %83
  %85 = load i32, ptr %11
  %86 = srem i32 %85, 2
  %87 = sext i32 %86 to i64
  %88 = getelementptr [2 x ptr], ptr %9, i64 0, i64 %87
  %89 = load ptr, ptr %88
  %90 = load i32, ptr %12
  %91 = sext i32 %90 to i64
  %92 = getelementptr i32, ptr %89, i64 %91
  store i32 %84, ptr %92
  br label %135

93:
  %94 = load i32, ptr %11
  %95 = sub i32 %94, 1
  %96 = srem i32 %95, 2
  %97 = sext i32 %96 to i64
  %98 = getelementptr [2 x ptr], ptr %9, i64 0, i64 %97
  %99 = load ptr, ptr %98
  %100 = load i32, ptr %12
  %101 = sext i32 %100 to i64
  %102 = getelementptr i32, ptr %99, i64 %101
  %103 = load i32, ptr %102
  %104 = load i32, ptr %11
  %105 = srem i32 %104, 2
  %106 = sext i32 %105 to i64
  %107 = getelementptr [2 x ptr], ptr %9, i64 0, i64 %106
  %108 = load ptr, ptr %107
  %109 = load i32, ptr %12
  %110 = sub i32 %109, 1
  %111 = sext i32 %110 to i64
  %112 = getelementptr i32, ptr %108, i64 %111
  %113 = load i32, ptr %112
  %114 = load i32, ptr %11
  %115 = sub i32 %114, 1
  %116 = srem i32 %115, 2
  %117 = sext i32 %116 to i64
  %118 = getelementptr [2 x ptr], ptr %9, i64 0, i64 %117
  %119 = load ptr, ptr %118
  %120 = load i32, ptr %12
  %121 = sub i32 %120, 1
  %122 = sext i32 %121 to i64
  %123 = getelementptr i32, ptr %119, i64 %122
  %124 = load i32, ptr %123
  %125 = call i32 @min(i32 %103, i32 %113, i32 %124)
  %126 = add i32 %125, 1
  %127 = load i32, ptr %11
  %128 = srem i32 %127, 2
  %129 = sext i32 %128 to i64
  %130 = getelementptr [2 x ptr], ptr %9, i64 0, i64 %129
  %131 = load ptr, ptr %130
  %132 = load i32, ptr %12
  %133 = sext i32 %132 to i64
  %134 = getelementptr i32, ptr %131, i64 %133
  store i32 %126, ptr %134
  br label %135

135:
  br label %136

136:
  %137 = load i32, ptr %12
  %138 = add i32 %137, 1
  store i32 %138, ptr %12
  br label %53

139:
  br label %140

140:
  %141 = load i32, ptr %11
  %142 = add i32 %141, 1
  store i32 %142, ptr %11
  br label %41

143:
  %144 = load i32, ptr %7
  %145 = srem i32 %144, 2
  %146 = sext i32 %145 to i64
  %147 = getelementptr [2 x ptr], ptr %9, i64 0, i64 %146
  %148 = load ptr, ptr %147
  %149 = load i32, ptr %8
  %150 = sext i32 %149 to i64
  %151 = getelementptr i32, ptr %148, i64 %150
  %152 = load i32, ptr %151
  store i32 %152, ptr %13
  %153 = getelementptr [2 x ptr], ptr %9, i64 0, i64 0
  %154 = load ptr, ptr %153
  call void @free(ptr %154)
  %155 = getelementptr [2 x ptr], ptr %9, i64 0, i64 1
  %156 = load ptr, ptr %155
  call void @free(ptr %156)
  %157 = load i32, ptr %13
  ret i32 %157
}

define i32 @main() {
0:
  %1 = call i32 @edit_distance(ptr @str1, ptr @str2, i32 6, i32 7)
  %2 = call i32 (ptr, ...) @printf(ptr @format, i32 %1)
  %3 = call i32 @edit_distance(ptr @str3, ptr @str4, i32 9, i32 9)
  %4 = call i32 (ptr, ...) @printf(ptr @format, i32 %3)
  ret i32 0
}
