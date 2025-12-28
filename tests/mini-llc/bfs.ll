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
  %4 = call i32 (ptr, ...) @printf(ptr @format, i32 %3)
  ret void
}

define void @bfs(i32 %0) {
1:
  %2 = alloca i32
  %3 = alloca ptr
  %4 = alloca ptr
  %5 = alloca i32
  %6 = alloca i32
  %7 = alloca i32
  %8 = alloca i32
  %9 = alloca i32
  store i32 %0, ptr %2
  %10 = load i32, ptr @n
  %11 = sext i32 %10 to i64
  %12 = mul i64 %11, 4
  %13 = call ptr @malloc(i64 %12)
  store ptr %13, ptr %3
  %14 = load ptr, ptr %3
  %15 = load i32, ptr @n
  %16 = sext i32 %15 to i64
  %17 = mul i64 %16, 4
  %18 = call ptr @memset(ptr %14, i32 0, i64 %17)
  %19 = load i32, ptr @n
  %20 = sext i32 %19 to i64
  %21 = mul i64 %20, 4
  %22 = call ptr @malloc(i64 %21)
  store ptr %22, ptr %4
  store i32 0, ptr %5
  store i32 0, ptr %6
  %23 = load i32, ptr %2
  call void @visit(i32 %23)
  %24 = load ptr, ptr %3
  %25 = load i32, ptr %2
  %26 = sext i32 %25 to i64
  %27 = getelementptr i32, ptr %24, i64 %26
  store i32 1, ptr %27
  %28 = load i32, ptr %2
  %29 = load ptr, ptr %4
  %30 = load i32, ptr %6
  %31 = add i32 %30, 1
  store i32 %31, ptr %6
  %32 = sext i32 %30 to i64
  %33 = getelementptr i32, ptr %29, i64 %32
  store i32 %28, ptr %33
  br label %34

34:
  %35 = load i32, ptr %5
  %36 = load i32, ptr %6
  %37 = icmp ne i32 %35, %36
  br i1 %37, label %38, label %85

38:
  %39 = load ptr, ptr %4
  %40 = load i32, ptr %5
  %41 = add i32 %40, 1
  store i32 %41, ptr %5
  %42 = sext i32 %40 to i64
  %43 = getelementptr i32, ptr %39, i64 %42
  %44 = load i32, ptr %43
  store i32 %44, ptr %7
  %45 = load ptr, ptr @adj
  %46 = load i32, ptr %7
  %47 = sext i32 %46 to i64
  %48 = getelementptr i32, ptr %45, i64 %47
  %49 = load i32, ptr %48
  store i32 %49, ptr %8
  br label %50

50:
  %51 = load i32, ptr %8
  %52 = icmp ne i32 %51, -1
  br i1 %52, label %53, label %84

53:
  %54 = load ptr, ptr @to
  %55 = load i32, ptr %8
  %56 = sext i32 %55 to i64
  %57 = getelementptr i32, ptr %54, i64 %56
  %58 = load i32, ptr %57
  store i32 %58, ptr %9
  %59 = load ptr, ptr %3
  %60 = load i32, ptr %9
  %61 = sext i32 %60 to i64
  %62 = getelementptr i32, ptr %59, i64 %61
  %63 = load i32, ptr %62
  %64 = icmp ne i32 %63, 0
  br i1 %64, label %77, label %65

65:
  %66 = load i32, ptr %9
  call void @visit(i32 %66)
  %67 = load ptr, ptr %3
  %68 = load i32, ptr %9
  %69 = sext i32 %68 to i64
  %70 = getelementptr i32, ptr %67, i64 %69
  store i32 1, ptr %70
  %71 = load i32, ptr %9
  %72 = load ptr, ptr %4
  %73 = load i32, ptr %6
  %74 = add i32 %73, 1
  store i32 %74   , ptr %6
  %75 = sext i32 %73 to i64
  %76 = getelementptr i32, ptr %72, i64 %75
  store i32 %71, ptr %76
  br label %77

77:
  br label %78

78:
  %79 = load ptr, ptr @next
  %80 = load i32, ptr %8
  %81 = sext i32 %80 to i64
  %82 = getelementptr i32, ptr %79, i64 %81
  %83 = load i32, ptr %82
  store i32 %83, ptr %8
  br label %50

84:
  br label %34

85:
  %86 = load ptr, ptr %4
  call void @free(ptr %86)
  %87 = load ptr, ptr %3
  call void @free(ptr %87)
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
  call void @bfs(i32 0)
  call void @free(ptr %5)
  call void @free(ptr %4)
  call void @free(ptr %3)
  call void @free(ptr %1)
  ret i32 0
}
