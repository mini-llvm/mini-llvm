@n = global i32 0
@m = global i32 0
@adj = global ptr null
@to = global ptr null
@next = global ptr null
@format = private constant [25 x i8] c"%d %d %d %d %d %d %d %d\0A\00"

declare ptr @malloc(i64) inaccessiblememonly
declare void @free(ptr) inaccessiblemem_or_argmemonly
declare ptr @memset(ptr, i32, i64) argmemonly
declare i32 @printf(ptr, ...)

define void @add_edge(i32 %0, i32 %1, i32 %2) {
3:
  %4 = alloca i32
  %5 = alloca i32
  %6 = alloca i32
  store i32 %0, ptr %4
  store i32 %1, ptr %5
  store i32 %2, ptr %6
  %7 = load i32, ptr %6
  %8 = load ptr, ptr @to
  %9 = load i32, ptr %4
  %10 = sext i32 %9 to i64
  %11 = getelementptr i32, ptr %8, i64 %10
  store i32 %7, ptr %11
  %12 = load ptr, ptr @adj
  %13 = load i32, ptr %5
  %14 = sext i32 %13 to i64
  %15 = getelementptr i32, ptr %12, i64 %14
  %16 = load i32, ptr %15
  %17 = load ptr, ptr @next
  %18 = load i32, ptr %4
  %19 = sext i32 %18 to i64
  %20 = getelementptr i32, ptr %17, i64 %19
  store i32 %16, ptr %20
  %21 = load i32, ptr %4
  %22 = load ptr, ptr @adj
  %23 = load i32, ptr %5
  %24 = sext i32 %23 to i64
  %25 = getelementptr i32, ptr %22, i64 %24
  store i32 %21, ptr %25
  ret void
}

define void @topological_sort(ptr %0) {
1:
  %2 = alloca ptr
  %3 = alloca ptr
  %4 = alloca i32
  %5 = alloca i32
  %6 = alloca i32
  %7 = alloca ptr
  %8 = alloca ptr
  %9 = alloca i32
  %10 = alloca i32
  %11 = alloca i32
  %12 = alloca i32
  %13 = alloca i32
  %14 = alloca i32
  %15 = alloca i32
  store ptr %0, ptr %2
  %16 = load i32, ptr @n
  %17 = sext i32 %16 to i64
  %18 = mul i64 %17, 4
  %19 = call ptr @malloc(i64 %18)
  store ptr %19, ptr %3
  %20 = load ptr, ptr %3
  %21 = load i32, ptr @n
  %22 = sext i32 %21 to i64
  %23 = mul i64 %22, 4
  %24 = call ptr @memset(ptr %20, i32 0, i64 %23)
  store i32 0, ptr %4
  br label %25

25:
  %26 = load i32, ptr %4
  %27 = load i32, ptr @n
  %28 = icmp slt i32 %26, %27
  br i1 %28, label %29, label %60

29:
  %30 = load ptr, ptr @adj
  %31 = load i32, ptr %4
  %32 = sext i32 %31 to i64
  %33 = getelementptr i32, ptr %30, i64 %32
  %34 = load i32, ptr %33
  store i32 %34, ptr %5
  br label %35

35:
  %36 = load i32, ptr %5
  %37 = icmp ne i32 %36, -1
  br i1 %37, label %38, label %56

38:
  %39 = load ptr, ptr @to
  %40 = load i32, ptr %5
  %41 = sext i32 %40 to i64
  %42 = getelementptr i32, ptr %39, i64 %41
  %43 = load i32, ptr %42
  store i32 %43, ptr %6
  %44 = load ptr, ptr %3
  %45 = load i32, ptr %6
  %46 = sext i32 %45 to i64
  %47 = getelementptr i32, ptr %44, i64 %46
  %48 = load i32, ptr %47
  %49 = add i32 %48, 1
  store i32 %49, ptr %47
  br label %50

50:
  %51 = load ptr, ptr @next
  %52 = load i32, ptr %5
  %53 = sext i32 %52 to i64
  %54 = getelementptr i32, ptr %51, i64 %53
  %55 = load i32, ptr %54
  store i32 %55, ptr %5
  br label %35

56:
  br label %57

57:
  %58 = load i32, ptr %4
  %59 = add i32 %58, 1
  store i32 %59, ptr %4
  br label %25

60:
  %61 = load i32, ptr @n
  %62 = sext i32 %61 to i64
  %63 = mul i64 %62, 4
  %64 = call ptr @malloc(i64 %63)
  store ptr %64, ptr %7
  %65 = load ptr, ptr %7
  %66 = load i32, ptr @n
  %67 = sext i32 %66 to i64
  %68 = mul i64 %67, 4
  %69 = call ptr @memset(ptr %65, i32 0, i64 %68)
  %70 = load i32, ptr @n
  %71 = sext i32 %70 to i64
  %72 = mul i64 %71, 4
  %73 = call ptr @malloc(i64 %72)
  store ptr %73, ptr %8
  store i32 0, ptr %9
  store i32 0, ptr %10
  store i32 0, ptr %11
  br label %74

74:
  %75 = load i32, ptr %11
  %76 = load i32, ptr @n
  %77 = icmp slt i32 %75, %76
  br i1 %77, label %78, label %96

78:
  %79 = load ptr, ptr %3
  %80 = load i32, ptr %11
  %81 = sext i32 %80 to i64
  %82 = getelementptr i32, ptr %79, i64 %81
  %83 = load i32, ptr %82
  %84 = icmp eq i32 %83, 0
  br i1 %84, label %85, label %92

85:
  %86 = load i32, ptr %11
  %87 = load ptr, ptr %8
  %88 = load i32, ptr %10
  %89 = add i32 %88, 1
  store i32 %89, ptr %10
  %90 = sext i32 %88 to i64
  %91 = getelementptr i32, ptr %87, i64 %90
  store i32 %86, ptr %91
  br label %92

92:
  br label %93

93:
  %94 = load i32, ptr %11
  %95 = add i32 %94, 1
  store i32 %95, ptr %11
  br label %74

96:
  store i32 0, ptr %12
  br label %97

97:
  %98 = load i32, ptr %12
  %99 = load i32, ptr @n
  %100 = icmp slt i32 %98, %99
  br i1 %100, label %101, label %152

101:
  %102 = load ptr, ptr %8
  %103 = load i32, ptr %9
  %104 = add i32 %103, 1
  store i32 %104, ptr %9
  %105 = sext i32 %103 to i64
  %106 = getelementptr i32, ptr %102, i64 %105
  %107 = load i32, ptr %106
  store i32 %107, ptr %13
  %108 = load i32, ptr %13
  %109 = load ptr, ptr %2
  %110 = load i32, ptr %12
  %111 = sext i32 %110 to i64
  %112 = getelementptr i32, ptr %109, i64 %111
  store i32 %108, ptr %112
  %113 = load ptr, ptr @adj
  %114 = load i32, ptr %13
  %115 = sext i32 %114 to i64
  %116 = getelementptr i32, ptr %113, i64 %115
  %117 = load i32, ptr %116
  store i32 %117, ptr %14
  br label %118

118:
  %119 = load i32, ptr %14
  %120 = icmp ne i32 %119, -1
  br i1 %120, label %121, label %148

121:
  %122 = load ptr, ptr @to
  %123 = load i32, ptr %14
  %124 = sext i32 %123 to i64
  %125 = getelementptr i32, ptr %122, i64 %124
  %126 = load i32, ptr %125
  store i32 %126, ptr %15
  %127 = load ptr, ptr %3
  %128 = load i32, ptr %15
  %129 = sext i32 %128 to i64
  %130 = getelementptr i32, ptr %127, i64 %129
  %131 = load i32, ptr %130
  %132 = add i32 %131, -1
  store i32 %132, ptr %130
  %133 = icmp eq i32 %132, 0
  br i1 %133, label %134, label %141

134:
  %135 = load i32, ptr %15
  %136 = load ptr, ptr %8
  %137 = load i32, ptr %10
  %138 = add i32 %137, 1
  store i32 %138, ptr %10
  %139 = sext i32 %137 to i64
  %140 = getelementptr i32, ptr %136, i64 %139
  store i32 %135, ptr %140
  br label %141

141:
  br label %142

142:
  %143 = load ptr, ptr @next
  %144 = load i32, ptr %14
  %145 = sext i32 %144 to i64
  %146 = getelementptr i32, ptr %143, i64 %145
  %147 = load i32, ptr %146
  store i32 %147, ptr %14
  br label %118

148:
  br label %149

149:
  %150 = load i32, ptr %12
  %151 = add i32 %150, 1
  store i32 %151, ptr %12
  br label %97

152:
  %153 = load ptr, ptr %8
  call void @free(ptr %153)
  %154 = load ptr, ptr %7
  call void @free(ptr %154)
  %155 = load ptr, ptr %3
  call void @free(ptr %155)
  ret void
}

define i32 @main() {
0:
  store i32 8, ptr @n
  store i32 9, ptr @m
  %1 = call ptr @malloc(i64 32)
  store ptr %1, ptr @adj
  %2 = call ptr @memset(ptr %1, i32 -1, i64 32)
  %3 = call ptr @malloc(i64 36)
  store ptr %3, ptr @to
  %4 = call ptr @malloc(i64 36)
  store ptr %4, ptr @next
  call void @add_edge(i32 0, i32 0, i32 1)
  call void @add_edge(i32 1, i32 1, i32 2)
  call void @add_edge(i32 2, i32 1, i32 5)
  call void @add_edge(i32 3, i32 1, i32 7)
  call void @add_edge(i32 4, i32 3, i32 1)
  call void @add_edge(i32 5, i32 3, i32 4)
  call void @add_edge(i32 6, i32 4, i32 5)
  call void @add_edge(i32 7, i32 6, i32 4)
  call void @add_edge(i32 8, i32 6, i32 7)
  %5 = call ptr @malloc(i64 32)
  call void @topological_sort(ptr %5)
  %6 = getelementptr i32, ptr %5, i64 0
  %7 = load i32, ptr %6
  %8 = getelementptr i32, ptr %5, i64 1
  %9 = load i32, ptr %8
  %10 = getelementptr i32, ptr %5, i64 2
  %11 = load i32, ptr %10
  %12 = getelementptr i32, ptr %5, i64 3
  %13 = load i32, ptr %12
  %14 = getelementptr i32, ptr %5, i64 4
  %15 = load i32, ptr %14
  %16 = getelementptr i32, ptr %5, i64 5
  %17 = load i32, ptr %16
  %18 = getelementptr i32, ptr %5, i64 6
  %19 = load i32, ptr %18
  %20 = getelementptr i32, ptr %5, i64 7
  %21 = load i32, ptr %20
  %22 = call i32 (ptr, ...) @printf(ptr @format, i32 %7, i32 %9, i32 %11, i32 %13, i32 %15, i32 %17, i32 %19, i32 %21)
  call void @free(ptr %5)
  call void @free(ptr %4)
  call void @free(ptr %3)
  call void @free(ptr %1)
  ret i32 0
}
