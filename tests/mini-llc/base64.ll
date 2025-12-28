@base64_table = constant [65 x i8] c"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/\00"
@data0 = private constant [1 x i8] c"\00"
@data1 = private constant [2 x i8] c"f\00"
@data2 = private constant [3 x i8] c"fo\00"
@data3 = private constant [4 x i8] c"foo\00"
@data4 = private constant [5 x i8] c"foob\00"
@data5 = private constant [6 x i8] c"fooba\00"
@data6 = private constant [7 x i8] c"foobar\00"
@format = private constant [4 x i8] c"%s\0A\00"

declare i32 @printf(ptr, ...)

define void @base64_encode(ptr %0, i32 %1, ptr %2) {
3:
  %4 = alloca ptr
  %5 = alloca i32
  %6 = alloca ptr
  %7 = alloca i32
  %8 = alloca i32
  %9 = alloca i32
  %10 = alloca i32
  %11 = alloca i32
  %12 = alloca i32
  %13 = alloca i32
  store ptr %0, ptr %4
  store i32 %1, ptr %5
  store ptr %2, ptr %6
  store i32 0, ptr %7
  store i32 0, ptr %8
  br label %14

14:
  %15 = load i32, ptr %7
  %16 = load i32, ptr %5
  %17 = icmp slt i32 %15, %16
  br i1 %17, label %18, label %111

18:
  %19 = load i32, ptr %7
  %20 = load i32, ptr %5
  %21 = icmp slt i32 %19, %20
  br i1 %21, label %22, label %30

22:
  %23 = load ptr, ptr %4
  %24 = load i32, ptr %7
  %25 = add i32 %24, 1
  store i32 %25, ptr %7
  %26 = sext i32 %24 to i64
  %27 = getelementptr i8, ptr %23, i64 %26
  %28 = load i8, ptr %27
  %29 = zext i8 %28 to i32
  br label %31

30:
  br label %31

31:
  %32 = phi i32 [ %29, %22 ], [ 0, %30 ]
  store i32 %32, ptr %10
  %33 = load i32, ptr %7
  %34 = load i32, ptr %5
  %35 = icmp slt i32 %33, %34
  br i1 %35, label %36, label %44

36:
  %37 = load ptr, ptr %4
  %38 = load i32, ptr %7
  %39 = add i32 %38, 1
  store i32 %39, ptr %7
  %40 = sext i32 %38 to i64
  %41 = getelementptr i8, ptr %37, i64 %40
  %42 = load i8, ptr %41
  %43 = zext i8 %42 to i32
  br label %45

44:
  br label %45

45:
  %46 = phi i32 [ %43, %36 ], [ 0, %44 ]
  store i32 %46, ptr %11
  %47 = load i32, ptr %7
  %48 = load i32, ptr %5
  %49 = icmp slt i32 %47, %48
  br i1 %49, label %50, label %58

50:
  %51 = load ptr, ptr %4
  %52 = load i32, ptr %7
  %53 = add i32 %52, 1
  store i32 %53, ptr %7
  %54 = sext i32 %52 to i64
  %55 = getelementptr i8, ptr %51, i64 %54
  %56 = load i8, ptr %55
  %57 = zext i8 %56 to i32
  br label %59

58:
  br label %59

59:
  %60 = phi i32 [ %57, %50 ], [ 0, %58 ]
  store i32 %60, ptr %12
  %61 = load i32, ptr %10
  %62 = shl i32 %61, 16
  %63 = load i32, ptr %11
  %64 = shl i32 %63, 8
  %65 = or i32 %62, %64
  %66 = load i32, ptr %12
  %67 = or i32 %65, %66
  store i32 %67, ptr %13
  %68 = load i32, ptr %13
  %69 = lshr i32 %68, 18
  %70 = and i32 %69, 63
  %71 = zext i32 %70 to i64
  %72 = getelementptr [65 x i8], ptr @base64_table, i64 0, i64 %71
  %73 = load i8, ptr %72
  %74 = load ptr, ptr %6
  %75 = load i32, ptr %8
  %76 = add i32 %75, 1
  store i32 %76, ptr %8
  %77 = sext i32 %75 to i64
  %78 = getelementptr i8, ptr %74, i64 %77
  store i8 %73, ptr %78
  %79 = load i32, ptr %13
  %80 = lshr i32 %79, 12
  %81 = and i32 %80, 63
  %82 = zext i32 %81 to i64
  %83 = getelementptr [65 x i8], ptr @base64_table, i64 0, i64 %82
  %84 = load i8, ptr %83
  %85 = load ptr, ptr %6
  %86 = load i32, ptr %8
  %87 = add i32 %86, 1
  store i32 %87, ptr %8
  %88 = sext i32 %86 to i64
  %89 = getelementptr i8, ptr %85, i64 %88
  store i8 %84, ptr %89
  %90 = load i32, ptr %13
  %91 = lshr i32 %90, 6
  %92 = and i32 %91, 63
  %93 = zext i32 %92 to i64
  %94 = getelementptr [65 x i8], ptr @base64_table, i64 0, i64 %93
  %95 = load i8, ptr %94
  %96 = load ptr, ptr %6
  %97 = load i32, ptr %8
  %98 = add i32 %97, 1
  store i32 %98, ptr %8
  %99 = sext i32 %97 to i64
  %100 = getelementptr i8, ptr %96, i64 %99
  store i8 %95, ptr %100
  %101 = load i32, ptr %13
  %102 = and i32 %101, 63
  %103 = zext i32 %102 to i64
  %104 = getelementptr [65 x i8], ptr @base64_table, i64 0, i64 %103
  %105 = load i8, ptr %104
  %106 = load ptr, ptr %6
  %107 = load i32, ptr %8
  %108 = add i32 %107, 1
  store i32 %108, ptr %8
  %109 = sext i32 %107 to i64
  %110 = getelementptr i8, ptr %106, i64 %109
  store i8 %105, ptr %110
  br label %14

111:
  store i32 0, ptr %9
  br label %112

112:
  %113 = load i32, ptr %9
  %114 = load i32, ptr %5
  %115 = srem i32 %114, 3
  %116 = sub i32 3, %115
  %117 = srem i32 %116, 3
  %118 = icmp slt i32 %113, %117
  br i1 %118, label %119, label %130

119:
  %120 = load ptr, ptr %6
  %121 = load i32, ptr %8
  %122 = sub i32 %121, 1
  %123 = load i32, ptr %9
  %124 = sub i32 %122, %123
  %125 = sext i32 %124 to i64
  %126 = getelementptr i8, ptr %120, i64 %125
  store i8 61, ptr %126
  br label %127

127:
  %128 = load i32, ptr %9
  %129 = add i32 %128, 1
  store i32 %129, ptr %9
  br label %112

130:
  %131 = load ptr, ptr %6
  %132 = load i32, ptr %8
  %133 = sext i32 %132 to i64
  %134 = getelementptr i8, ptr %131, i64 %133
  store i8 0, ptr %134
  ret void
}

define i32 @main() {
0:
  %1 = alloca [256 x i8]
  call void @base64_encode(ptr @data0, i32 0, ptr %1)
  %2 = call i32 (ptr, ...) @printf(ptr @format, ptr %1)
  call void @base64_encode(ptr @data1, i32 1, ptr %1)
  %3 = call i32 (ptr, ...) @printf(ptr @format, ptr %1)
  call void @base64_encode(ptr @data2, i32 2, ptr %1)
  %4 = call i32 (ptr, ...) @printf(ptr @format, ptr %1)
  call void @base64_encode(ptr @data3, i32 3, ptr %1)
  %5 = call i32 (ptr, ...) @printf(ptr @format, ptr %1)
  call void @base64_encode(ptr @data4, i32 4, ptr %1)
  %6 = call i32 (ptr, ...) @printf(ptr @format, ptr %1)
  call void @base64_encode(ptr @data5, i32 5, ptr %1)
  %7 = call i32 (ptr, ...) @printf(ptr @format, ptr %1)
  call void @base64_encode(ptr @data6, i32 6, ptr %1)
  %8 = call i32 (ptr, ...) @printf(ptr @format, ptr %1)
  ret i32 0
}
