@format = private constant [4 x i8] c"%d\0A\00"

declare ptr @calloc(i64, i64) inaccessiblememonly
declare void @free(ptr) inaccessiblemem_or_argmemonly
declare i32 @printf(ptr, ...)

define ptr @bitset_create(i32 %0) {
1:
  %2 = alloca i32
  store i32 %0, ptr %2
  %3 = load i32, ptr %2
  %4 = add i32 %3, 63
  %5 = sdiv i32 %4, 64
  %6 = sext i32 %5 to i64
  %7 = call ptr @calloc(i64 %6, i64 8)
  ret ptr %7
}

define void @bitset_destroy(ptr %0) {
1:
  %2 = alloca ptr
  store ptr %0, ptr %2
  %3 = load ptr, ptr %2
  call void @free(ptr %3)
  ret void
}

define i32 @bitset_test(ptr %0, i32 %1) {
2:
  %3 = alloca ptr
  %4 = alloca i32
  store ptr %0, ptr %3
  store i32 %1, ptr %4
  %5 = load ptr, ptr %3
  %6 = load i32, ptr %4
  %7 = sdiv i32 %6, 64
  %8 = sext i32 %7 to i64
  %9 = getelementptr i64, ptr %5, i64 %8
  %10 = load i64, ptr %9
  %11 = load i32, ptr %4
  %12 = srem i32 %11, 64
  %13 = zext i32 %12 to i64
  %14 = lshr i64 %10, %13
  %15 = and i64 %14, 1
  %16 = trunc i64 %15 to i32
  ret i32 %16
}

define void @bitset_set(ptr %0, i32 %1) {
2:
  %3 = alloca ptr
  %4 = alloca i32
  store ptr %0, ptr %3
  store i32 %1, ptr %4
  %5 = load i32, ptr %4
  %6 = srem i32 %5, 64
  %7 = zext i32 %6 to i64
  %8 = shl i64 1, %7
  %9 = load ptr, ptr %3
  %10 = load i32, ptr %4
  %11 = sdiv i32 %10, 64
  %12 = sext i32 %11 to i64
  %13 = getelementptr i64, ptr %9, i64 %12
  %14 = load i64, ptr %13
  %15 = or i64 %14, %8
  store i64 %15, ptr %13
  ret void
}

define void @bitset_clear(ptr %0, i32 %1) {
2:
  %3 = alloca ptr
  %4 = alloca i32
  store ptr %0, ptr %3
  store i32 %1, ptr %4
  %5 = load i32, ptr %4
  %6 = srem i32 %5, 64
  %7 = zext i32 %6 to i64
  %8 = shl i64 1, %7
  %9 = xor i64 %8, -1
  %10 = load ptr, ptr %3
  %11 = load i32, ptr %4
  %12 = sdiv i32 %11, 64
  %13 = sext i32 %12 to i64
  %14 = getelementptr i64, ptr %10, i64 %13
  %15 = load i64, ptr %14
  %16 = and i64 %15, %9
  store i64 %16, ptr %14
  ret void
}

define i32 @main() {
0:
  %1 = call ptr @bitset_create(i32 256)
  call void @bitset_set(ptr %1, i32 0)
  call void @bitset_set(ptr %1, i32 32)
  call void @bitset_set(ptr %1, i32 63)
  call void @bitset_set(ptr %1, i32 64)
  call void @bitset_set(ptr %1, i32 96)
  call void @bitset_set(ptr %1, i32 127)
  call void @bitset_set(ptr %1, i32 128)
  call void @bitset_set(ptr %1, i32 160)
  call void @bitset_set(ptr %1, i32 191)
  call void @bitset_set(ptr %1, i32 192)
  call void @bitset_set(ptr %1, i32 224)
  call void @bitset_set(ptr %1, i32 255)
  %2 = call i32 @bitset_test(ptr %1, i32 0)
  %3 = call i32 (ptr, ...) @printf(ptr @format, i32 %2)
  %4 = call i32 @bitset_test(ptr %1, i32 1)
  %5 = call i32 (ptr, ...) @printf(ptr @format, i32 %4)
  %6 = call i32 @bitset_test(ptr %1, i32 31)
  %7 = call i32 (ptr, ...) @printf(ptr @format, i32 %6)
  %8 = call i32 @bitset_test(ptr %1, i32 32)
  %9 = call i32 (ptr, ...) @printf(ptr @format, i32 %8)
  %10 = call i32 @bitset_test(ptr %1, i32 33)
  %11 = call i32 (ptr, ...) @printf(ptr @format, i32 %10)
  %12 = call i32 @bitset_test(ptr %1, i32 62)
  %13 = call i32 (ptr, ...) @printf(ptr @format, i32 %12)
  %14 = call i32 @bitset_test(ptr %1, i32 63)
  %15 = call i32 (ptr, ...) @printf(ptr @format, i32 %14)
  %16 = call i32 @bitset_test(ptr %1, i32 64)
  %17 = call i32 (ptr, ...) @printf(ptr @format, i32 %16)
  %18 = call i32 @bitset_test(ptr %1, i32 65)
  %19 = call i32 (ptr, ...) @printf(ptr @format, i32 %18)
  %20 = call i32 @bitset_test(ptr %1, i32 95)
  %21 = call i32 (ptr, ...) @printf(ptr @format, i32 %20)
  %22 = call i32 @bitset_test(ptr %1, i32 96)
  %23 = call i32 (ptr, ...) @printf(ptr @format, i32 %22)
  %24 = call i32 @bitset_test(ptr %1, i32 97)
  %25 = call i32 (ptr, ...) @printf(ptr @format, i32 %24)
  %26 = call i32 @bitset_test(ptr %1, i32 126)
  %27 = call i32 (ptr, ...) @printf(ptr @format, i32 %26)
  %28 = call i32 @bitset_test(ptr %1, i32 127)
  %29 = call i32 (ptr, ...) @printf(ptr @format, i32 %28)
  %30 = call i32 @bitset_test(ptr %1, i32 128)
  %31 = call i32 (ptr, ...) @printf(ptr @format, i32 %30)
  %32 = call i32 @bitset_test(ptr %1, i32 129)
  %33 = call i32 (ptr, ...) @printf(ptr @format, i32 %32)
  %34 = call i32 @bitset_test(ptr %1, i32 159)
  %35 = call i32 (ptr, ...) @printf(ptr @format, i32 %34)
  %36 = call i32 @bitset_test(ptr %1, i32 160)
  %37 = call i32 (ptr, ...) @printf(ptr @format, i32 %36)
  %38 = call i32 @bitset_test(ptr %1, i32 161)
  %39 = call i32 (ptr, ...) @printf(ptr @format, i32 %38)
  %40 = call i32 @bitset_test(ptr %1, i32 190)
  %41 = call i32 (ptr, ...) @printf(ptr @format, i32 %40)
  %42 = call i32 @bitset_test(ptr %1, i32 191)
  %43 = call i32 (ptr, ...) @printf(ptr @format, i32 %42)
  %44 = call i32 @bitset_test(ptr %1, i32 192)
  %45 = call i32 (ptr, ...) @printf(ptr @format, i32 %44)
  %46 = call i32 @bitset_test(ptr %1, i32 193)
  %47 = call i32 (ptr, ...) @printf(ptr @format, i32 %46)
  %48 = call i32 @bitset_test(ptr %1, i32 223)
  %49 = call i32 (ptr, ...) @printf(ptr @format, i32 %48)
  %50 = call i32 @bitset_test(ptr %1, i32 224)
  %51 = call i32 (ptr, ...) @printf(ptr @format, i32 %50)
  %52 = call i32 @bitset_test(ptr %1, i32 225)
  %53 = call i32 (ptr, ...) @printf(ptr @format, i32 %52)
  %54 = call i32 @bitset_test(ptr %1, i32 254)
  %55 = call i32 (ptr, ...) @printf(ptr @format, i32 %54)
  %56 = call i32 @bitset_test(ptr %1, i32 255)
  %57 = call i32 (ptr, ...) @printf(ptr @format, i32 %56)
  call void @bitset_clear(ptr %1, i32 64)
  call void @bitset_clear(ptr %1, i32 160)
  call void @bitset_clear(ptr %1, i32 255)
  %58 = call i32 @bitset_test(ptr %1, i32 0)
  %59 = call i32 (ptr, ...) @printf(ptr @format, i32 %58)
  %60 = call i32 @bitset_test(ptr %1, i32 1)
  %61 = call i32 (ptr, ...) @printf(ptr @format, i32 %60)
  %62 = call i32 @bitset_test(ptr %1, i32 31)
  %63 = call i32 (ptr, ...) @printf(ptr @format, i32 %62)
  %64 = call i32 @bitset_test(ptr %1, i32 32)
  %65 = call i32 (ptr, ...) @printf(ptr @format, i32 %64)
  %66 = call i32 @bitset_test(ptr %1, i32 33)
  %67 = call i32 (ptr, ...) @printf(ptr @format, i32 %66)
  %68 = call i32 @bitset_test(ptr %1, i32 62)
  %69 = call i32 (ptr, ...) @printf(ptr @format, i32 %68)
  %70 = call i32 @bitset_test(ptr %1, i32 63)
  %71 = call i32 (ptr, ...) @printf(ptr @format, i32 %70)
  %72 = call i32 @bitset_test(ptr %1, i32 64)
  %73 = call i32 (ptr, ...) @printf(ptr @format, i32 %72)
  %74 = call i32 @bitset_test(ptr %1, i32 65)
  %75 = call i32 (ptr, ...) @printf(ptr @format, i32 %74)
  %76 = call i32 @bitset_test(ptr %1, i32 95)
  %77 = call i32 (ptr, ...) @printf(ptr @format, i32 %76)
  %78 = call i32 @bitset_test(ptr %1, i32 96)
  %79 = call i32 (ptr, ...) @printf(ptr @format, i32 %78)
  %80 = call i32 @bitset_test(ptr %1, i32 97)
  %81 = call i32 (ptr, ...) @printf(ptr @format, i32 %80)
  %82 = call i32 @bitset_test(ptr %1, i32 126)
  %83 = call i32 (ptr, ...) @printf(ptr @format, i32 %82)
  %84 = call i32 @bitset_test(ptr %1, i32 127)
  %85 = call i32 (ptr, ...) @printf(ptr @format, i32 %84)
  %86 = call i32 @bitset_test(ptr %1, i32 128)
  %87 = call i32 (ptr, ...) @printf(ptr @format, i32 %86)
  %88 = call i32 @bitset_test(ptr %1, i32 129)
  %89 = call i32 (ptr, ...) @printf(ptr @format, i32 %88)
  %90 = call i32 @bitset_test(ptr %1, i32 159)
  %91 = call i32 (ptr, ...) @printf(ptr @format, i32 %90)
  %92 = call i32 @bitset_test(ptr %1, i32 160)
  %93 = call i32 (ptr, ...) @printf(ptr @format, i32 %92)
  %94 = call i32 @bitset_test(ptr %1, i32 161)
  %95 = call i32 (ptr, ...) @printf(ptr @format, i32 %94)
  %96 = call i32 @bitset_test(ptr %1, i32 190)
  %97 = call i32 (ptr, ...) @printf(ptr @format, i32 %96)
  %98 = call i32 @bitset_test(ptr %1, i32 191)
  %99 = call i32 (ptr, ...) @printf(ptr @format, i32 %98)
  %100 = call i32 @bitset_test(ptr %1, i32 192)
  %101 = call i32 (ptr, ...) @printf(ptr @format, i32 %100)
  %102 = call i32 @bitset_test(ptr %1, i32 193)
  %103 = call i32 (ptr, ...) @printf(ptr @format, i32 %102)
  %104 = call i32 @bitset_test(ptr %1, i32 223)
  %105 = call i32 (ptr, ...) @printf(ptr @format, i32 %104)
  %106 = call i32 @bitset_test(ptr %1, i32 224)
  %107 = call i32 (ptr, ...) @printf(ptr @format, i32 %106)
  %108 = call i32 @bitset_test(ptr %1, i32 225)
  %109 = call i32 (ptr, ...) @printf(ptr @format, i32 %108)
  %110 = call i32 @bitset_test(ptr %1, i32 254)
  %111 = call i32 (ptr, ...) @printf(ptr @format, i32 %110)
  %112 = call i32 @bitset_test(ptr %1, i32 255)
  %113 = call i32 (ptr, ...) @printf(ptr @format, i32 %112)
  call void @bitset_destroy(ptr %1)
  ret i32 0
}
