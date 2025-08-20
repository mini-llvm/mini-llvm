@n = global i32 0
@m = global i32 0
@adj = global ptr null
@to = global ptr null
@weight = global ptr null
@next = global ptr null
@heap = global ptr null
@heap_size = global i32 0
@heap_idx = global ptr null
@dist = global ptr null
@format = private constant [16 x i8] c"%d %d %d %d %d\0A\00"

declare ptr @malloc(i64) inaccessiblememonly
declare void @free(ptr) inaccessiblemem_or_argmemonly
declare ptr @memset(ptr, i32, i64) argmemonly
declare i32 @printf(ptr, ...)

define void @add_edge(i32 %0, i32 %1, i32 %2, i32 %3) {
4:
  %5 = alloca i32
  %6 = alloca i32
  %7 = alloca i32
  %8 = alloca i32
  store i32 %0, ptr %5
  store i32 %1, ptr %6
  store i32 %2, ptr %7
  store i32 %3, ptr %8
  %9 = load i32, ptr %7
  %10 = load ptr, ptr @to
  %11 = load i32, ptr %5
  %12 = sext i32 %11 to i64
  %13 = getelementptr i32, ptr %10, i64 %12
  store i32 %9, ptr %13
  %14 = load i32, ptr %8
  %15 = load ptr, ptr @weight
  %16 = load i32, ptr %5
  %17 = sext i32 %16 to i64
  %18 = getelementptr i32, ptr %15, i64 %17
  store i32 %14, ptr %18
  %19 = load ptr, ptr @adj
  %20 = load i32, ptr %6
  %21 = sext i32 %20 to i64
  %22 = getelementptr i32, ptr %19, i64 %21
  %23 = load i32, ptr %22
  %24 = load ptr, ptr @next
  %25 = load i32, ptr %5
  %26 = sext i32 %25 to i64
  %27 = getelementptr i32, ptr %24, i64 %26
  store i32 %23, ptr %27
  %28 = load i32, ptr %5
  %29 = load ptr, ptr @adj
  %30 = load i32, ptr %6
  %31 = sext i32 %30 to i64
  %32 = getelementptr i32, ptr %29, i64 %31
  store i32 %28, ptr %32
  ret void
}

define void @percolate_down(i32 %0) {
1:
  %2 = alloca i32
  %3 = alloca i32
  %4 = alloca i32
  store i32 %0, ptr %2
  %5 = load ptr, ptr @heap
  %6 = load i32, ptr %2
  %7 = sext i32 %6 to i64
  %8 = getelementptr i32, ptr %5, i64 %7
  %9 = load i32, ptr %8
  store i32 %9, ptr %3
  br label %10

10:
  %11 = load i32, ptr %2
  %12 = load i32, ptr @heap_size
  %13 = sdiv i32 %12, 2
  %14 = icmp slt i32 %11, %13
  br i1 %14, label %15, label %84

15:
  %16 = load i32, ptr %2
  %17 = mul i32 %16, 2
  %18 = add i32 %17, 1
  store i32 %18, ptr %4
  %19 = load i32, ptr %4
  %20 = add i32 %19, 1
  %21 = load i32, ptr @heap_size
  %22 = icmp slt i32 %20, %21
  br i1 %22, label %23, label %47

23:
  %24 = load ptr, ptr @dist
  %25 = load ptr, ptr @heap
  %26 = load i32, ptr %4
  %27 = add i32 %26, 1
  %28 = sext i32 %27 to i64
  %29 = getelementptr i32, ptr %25, i64 %28
  %30 = load i32, ptr %29
  %31 = sext i32 %30 to i64
  %32 = getelementptr i32, ptr %24, i64 %31
  %33 = load i32, ptr %32
  %34 = load ptr, ptr @dist
  %35 = load ptr, ptr @heap
  %36 = load i32, ptr %4
  %37 = sext i32 %36 to i64
  %38 = getelementptr i32, ptr %35, i64 %37
  %39 = load i32, ptr %38
  %40 = sext i32 %39 to i64
  %41 = getelementptr i32, ptr %34, i64 %40
  %42 = load i32, ptr %41
  %43 = icmp slt i32 %33, %42
  br i1 %43, label %44, label %47

44:
  %45 = load i32, ptr %4
  %46 = add i32 %45, 1
  store i32 %46, ptr %4
  br label %47

47:
  %48 = load ptr, ptr @dist
  %49 = load i32, ptr %3
  %50 = sext i32 %49 to i64
  %51 = getelementptr i32, ptr %48, i64 %50
  %52 = load i32, ptr %51
  %53 = load ptr, ptr @dist
  %54 = load ptr, ptr @heap
  %55 = load i32, ptr %4
  %56 = sext i32 %55 to i64
  %57 = getelementptr i32, ptr %54, i64 %56
  %58 = load i32, ptr %57
  %59 = sext i32 %58 to i64
  %60 = getelementptr i32, ptr %53, i64 %59
  %61 = load i32, ptr %60
  %62 = icmp sle i32 %52, %61
  br i1 %62, label %63, label %64

63:
  br label %84

64:
  %65 = load ptr, ptr @heap
  %66 = load i32, ptr %4
  %67 = sext i32 %66 to i64
  %68 = getelementptr i32, ptr %65, i64 %67
  %69 = load i32, ptr %68
  %70 = load ptr, ptr @heap
  %71 = load i32, ptr %2
  %72 = sext i32 %71 to i64
  %73 = getelementptr i32, ptr %70, i64 %72
  store i32 %69, ptr %73
  %74 = load i32, ptr %2
  %75 = load ptr, ptr @heap_idx
  %76 = load ptr, ptr @heap
  %77 = load i32, ptr %2
  %78 = sext i32 %77 to i64
  %79 = getelementptr i32, ptr %76, i64 %78
  %80 = load i32, ptr %79
  %81 = sext i32 %80 to i64
  %82 = getelementptr i32, ptr %75, i64 %81
  store i32 %74, ptr %82
  %83 = load i32, ptr %4
  store i32 %83, ptr %2
  br label %10

84:
  %85 = load i32, ptr %3
  %86 = load ptr, ptr @heap
  %87 = load i32, ptr %2
  %88 = sext i32 %87 to i64
  %89 = getelementptr i32, ptr %86, i64 %88
  store i32 %85, ptr %89
  %90 = load i32, ptr %2
  %91 = load ptr, ptr @heap_idx
  %92 = load ptr, ptr @heap
  %93 = load i32, ptr %2
  %94 = sext i32 %93 to i64
  %95 = getelementptr i32, ptr %92, i64 %94
  %96 = load i32, ptr %95
  %97 = sext i32 %96 to i64
  %98 = getelementptr i32, ptr %91, i64 %97
  store i32 %90, ptr %98
  ret void
}

define void @percolate_up(i32 %0) {
1:
  %2 = alloca i32
  %3 = alloca i32
  %4 = alloca i32
  store i32 %0, ptr %2
  %5 = load ptr, ptr @heap
  %6 = load i32, ptr %2
  %7 = sext i32 %6 to i64
  %8 = getelementptr i32, ptr %5, i64 %7
  %9 = load i32, ptr %8
  store i32 %9, ptr %3
  br label %10

10:
  %11 = load i32, ptr %2
  %12 = icmp sgt i32 %11, 0
  br i1 %12, label %13, label %53

13:
  %14 = load i32, ptr %2
  %15 = sub i32 %14, 1
  %16 = sdiv i32 %15, 2
  store i32 %16, ptr %4
  %17 = load ptr, ptr @dist
  %18 = load ptr, ptr @heap
  %19 = load i32, ptr %4
  %20 = sext i32 %19 to i64
  %21 = getelementptr i32, ptr %18, i64 %20
  %22 = load i32, ptr %21
  %23 = sext i32 %22 to i64
  %24 = getelementptr i32, ptr %17, i64 %23
  %25 = load i32, ptr %24
  %26 = load ptr, ptr @dist
  %27 = load i32, ptr %3
  %28 = sext i32 %27 to i64
  %29 = getelementptr i32, ptr %26, i64 %28
  %30 = load i32, ptr %29
  %31 = icmp sle i32 %25, %30
  br i1 %31, label %32, label %33

32:
  br label %53

33:
  %34 = load ptr, ptr @heap
  %35 = load i32, ptr %4
  %36 = sext i32 %35 to i64
  %37 = getelementptr i32, ptr %34, i64 %36
  %38 = load i32, ptr %37
  %39 = load ptr, ptr @heap
  %40 = load i32, ptr %2
  %41 = sext i32 %40 to i64
  %42 = getelementptr i32, ptr %39, i64 %41
  store i32 %38, ptr %42
  %43 = load i32, ptr %2
  %44 = load ptr, ptr @heap_idx
  %45 = load ptr, ptr @heap
  %46 = load i32, ptr %2
  %47 = sext i32 %46 to i64
  %48 = getelementptr i32, ptr %45, i64 %47
  %49 = load i32, ptr %48
  %50 = sext i32 %49 to i64
  %51 = getelementptr i32, ptr %44, i64 %50
  store i32 %43, ptr %51
  %52 = load i32, ptr %4
  store i32 %52, ptr %2
  br label %10

53:
  %54 = load i32, ptr %3
  %55 = load ptr, ptr @heap
  %56 = load i32, ptr %2
  %57 = sext i32 %56 to i64
  %58 = getelementptr i32, ptr %55, i64 %57
  store i32 %54, ptr %58
  %59 = load i32, ptr %2
  %60 = load ptr, ptr @heap_idx
  %61 = load ptr, ptr @heap
  %62 = load i32, ptr %2
  %63 = sext i32 %62 to i64
  %64 = getelementptr i32, ptr %61, i64 %63
  %65 = load i32, ptr %64
  %66 = sext i32 %65 to i64
  %67 = getelementptr i32, ptr %60, i64 %66
  store i32 %59, ptr %67
  ret void
}

define i32 @is_empty() {
0:
  %1 = load i32, ptr @heap_size
  %2 = icmp eq i32 %1, 0
  %3 = zext i1 %2 to i32
  ret i32 %3
}

define i32 @find_min() {
0:
  %1 = load ptr, ptr @heap
  %2 = getelementptr i32, ptr %1, i64 0
  %3 = load i32, ptr %2
  ret i32 %3
}

define void @insert(i32 %0, i32 %1) {
2:
  %3 = alloca i32
  %4 = alloca i32
  store i32 %0, ptr %3
  store i32 %1, ptr %4
  %5 = load i32, ptr %4
  %6 = load ptr, ptr @dist
  %7 = load i32, ptr %3
  %8 = sext i32 %7 to i64
  %9 = getelementptr i32, ptr %6, i64 %8
  store i32 %5, ptr %9
  %10 = load i32, ptr @heap_size
  %11 = add i32 %10, 1
  store i32 %11, ptr @heap_size
  %12 = load i32, ptr %3
  %13 = load ptr, ptr @heap
  %14 = load i32, ptr @heap_size
  %15 = sub i32 %14, 1
  %16 = sext i32 %15 to i64
  %17 = getelementptr i32, ptr %13, i64 %16
  store i32 %12, ptr %17
  %18 = load i32, ptr @heap_size
  %19 = sub i32 %18, 1
  %20 = load ptr, ptr @heap_idx
  %21 = load ptr, ptr @heap
  %22 = load i32, ptr @heap_size
  %23 = sub i32 %22, 1
  %24 = sext i32 %23 to i64
  %25 = getelementptr i32, ptr %21, i64 %24
  %26 = load i32, ptr %25
  %27 = sext i32 %26 to i64
  %28 = getelementptr i32, ptr %20, i64 %27
  store i32 %19, ptr %28
  %29 = load i32, ptr @heap_size
  %30 = sub i32 %29, 1
  call void @percolate_up(i32 %30)
  ret void
}

define void @delete_min() {
0:
  %1 = load i32, ptr @heap_size
  %2 = add i32 %1, -1
  store i32 %2, ptr @heap_size
  %3 = load ptr, ptr @heap
  %4 = load i32, ptr @heap_size
  %5 = sext i32 %4 to i64
  %6 = getelementptr i32, ptr %3, i64 %5
  %7 = load i32, ptr %6
  %8 = load ptr, ptr @heap
  %9 = getelementptr i32, ptr %8, i64 0
  store i32 %7, ptr %9
  %10 = load ptr, ptr @heap_idx
  %11 = load ptr, ptr @heap
  %12 = getelementptr i32, ptr %11, i64 0
  %13 = load i32, ptr %12
  %14 = sext i32 %13 to i64
  %15 = getelementptr i32, ptr %10, i64 %14
  store i32 0, ptr %15
  call void @percolate_down(i32 0)
  ret void
}

define void @decrease_key(i32 %0, i32 %1) {
2:
  %3 = alloca i32
  %4 = alloca i32
  store i32 %0, ptr %3
  store i32 %1, ptr %4
  %5 = load i32, ptr %4
  %6 = load ptr, ptr @dist
  %7 = load i32, ptr %3
  %8 = sext i32 %7 to i64
  %9 = getelementptr i32, ptr %6, i64 %8
  store i32 %5, ptr %9
  %10 = load ptr, ptr @heap_idx
  %11 = load i32, ptr %3
  %12 = sext i32 %11 to i64
  %13 = getelementptr i32, ptr %10, i64 %12
  %14 = load i32, ptr %13
  call void @percolate_up(i32 %14)
  ret void
}

define void @dijkstra(i32 %0) {
1:
  %2 = alloca i32
  %3 = alloca i32
  %4 = alloca i32
  %5 = alloca i32
  %6 = alloca i32
  store i32 %0, ptr %2
  %7 = load i32, ptr %2
  call void @insert(i32 %7, i32 0)
  br label %8

8:
  %9 = call i32 @is_empty()
  %10 = icmp ne i32 %9, 0
  %11 = xor i1 %10, true
  br i1 %11, label %12, label %80

12:
  %13 = call i32 @find_min()
  store i32 %13, ptr %3
  call void @delete_min()
  %14 = load ptr, ptr @adj
  %15 = load i32, ptr %3
  %16 = sext i32 %15 to i64
  %17 = getelementptr i32, ptr %14, i64 %16
  %18 = load i32, ptr %17
  store i32 %18, ptr %4
  br label %19

19:
  %20 = load i32, ptr %4
  %21 = icmp ne i32 %20, -1
  br i1 %21, label %22, label %79

22:
  %23 = load ptr, ptr @to
  %24 = load i32, ptr %4
  %25 = sext i32 %24 to i64
  %26 = getelementptr i32, ptr %23, i64 %25
  %27 = load i32, ptr %26
  store i32 %27, ptr %5
  %28 = load ptr, ptr @weight
  %29 = load i32, ptr %4
  %30 = sext i32 %29 to i64
  %31 = getelementptr i32, ptr %28, i64 %30
  %32 = load i32, ptr %31
  store i32 %32, ptr %6
  %33 = load ptr, ptr @dist
  %34 = load i32, ptr %5
  %35 = sext i32 %34 to i64
  %36 = getelementptr i32, ptr %33, i64 %35
  %37 = load i32, ptr %36
  %38 = icmp eq i32 %37, -1
  br i1 %38, label %39, label %48

39:
  %40 = load i32, ptr %5
  %41 = load ptr, ptr @dist
  %42 = load i32, ptr %3
  %43 = sext i32 %42 to i64
  %44 = getelementptr i32, ptr %41, i64 %43
  %45 = load i32, ptr %44
  %46 = load i32, ptr %6
  %47 = add i32 %45, %46
  call void @insert(i32 %40, i32 %47)
  br label %72

48:
  %49 = load ptr, ptr @dist
  %50 = load i32, ptr %5
  %51 = sext i32 %50 to i64
  %52 = getelementptr i32, ptr %49, i64 %51
  %53 = load i32, ptr %52
  %54 = load ptr, ptr @dist
  %55 = load i32, ptr %3
  %56 = sext i32 %55 to i64
  %57 = getelementptr i32, ptr %54, i64 %56
  %58 = load i32, ptr %57
  %59 = load i32, ptr %6
  %60 = add i32 %58, %59
  %61 = icmp sgt i32 %53, %60
  br i1 %61, label %62, label %71

62:
  %63 = load i32, ptr %5
  %64 = load ptr, ptr @dist
  %65 = load i32, ptr %3
  %66 = sext i32 %65 to i64
  %67 = getelementptr i32, ptr %64, i64 %66
  %68 = load i32, ptr %67
  %69 = load i32, ptr %6
  %70 = add i32 %68, %69
  call void @decrease_key(i32 %63, i32 %70)
  br label %71

71:
  br label %72

72:
  br label %73

73:
  %74 = load ptr, ptr @next
  %75 = load i32, ptr %4
  %76 = sext i32 %75 to i64
  %77 = getelementptr i32, ptr %74, i64 %76
  %78 = load i32, ptr %77
  store i32 %78, ptr %4
  br label %19

79:
  br label %8

80:
  ret void
}

define i32 @main() {
0:
  store i32 5, ptr @n
  store i32 10, ptr @m
  %1 = call ptr @malloc(i64 20)
  store ptr %1, ptr @adj
  %2 = call ptr @memset(ptr %1, i32 -1, i64 20)
  %3 = call ptr @malloc(i64 40)
  store ptr %3, ptr @to
  %4 = call ptr @malloc(i64 40)
  store ptr %4, ptr @weight
  %5 = call ptr @malloc(i64 40)
  store ptr %5, ptr @next
  call void @add_edge(i32 0, i32 0, i32 1, i32 3)
  call void @add_edge(i32 1, i32 0, i32 3, i32 5)
  call void @add_edge(i32 2, i32 1, i32 2, i32 6)
  call void @add_edge(i32 3, i32 1, i32 3, i32 2)
  call void @add_edge(i32 4, i32 2, i32 4, i32 2)
  call void @add_edge(i32 5, i32 3, i32 1, i32 1)
  call void @add_edge(i32 6, i32 3, i32 2, i32 4)
  call void @add_edge(i32 7, i32 3, i32 4, i32 6)
  call void @add_edge(i32 8, i32 4, i32 0, i32 3)
  call void @add_edge(i32 9, i32 4, i32 2, i32 7)
  %6 = call ptr @malloc(i64 20)
  store ptr %6, ptr @heap
  %7 = call ptr @malloc(i64 20)
  store ptr %7, ptr @heap_idx
  %8 = call ptr @malloc(i64 20)
  store ptr %8, ptr @dist
  %9 = call ptr @memset(ptr %8, i32 -1, i64 20)
  call void @dijkstra(i32 0)
  %10 = getelementptr i32, ptr %8, i64 0
  %11 = load i32, ptr %10
  %12 = getelementptr i32, ptr %8, i64 1
  %13 = load i32, ptr %12
  %14 = getelementptr i32, ptr %8, i64 2
  %15 = load i32, ptr %14
  %16 = getelementptr i32, ptr %8, i64 3
  %17 = load i32, ptr %16
  %18 = getelementptr i32, ptr %8, i64 4
  %19 = load i32, ptr %18
  %20 = call i32 (ptr, ...) @printf(ptr @format, i32 %11, i32 %13, i32 %15, i32 %17, i32 %19)
  call void @free(ptr %8)
  call void @free(ptr %7)
  call void @free(ptr %6)
  call void @free(ptr %5)
  call void @free(ptr %4)
  call void @free(ptr %3)
  call void @free(ptr %1)
  ret i32 0
}
