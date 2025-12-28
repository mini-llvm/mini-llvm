@n = global i32 0
@m = global i32 0
@adj = global ptr null
@to = global ptr null
@next = global ptr null
@visited = global ptr null
@format = private constant [4 x i8] c"%d\0A\00"

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

define void @visit(i32 %0) {
1:
  %2 = alloca i32
  store i32 %0, ptr %2
  %3 = load i32, ptr %2
  %4 = call i32  @printf(ptr @format, i32 %3)
  ret void
}

define void @dfs(i32 %0) {
1:
  %2 = alloca i32
  %3 = alloca i32
  %4 = alloca i32
  store i32 %0, ptr %2
  %5 = load i32, ptr %2
  call void @visit(i32 %5)
  %6 = load ptr, ptr @visited
  %7 = load i32, ptr %2
  %8 = sext i32 %7 to i64
  %9 = getelementptr i32, ptr %6, i64 %8
  store i32 1, ptr %9
  %10 = load ptr, ptr @adj
  %11 = load i32, ptr %2
  %12 = sext i32 %11 to i64
  %13 = getelementptr i32, ptr %10, i64 %12
  %14 = load i32, ptr %13
  store i32 %14, ptr %3
  br label %15

15:
  %16 = load i32, ptr %3
  %17 = icmp ne i32 %16, -1
  br i1 %17, label %18, label %39

18:
  %19 = load ptr, ptr @to
  %20 = load i32, ptr %3
  %21 = sext i32 %20 to i64
  %22 = getelementptr i32, ptr %19, i64 %21
  %23 = load i32, ptr %22
  store i32 %23, ptr %4
  %24 = load ptr, ptr @visited
  %25 = load i32, ptr %4
  %26 = sext i32 %25 to i64
  %27 = getelementptr i32, ptr %24, i64 %26
  %28 = load i32, ptr %27
  %29 = icmp ne i32 %28, 0
  br i1 %29, label %32, label %30

30:
  %31 = load i32, ptr %4
  call void @dfs(i32 %31)
  br label %32

32:
  br label %33

33:
  %34 = load ptr, ptr @next
  %35 = load i32, ptr %3
  %36 = sext i32 %35 to i64
  %37 = getelementptr i32, ptr %34, i64 %36
  %38 = load i32, ptr %37
  store i32 %38, ptr %3
  br label %15

39:
  ret void
}

define i32 @main() {
0:
  store i32 9, ptr @n
  store i32 12, ptr @m
  %1 = call ptr @malloc(i64 36)
  store ptr %1, ptr @adj
  %2 = call ptr @memset(ptr %1, i32 -1, i64 36)
  %3 = call ptr @malloc(i64 48)
  store ptr %3, ptr @to
  %4 = call ptr @malloc(i64 48)
  store ptr %4, ptr @next
  call void @add_edge(i32 0, i32 0, i32 1)
  call void @add_edge(i32 1, i32 1, i32 2)
  call void @add_edge(i32 2, i32 3, i32 4)
  call void @add_edge(i32 3, i32 4, i32 5)
  call void @add_edge(i32 4, i32 6, i32 7)
  call void @add_edge(i32 5, i32 7, i32 8)
  call void @add_edge(i32 6, i32 0, i32 3)
  call void @add_edge(i32 7, i32 1, i32 4)
  call void @add_edge(i32 8, i32 2, i32 5)
  call void @add_edge(i32 9, i32 3, i32 6)
  call void @add_edge(i32 10, i32 4, i32 7)
  call void @add_edge(i32 11, i32 5, i32 8)
  %5 = call ptr @malloc(i64 36)
  store ptr %5, ptr @visited
  %6 = call ptr @memset(ptr %5, i32 0, i64 36)
  call void @dfs(i32 0)
  call void @free(ptr %5)
  call void @free(ptr %4)
  call void @free(ptr %3)
  call void @free(ptr %1)
  ret i32 0
}
