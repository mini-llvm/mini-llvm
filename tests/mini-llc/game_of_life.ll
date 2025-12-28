declare ptr @malloc(i64) inaccessiblememonly
declare void @free(ptr) inaccessiblemem_or_argmemonly
declare ptr @memset(ptr, i32, i64) argmemonly
declare i32 @putchar(i32)

define i32 @count_neighbors(ptr %0, i32 %1, i32 %2, i32 %3, i32 %4) {
5:
  %6 = alloca ptr
  %7 = alloca i32
  %8 = alloca i32
  %9 = alloca i32
  %10 = alloca i32
  %11 = alloca i32
  %12 = alloca i32
  %13 = alloca i32
  %14 = alloca i32
  %15 = alloca i32
  store ptr %0, ptr %6
  store i32 %1, ptr %7
  store i32 %2, ptr %8
  store i32 %3, ptr %9
  store i32 %4, ptr %10
  store i32 0, ptr %11
  store i32 -1, ptr %12
  br label %16

16:
  %17 = load i32, ptr %12
  %18 = icmp sle i32 %17, 1
  br i1 %18, label %19, label %64

19:
  store i32 -1, ptr %13
  br label %20

20:
  %21 = load i32, ptr %13
  %22 = icmp sle i32 %21, 1
  br i1 %22, label %23, label %60

23:
  %24 = load i32, ptr %12
  %25 = icmp eq i32 %24, 0
  br i1 %25, label %26, label %30

26:
  %27 = load i32, ptr %13
  %28 = icmp eq i32 %27, 0
  br i1 %28, label %29, label %30

29:
  br label %57

30:
  %31 = load i32, ptr %9
  %32 = load i32, ptr %12
  %33 = add i32 %31, %32
  %34 = load i32, ptr %7
  %35 = add i32 %33, %34
  %36 = load i32, ptr %7
  %37 = srem i32 %35, %36
  store i32 %37, ptr %14
  %38 = load i32, ptr %10
  %39 = load i32, ptr %13
  %40 = add i32 %38, %39
  %41 = load i32, ptr %8
  %42 = add i32 %40, %41
  %43 = load i32, ptr %8
  %44 = srem i32 %42, %43
  store i32 %44, ptr %15
  %45 = load ptr, ptr %6
  %46 = load i32, ptr %14
  %47 = load i32, ptr %8
  %48 = mul i32 %46, %47
  %49 = load i32, ptr %15
  %50 = add i32 %48, %49
  %51 = sext i32 %50 to i64
  %52 = getelementptr i8, ptr %45, i64 %51
  %53 = load i8, ptr %52
  %54 = zext i8 %53 to i32
  %55 = load i32, ptr %11
  %56 = add i32 %55, %54
  store i32 %56, ptr %11
  br label %57

57:
  %58 = load i32, ptr %13
  %59 = add i32 %58, 1
  store i32 %59, ptr %13
  br label %20

60:
  br label %61

61:
  %62 = load i32, ptr %12
  %63 = add i32 %62, 1
  store i32 %63, ptr %12
  br label %16

64:
  %65 = load i32, ptr %11
  ret i32 %65
}

define void @step(ptr %0, ptr %1, i32 %2, i32 %3) {
4:
  %5 = alloca ptr
  %6 = alloca ptr
  %7 = alloca i32
  %8 = alloca i32
  %9 = alloca i32
  %10 = alloca i32
  %11 = alloca i32
  store ptr %0, ptr %5
  store ptr %1, ptr %6
  store i32 %2, ptr %7
  store i32 %3, ptr %8
  store i32 0, ptr %9
  br label %12

12:
  %13 = load i32, ptr %9
  %14 = load i32, ptr %7
  %15 = icmp slt i32 %13, %14
  br i1 %15, label %16, label %80

16:
  store i32 0, ptr %10
  br label %17

17:
  %18 = load i32, ptr %10
  %19 = load i32, ptr %8
  %20 = icmp slt i32 %18, %19
  br i1 %20, label %21, label %76

21:
  %22 = load ptr, ptr %5
  %23 = load i32, ptr %7
  %24 = load i32, ptr %8
  %25 = load i32, ptr %9
  %26 = load i32, ptr %10
  %27 = call i32 @count_neighbors(ptr %22, i32 %23, i32 %24, i32 %25, i32 %26)
  store i32 %27, ptr %11
  %28 = load ptr, ptr %5
  %29 = load i32, ptr %9
  %30 = load i32, ptr %8
  %31 = mul i32 %29, %30
  %32 = load i32, ptr %10
  %33 = add i32 %31, %32
  %34 = sext i32 %33 to i64
  %35 = getelementptr i8, ptr %28, i64 %34
  %36 = load i8, ptr %35
  %37 = zext i8 %36 to i32
  %38 = icmp ne i32 %37, 0
  br i1 %38, label %39, label %45

39:
  %40 = load i32, ptr %11
  %41 = icmp eq i32 %40, 2
  br i1 %41, label %61, label %42

42:
  %43 = load i32, ptr %11
  %44 = icmp eq i32 %43, 3
  br i1 %44, label %61, label %45

45:
  %46 = load ptr, ptr %5
  %47 = load i32, ptr %9
  %48 = load i32, ptr %8
  %49 = mul i32 %47, %48
  %50 = load i32, ptr %10
  %51 = add i32 %49, %50
  %52 = sext i32 %51 to i64
  %53 = getelementptr i8, ptr %46, i64 %52
  %54 = load i8, ptr %53
  %55 = icmp ne i8 %54, 0
  br i1 %55, label %59, label %56

56:
  %57 = load i32, ptr %11
  %58 = icmp eq i32 %57, 3
  br label %59

59:
  %60 = phi i1 [ false, %45 ], [ %58, %56 ]
  br label %61

61:
  %62 = phi i1 [ true, %42 ], [ true, %39 ], [ %60, %59 ]
  %63 = zext i1 %62 to i32
  %64 = trunc i32 %63 to i8
  %65 = load ptr, ptr %6
  %66 = load i32, ptr %9
  %67 = load i32, ptr %8
  %68 = mul i32 %66, %67
  %69 = load i32, ptr %10
  %70 = add i32 %68, %69
  %71 = sext i32 %70 to i64
  %72 = getelementptr i8, ptr %65, i64 %71
  store i8 %64, ptr %72
  br label %73

73:
  %74 = load i32, ptr %10
  %75 = add i32 %74, 1
  store i32 %75, ptr %10
  br label %17

76:
  br label %77

77:
  %78 = load i32, ptr %9
  %79 = add i32 %78, 1
  store i32 %79, ptr %9
  br label %12

80:
  ret void
}

define void @draw(ptr %0, i32 %1, i32 %2) {
3:
  %4 = alloca ptr
  %5 = alloca i32
  %6 = alloca i32
  %7 = alloca i32
  %8 = alloca i32
  store ptr %0, ptr %4
  store i32 %1, ptr %5
  store i32 %2, ptr %6
  store i32 0, ptr %7
  br label %9

9:
  %10 = load i32, ptr %7
  %11 = load i32, ptr %5
  %12 = icmp slt i32 %10, %11
  br i1 %12, label %13, label %41

13:
  store i32 0, ptr %8
  br label %14

14:
  %15 = load i32, ptr %8
  %16 = load i32, ptr %6
  %17 = icmp slt i32 %15, %16
  br i1 %17, label %18, label %36

18:
  %19 = load ptr, ptr %4
  %20 = load i32, ptr %7
  %21 = load i32, ptr %6
  %22 = mul i32 %20, %21
  %23 = load i32, ptr %8
  %24 = add i32 %22, %23
  %25 = sext i32 %24 to i64
  %26 = getelementptr i8, ptr %19, i64 %25
  %27 = load i8, ptr %26
  %28 = zext i8 %27 to i32
  %29 = icmp ne i32 %28, 0
  %30 = zext i1 %29 to i64
  %31 = select i1 %29, i32 88, i32 46
  %32 = call i32 @putchar(i32 %31)
  br label %33

33:
  %34 = load i32, ptr %8
  %35 = add i32 %34, 1
  store i32 %35, ptr %8
  br label %14

36:
  %37 = call i32 @putchar(i32 10)
  br label %38

38:
  %39 = load i32, ptr %7
  %40 = add i32 %39, 1
  store i32 %40, ptr %7
  br label %9

41:
  %42 = call i32 @putchar(i32 10)
  ret void
}

define void @swap(ptr %0, ptr %1) {
2:
  %3 = alloca ptr
  %4 = alloca ptr
  %5 = alloca ptr
  store ptr %0, ptr %3
  store ptr %1, ptr %4
  %6 = load ptr, ptr %3
  %7 = load ptr, ptr %6
  store ptr %7, ptr %5
  %8 = load ptr, ptr %4
  %9 = load ptr, ptr %8
  %10 = load ptr, ptr %3
  store ptr %9, ptr %10
  %11 = load ptr, ptr %5
  %12 = load ptr, ptr %4
  store ptr %11, ptr %12
  ret void
}

define i32 @main() {
0:
  %1 = alloca ptr
  %2 = alloca ptr
  %3 = call ptr @malloc(i64 418)
  store ptr %3, ptr %1
  %4 = call ptr @malloc(i64 418)
  store ptr %4, ptr %2
  %5 = call ptr @memset(ptr %3, i32 0, i64 418)
  %6 = getelementptr i8, ptr %3, i64 63
  store i8 1, ptr %6
  %7 = getelementptr i8, ptr %3, i64 99
  store i8 1, ptr %7
  %8 = getelementptr i8, ptr %3, i64 101
  store i8 1, ptr %8
  %9 = getelementptr i8, ptr %3, i64 127
  store i8 1, ptr %9
  %10 = getelementptr i8, ptr %3, i64 128
  store i8 1, ptr %10
  %11 = getelementptr i8, ptr %3, i64 135
  store i8 1, ptr %11
  %12 = getelementptr i8, ptr %3, i64 136
  store i8 1, ptr %12
  %13 = getelementptr i8, ptr %3, i64 149
  store i8 1, ptr %13
  %14 = getelementptr i8, ptr %3, i64 150
  store i8 1, ptr %14
  %15 = getelementptr i8, ptr %3, i64 164
  store i8 1, ptr %15
  %16 = getelementptr i8, ptr %3, i64 168
  store i8 1, ptr %16
  %17 = getelementptr i8, ptr %3, i64 173
  store i8 1, ptr %17
  %18 = getelementptr i8, ptr %3, i64 174
  store i8 1, ptr %18
  %19 = getelementptr i8, ptr %3, i64 187
  store i8 1, ptr %19
  %20 = getelementptr i8, ptr %3, i64 188
  store i8 1, ptr %20
  %21 = getelementptr i8, ptr %3, i64 191
  store i8 1, ptr %21
  %22 = getelementptr i8, ptr %3, i64 192
  store i8 1, ptr %22
  %23 = getelementptr i8, ptr %3, i64 201
  store i8 1, ptr %23
  %24 = getelementptr i8, ptr %3, i64 207
  store i8 1, ptr %24
  %25 = getelementptr i8, ptr %3, i64 211
  store i8 1, ptr %25
  %26 = getelementptr i8, ptr %3, i64 212
  store i8 1, ptr %26
  %27 = getelementptr i8, ptr %3, i64 229
  store i8 1, ptr %27
  %28 = getelementptr i8, ptr %3, i64 230
  store i8 1, ptr %28
  %29 = getelementptr i8, ptr %3, i64 239
  store i8 1, ptr %29
  %30 = getelementptr i8, ptr %3, i64 243
  store i8 1, ptr %30
  %31 = getelementptr i8, ptr %3, i64 245
  store i8 1, ptr %31
  %32 = getelementptr i8, ptr %3, i64 246
  store i8 1, ptr %32
  %33 = getelementptr i8, ptr %3, i64 251
  store i8 1, ptr %33
  %34 = getelementptr i8, ptr %3, i64 253
  store i8 1, ptr %34
  %35 = getelementptr i8, ptr %3, i64 277
  store i8 1, ptr %35
  %36 = getelementptr i8, ptr %3, i64 283
  store i8 1, ptr %36
  %37 = getelementptr i8, ptr %3, i64 291
  store i8 1, ptr %37
  %38 = getelementptr i8, ptr %3, i64 316
  store i8 1, ptr %38
  %39 = getelementptr i8, ptr %3, i64 320
  store i8 1, ptr %39
  %40 = getelementptr i8, ptr %3, i64 355
  store i8 1, ptr %40
  %41 = getelementptr i8, ptr %3, i64 356
  store i8 1, ptr %41
  call void @draw(ptr %3, i32 11, i32 38)
  call void @step(ptr %3, ptr %4, i32 11, i32 38)
  call void @swap(ptr %1, ptr %2)
  %42 = load ptr, ptr %1
  call void @draw(ptr %42, i32 11, i32 38)
  %43 = load ptr, ptr %2
  call void @step(ptr %42, ptr %43, i32 11, i32 38)
  call void @swap(ptr %1, ptr %2)
  %44 = load ptr, ptr %1
  call void @draw(ptr %44, i32 11, i32 38)
  %45 = load ptr, ptr %2
  call void @step(ptr %44, ptr %45, i32 11, i32 38)
  call void @swap(ptr %1, ptr %2)
  %46 = load ptr, ptr %1
  call void @draw(ptr %46, i32 11, i32 38)
  %47 = load ptr, ptr %2
  call void @step(ptr %46, ptr %47, i32 11, i32 38)
  call void @swap(ptr %1, ptr %2)
  %48 = load ptr, ptr %1
  call void @draw(ptr %48, i32 11, i32 38)
  %49 = load ptr, ptr %2
  call void @step(ptr %48, ptr %49, i32 11, i32 38)
  call void @swap(ptr %1, ptr %2)
  %50 = load ptr, ptr %1
  call void @draw(ptr %50, i32 11, i32 38)
  %51 = load ptr, ptr %2
  call void @step(ptr %50, ptr %51, i32 11, i32 38)
  call void @swap(ptr %1, ptr %2)
  %52 = load ptr, ptr %1
  call void @draw(ptr %52, i32 11, i32 38)
  %53 = load ptr, ptr %2
  call void @step(ptr %52, ptr %53, i32 11, i32 38)
  call void @swap(ptr %1, ptr %2)
  %54 = load ptr, ptr %1
  call void @draw(ptr %54, i32 11, i32 38)
  %55 = load ptr, ptr %2
  call void @step(ptr %54, ptr %55, i32 11, i32 38)
  call void @swap(ptr %1, ptr %2)
  %56 = load ptr, ptr %1
  call void @draw(ptr %56, i32 11, i32 38)
  %57 = load ptr, ptr %2
  call void @step(ptr %56, ptr %57, i32 11, i32 38)
  call void @swap(ptr %1, ptr %2)
  %58 = load ptr, ptr %1
  call void @draw(ptr %58, i32 11, i32 38)
  %59 = load ptr, ptr %2
  call void @step(ptr %58, ptr %59, i32 11, i32 38)
  call void @swap(ptr %1, ptr %2)
  %60 = load ptr, ptr %1
  call void @free(ptr %60)
  %61 = load ptr, ptr %2
  call void @free(ptr %61)
  ret i32 0
}
