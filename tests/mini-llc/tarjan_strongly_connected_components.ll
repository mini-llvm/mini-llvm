@n = global i32 0
@m = global i32 0
@adj = global ptr null
@to = global ptr null
@next = global ptr null
@dfn = global ptr null
@timer = global i32 0
@low = global ptr null
@scc = global ptr null
@scc_cnt = global i32 0
@stack = global ptr null
@top = global i32 0
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

define void @tarjan(i32 %0) {
1:
  %2 = alloca i32
  %3 = alloca i32
  %4 = alloca i32
  %5 = alloca i32
  store i32 %0, ptr %2
  %6 = load i32, ptr @timer
  %7 = add i32 %6, 1
  store i32 %7, ptr @timer
  %8 = load ptr, ptr @low
  %9 = load i32, ptr %2
  %10 = sext i32 %9 to i64
  %11 = getelementptr i32, ptr %8, i64 %10
  store i32 %7, ptr %11
  %12 = load ptr, ptr @dfn
  %13 = load i32, ptr %2
  %14 = sext i32 %13 to i64
  %15 = getelementptr i32, ptr %12, i64 %14
  store i32 %7, ptr %15
  %16 = load i32, ptr %2
  %17 = load ptr, ptr @stack
  %18 = load i32, ptr @top
  %19 = add i32 %18, 1
  store i32 %19, ptr @top
  %20 = sext i32 %18 to i64
  %21 = getelementptr i32, ptr %17, i64 %20
  store i32 %16, ptr %21
  %22 = load ptr, ptr @adj
  %23 = load i32, ptr %2
  %24 = sext i32 %23 to i64
  %25 = getelementptr i32, ptr %22, i64 %24
  %26 = load i32, ptr %25
  store i32 %26, ptr %3
  br label %27

27:
  %28 = load i32, ptr %3
  %29 = icmp ne i32 %28, -1
  br i1 %29, label %30, label %90

30:
  %31 = load ptr, ptr @to
  %32 = load i32, ptr %3
  %33 = sext i32 %32 to i64
  %34 = getelementptr i32, ptr %31, i64 %33
  %35 = load i32, ptr %34
  store i32 %35, ptr %4
  %36 = load ptr, ptr @dfn
  %37 = load i32, ptr %4
  %38 = sext i32 %37 to i64
  %39 = getelementptr i32, ptr %36, i64 %38
  %40 = load i32, ptr %39
  %41 = icmp ne i32 %40, 0
  br i1 %41, label %59, label %42

42:
  %43 = load i32, ptr %4
  call void @tarjan(i32 %43)
  %44 = load ptr, ptr @low
  %45 = load i32, ptr %2
  %46 = sext i32 %45 to i64
  %47 = getelementptr i32, ptr %44, i64 %46
  %48 = load i32, ptr %47
  %49 = load ptr, ptr @low
  %50 = load i32, ptr %4
  %51 = sext i32 %50 to i64
  %52 = getelementptr i32, ptr %49, i64 %51
  %53 = load i32, ptr %52
  %54 = call i32 @min(i32 %48, i32 %53)
  %55 = load ptr, ptr @low
  %56 = load i32, ptr %2
  %57 = sext i32 %56 to i64
  %58 = getelementptr i32, ptr %55, i64 %57
  store i32 %54, ptr %58
  br label %83

59:
  %60 = load ptr, ptr @scc
  %61 = load i32, ptr %4
  %62 = sext i32 %61 to i64
  %63 = getelementptr i32, ptr %60, i64 %62
  %64 = load i32, ptr %63
  %65 = icmp ne i32 %64, 0
  br i1 %65, label %82, label %66

66:
  %67 = load ptr, ptr @low
  %68 = load i32, ptr %2
  %69 = sext i32 %68 to i64
  %70 = getelementptr i32, ptr %67, i64 %69
  %71 = load i32, ptr %70
  %72 = load ptr, ptr @dfn
  %73 = load i32, ptr %4
  %74 = sext i32 %73 to i64
  %75 = getelementptr i32, ptr %72, i64 %74
  %76 = load i32, ptr %75
  %77 = call i32 @min(i32 %71, i32 %76)
  %78 = load ptr, ptr @low
  %79 = load i32, ptr %2
  %80 = sext i32 %79 to i64
  %81 = getelementptr i32, ptr %78, i64 %80
  store i32 %77, ptr %81
  br label %82

82:
  br label %83

83:
  br label %84

84:
  %85 = load ptr, ptr @next
  %86 = load i32, ptr %3
  %87 = sext i32 %86 to i64
  %88 = getelementptr i32, ptr %85, i64 %87
  %89 = load i32, ptr %88
  store i32 %89, ptr %3
  br label %27

90:
  %91 = load ptr, ptr @low
  %92 = load i32, ptr %2
  %93 = sext i32 %92 to i64
  %94 = getelementptr i32, ptr %91, i64 %93
  %95 = load i32, ptr %94
  %96 = load ptr, ptr @dfn
  %97 = load i32, ptr %2
  %98 = sext i32 %97 to i64
  %99 = getelementptr i32, ptr %96, i64 %98
  %100 = load i32, ptr %99
  %101 = icmp eq i32 %95, %100
  br i1 %101, label %102, label %122

102:
  %103 = load i32, ptr @scc_cnt
  %104 = add i32 %103, 1
  store i32 %104, ptr @scc_cnt
  br label %105

105:
  %106 = load ptr, ptr @stack
  %107 = load i32, ptr @top
  %108 = add i32 %107, -1
  store i32 %108, ptr @top
  %109 = sext i32 %108 to i64
  %110 = getelementptr i32, ptr %106, i64 %109
  %111 = load i32, ptr %110
  store i32 %111, ptr %5
  %112 = load i32, ptr @scc_cnt
  %113 = load ptr, ptr @scc
  %114 = load i32, ptr %5
  %115 = sext i32 %114 to i64
  %116 = getelementptr i32, ptr %113, i64 %115
  store i32 %112, ptr %116
  br label %117

117:
  %118 = load i32, ptr %5
  %119 = load i32, ptr %2
  %120 = icmp ne i32 %118, %119
  br i1 %120, label %105, label %121

121:
  br label %122

122:
  ret void
}

define void @tarjan_all() {
0:
  %1 = alloca i32
  store i32 0, ptr %1
  br label %2

2:
  %3 = load i32, ptr %1
  %4 = load i32, ptr @n
  %5 = icmp slt i32 %3, %4
  br i1 %5, label %6, label %19

6:
  %7 = load ptr, ptr @dfn
  %8 = load i32, ptr %1
  %9 = sext i32 %8 to i64
  %10 = getelementptr i32, ptr %7, i64 %9
  %11 = load i32, ptr %10
  %12 = icmp ne i32 %11, 0
  br i1 %12, label %15, label %13

13:
  %14 = load i32, ptr %1
  call void @tarjan(i32 %14)
  br label %15

15:
  br label %16

16:
  %17 = load i32, ptr %1
  %18 = add i32 %17, 1
  store i32 %18, ptr %1
  br label %2

19:
  ret void
}

define i32 @main() {
0:
  store i32 8, ptr @n
  store i32 14, ptr @m
  %1 = call ptr @malloc(i64 32)
  store ptr %1, ptr @adj
  %2 = call ptr @memset(ptr %1, i32 -1, i64 32)
  %3 = call ptr @malloc(i64 56)
  store ptr %3, ptr @to
  %4 = call ptr @malloc(i64 56)
  store ptr %4, ptr @next
  call void @add_edge(i32 0, i32 0, i32 1)
  call void @add_edge(i32 1, i32 1, i32 2)
  call void @add_edge(i32 2, i32 1, i32 4)
  call void @add_edge(i32 3, i32 1, i32 5)
  call void @add_edge(i32 4, i32 2, i32 3)
  call void @add_edge(i32 5, i32 2, i32 6)
  call void @add_edge(i32 6, i32 3, i32 2)
  call void @add_edge(i32 7, i32 3, i32 7)
  call void @add_edge(i32 8, i32 4, i32 0)
  call void @add_edge(i32 9, i32 4, i32 5)
  call void @add_edge(i32 10, i32 5, i32 6)
  call void @add_edge(i32 11, i32 6, i32 5)
  call void @add_edge(i32 12, i32 6, i32 7)
  call void @add_edge(i32 13, i32 7, i32 7)
  %5 = call ptr @malloc(i64 32)
  store ptr %5, ptr @dfn
  %6 = call ptr @memset(ptr %5, i32 0, i64 32)
  %7 = call ptr @malloc(i64 32)
  store ptr %7, ptr @low
  %8 = call ptr @malloc(i64 32)
  store ptr %8, ptr @scc
  %9 = call ptr @memset(ptr %8, i32 0, i64 32)
  %10 = call ptr @malloc(i64 32)
  store ptr %10, ptr @stack
  call void @tarjan_all()
  %11 = getelementptr i32, ptr %8, i64 0
  %12 = load i32, ptr %11
  %13 = getelementptr i32, ptr %8, i64 1
  %14 = load i32, ptr %13
  %15 = getelementptr i32, ptr %8, i64 2
  %16 = load i32, ptr %15
  %17 = getelementptr i32, ptr %8, i64 3
  %18 = load i32, ptr %17
  %19 = getelementptr i32, ptr %8, i64 4
  %20 = load i32, ptr %19
  %21 = getelementptr i32, ptr %8, i64 5
  %22 = load i32, ptr %21
  %23 = getelementptr i32, ptr %8, i64 6
  %24 = load i32, ptr %23
  %25 = getelementptr i32, ptr %8, i64 7
  %26 = load i32, ptr %25
  %27 = call i32 (ptr, ...) @printf(ptr @format, i32 %12, i32 %14, i32 %16, i32 %18, i32 %20, i32 %22, i32 %24, i32 %26)
  call void @free(ptr %10)
  call void @free(ptr %8)
  call void @free(ptr %7)
  call void @free(ptr %5)
  call void @free(ptr %4)
  call void @free(ptr %3)
  call void @free(ptr %1)
  ret i32 0
}
