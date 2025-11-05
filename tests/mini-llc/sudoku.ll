@board = private constant [81 x i32] [
  i32 5, i32 3, i32 0, i32 0, i32 7, i32 0, i32 0, i32 0, i32 0,
  i32 6, i32 0, i32 0, i32 1, i32 9, i32 5, i32 0, i32 0, i32 0,
  i32 0, i32 9, i32 8, i32 0, i32 0, i32 0, i32 0, i32 6, i32 0,
  i32 8, i32 0, i32 0, i32 0, i32 6, i32 0, i32 0, i32 0, i32 3,
  i32 4, i32 0, i32 0, i32 8, i32 0, i32 3, i32 0, i32 0, i32 1,
  i32 7, i32 0, i32 0, i32 0, i32 2, i32 0, i32 0, i32 0, i32 6,
  i32 0, i32 6, i32 0, i32 0, i32 0, i32 0, i32 2, i32 8, i32 0,
  i32 0, i32 0, i32 0, i32 4, i32 1, i32 9, i32 0, i32 0, i32 5,
  i32 0, i32 0, i32 0, i32 0, i32 8, i32 0, i32 0, i32 7, i32 9
]

declare i32 @putchar(i32)
declare ptr @memset(ptr, i32, i64) argmemonly
declare ptr @memcpy(ptr, ptr, i64) argmemonly

define void @print_board(ptr %0) {
1:
  %2 = alloca ptr
  %3 = alloca i32
  %4 = alloca i32
  store ptr %0, ptr %2
  store i32 0, ptr %3
  br label %5

5:
  %6 = load i32, ptr %3
  %7 = icmp slt i32 %6, 9
  br i1 %7, label %8, label %31

8:
  store i32 0, ptr %4
  br label %9

9:
  %10 = load i32, ptr %4
  %11 = icmp slt i32 %10, 9
  br i1 %11, label %12, label %26

12:
  %13 = load ptr, ptr %2
  %14 = load i32, ptr %3
  %15 = mul i32 %14, 9
  %16 = load i32, ptr %4
  %17 = add i32 %15, %16
  %18 = sext i32 %17 to i64
  %19 = getelementptr i32, ptr %13, i64 %18
  %20 = load i32, ptr %19
  %21 = add i32 48, %20
  %22 = call i32 @putchar(i32 %21)
  br label %23

23:
  %24 = load i32, ptr %4
  %25 = add i32 %24, 1
  store i32 %25, ptr %4
  br label %9

26:
  %27 = call i32 @putchar(i32 10)
  br label %28

28:
  %29 = load i32, ptr %3
  %30 = add i32 %29, 1
  store i32 %30, ptr %3
  br label %5

31:
  ret void
}

define i32 @backtrack(ptr %0, i32 %1, i32 %2, ptr %3, ptr %4, ptr %5) {
6:
  %7 = alloca i32
  %8 = alloca ptr
  %9 = alloca i32
  %10 = alloca i32
  %11 = alloca ptr
  %12 = alloca ptr
  %13 = alloca ptr
  %14 = alloca i32
  %15 = alloca i32
  store ptr %0, ptr %8
  store i32 %1, ptr %9
  store i32 %2, ptr %10
  store ptr %3, ptr %11
  store ptr %4, ptr %12
  store ptr %5, ptr %13
  %16 = load i32, ptr %9
  %17 = icmp eq i32 %16, 9
  br i1 %17, label %18, label %19

18:
  store i32 1, ptr %7
  br label %181

19:
  %20 = load i32, ptr %10
  %21 = icmp eq i32 %20, 9
  br i1 %21, label %22, label %30

22:
  %23 = load ptr, ptr %8
  %24 = load i32, ptr %9
  %25 = add i32 %24, 1
  %26 = load ptr, ptr %11
  %27 = load ptr, ptr %12
  %28 = load ptr, ptr %13
  %29 = call i32 @backtrack(ptr %23, i32 %25, i32 0, ptr %26, ptr %27, ptr %28)
  store i32 %29, ptr %7
  br label %181

30:
  %31 = load ptr, ptr %8
  %32 = load i32, ptr %9
  %33 = mul i32 %32, 9
  %34 = load i32, ptr %10
  %35 = add i32 %33, %34
  %36 = sext i32 %35 to i64
  %37 = getelementptr i32, ptr %31, i64 %36
  %38 = load i32, ptr %37
  %39 = icmp ne i32 %38, 0
  br i1 %39, label %40, label %49

40:
  %41 = load ptr, ptr %8
  %42 = load i32, ptr %9
  %43 = load i32, ptr %10
  %44 = add i32 %43, 1
  %45 = load ptr, ptr %11
  %46 = load ptr, ptr %12
  %47 = load ptr, ptr %13
  %48 = call i32 @backtrack(ptr %41, i32 %42, i32 %44, ptr %45, ptr %46, ptr %47)
  store i32 %48, ptr %7
  br label %181

49:
  %50 = load ptr, ptr %11
  %51 = load i32, ptr %9
  %52 = sext i32 %51 to i64
  %53 = getelementptr i32, ptr %50, i64 %52
  %54 = load i32, ptr %53
  %55 = load ptr, ptr %12
  %56 = load i32, ptr %10
  %57 = sext i32 %56 to i64
  %58 = getelementptr i32, ptr %55, i64 %57
  %59 = load i32, ptr %58
  %60 = or i32 %54, %59
  %61 = load ptr, ptr %13
  %62 = load i32, ptr %9
  %63 = sdiv i32 %62, 3
  %64 = mul i32 %63, 3
  %65 = load i32, ptr %10
  %66 = sdiv i32 %65, 3
  %67 = add i32 %64, %66
  %68 = sext i32 %67 to i64
  %69 = getelementptr i32, ptr %61, i64 %68
  %70 = load i32, ptr %69
  %71 = or i32 %60, %70
  store i32 %71, ptr %14
  store i32 1, ptr %15
  br label %72

72:
  %73 = load i32, ptr %15
  %74 = icmp sle i32 %73, 9
  br i1 %74, label %75, label %180

75:
  %76 = load i32, ptr %14
  %77 = load i32, ptr %15
  %78 = sub i32 %77, 1
  %79 = ashr i32 %76, %78
  %80 = and i32 %79, 1
  %81 = icmp ne i32 %80, 0
  br i1 %81, label %176, label %82

82:
  %83 = load i32, ptr %15
  %84 = load ptr, ptr %8
  %85 = load i32, ptr %9
  %86 = mul i32 %85, 9
  %87 = load i32, ptr %10
  %88 = add i32 %86, %87
  %89 = sext i32 %88 to i64
  %90 = getelementptr i32, ptr %84, i64 %89
  store i32 %83, ptr %90
  %91 = load i32, ptr %15
  %92 = sub i32 %91, 1
  %93 = shl i32 1, %92
  %94 = load ptr, ptr %11
  %95 = load i32, ptr %9
  %96 = sext i32 %95 to i64
  %97 = getelementptr i32, ptr %94, i64 %96
  %98 = load i32, ptr %97
  %99 = or i32 %98, %93
  store i32 %99, ptr %97
  %100 = load i32, ptr %15
  %101 = sub i32 %100, 1
  %102 = shl i32 1, %101
  %103 = load ptr, ptr %12
  %104 = load i32, ptr %10
  %105 = sext i32 %104 to i64
  %106 = getelementptr i32, ptr %103, i64 %105
  %107 = load i32, ptr %106
  %108 = or i32 %107, %102
  store i32 %108, ptr %106
  %109 = load i32, ptr %15
  %110 = sub i32 %109, 1
  %111 = shl i32 1, %110
  %112 = load ptr, ptr %13
  %113 = load i32, ptr %9
  %114 = sdiv i32 %113, 3
  %115 = mul i32 %114, 3
  %116 = load i32, ptr %10
  %117 = sdiv i32 %116, 3
  %118 = add i32 %115, %117
  %119 = sext i32 %118 to i64
  %120 = getelementptr i32, ptr %112, i64 %119
  %121 = load i32, ptr %120
  %122 = or i32 %121, %111
  store i32 %122, ptr %120
  %123 = load ptr, ptr %8
  %124 = load i32, ptr %9
  %125 = load i32, ptr %10
  %126 = add i32 %125, 1
  %127 = load ptr, ptr %11
  %128 = load ptr, ptr %12
  %129 = load ptr, ptr %13
  %130 = call i32 @backtrack(ptr %123, i32 %124, i32 %126, ptr %127, ptr %128, ptr %129)
  %131 = icmp ne i32 %130, 0
  br i1 %131, label %132, label %133

132:
  store i32 1, ptr %7
  br label %181

133:
  %134 = load ptr, ptr %8
  %135 = load i32, ptr %9
  %136 = mul i32 %135, 9
  %137 = load i32, ptr %10
  %138 = add i32 %136, %137
  %139 = sext i32 %138 to i64
  %140 = getelementptr i32, ptr %134, i64 %139
  store i32 0, ptr %140
  %141 = load i32, ptr %15
  %142 = sub i32 %141, 1
  %143 = shl i32 1, %142
  %144 = xor i32 %143, -1
  %145 = load ptr, ptr %11
  %146 = load i32, ptr %9
  %147 = sext i32 %146 to i64
  %148 = getelementptr i32, ptr %145, i64 %147
  %149 = load i32, ptr %148
  %150 = and i32 %149, %144
  store i32 %150, ptr %148
  %151 = load i32, ptr %15
  %152 = sub i32 %151, 1
  %153 = shl i32 1, %152
  %154 = xor i32 %153, -1
  %155 = load ptr, ptr %12
  %156 = load i32, ptr %10
  %157 = sext i32 %156 to i64
  %158 = getelementptr i32, ptr %155, i64 %157
  %159 = load i32, ptr %158
  %160 = and i32 %159, %154
  store i32 %160, ptr %158
  %161 = load i32, ptr %15
  %162 = sub i32 %161, 1
  %163 = shl i32 1, %162
  %164 = xor i32 %163, -1
  %165 = load ptr, ptr %13
  %166 = load i32, ptr %9
  %167 = sdiv i32 %166, 3
  %168 = mul i32 %167, 3
  %169 = load i32, ptr %10
  %170 = sdiv i32 %169, 3
  %171 = add i32 %168, %170
  %172 = sext i32 %171 to i64
  %173 = getelementptr i32, ptr %165, i64 %172
  %174 = load i32, ptr %173
  %175 = and i32 %174, %164
  store i32 %175, ptr %173
  br label %176

176:
  br label %177

177:
  %178 = load i32, ptr %15
  %179 = add i32 %178, 1
  store i32 %179, ptr %15
  br label %72

180:
  store i32 0, ptr %7
  br label %181

181:
  %182 = load i32, ptr %7
  ret i32 %182
}

define i32 @solve(ptr %0) {
1:
  %2 = alloca ptr
  %3 = alloca [9 x i32]
  %4 = alloca [9 x i32]
  %5 = alloca [9 x i32]
  %6 = alloca i32
  %7 = alloca i32
  %8 = alloca i32
  store ptr %0, ptr %2
  %9 = getelementptr [9 x i32], ptr %3, i64 0, i64 0
  %10 = call ptr @memset(ptr %9, i32 0, i64 36)
  %11 = getelementptr [9 x i32], ptr %4, i64 0, i64 0
  %12 = call ptr @memset(ptr %11, i32 0, i64 36)
  %13 = getelementptr [9 x i32], ptr %5, i64 0, i64 0
  %14 = call ptr @memset(ptr %13, i32 0, i64 36)
  store i32 0, ptr %6
  br label %15

15:
  %16 = load i32, ptr %6
  %17 = icmp slt i32 %16, 9
  br i1 %17, label %18, label %71

18:
  store i32 0, ptr %7
  br label %19

19:
  %20 = load i32, ptr %7
  %21 = icmp slt i32 %20, 9
  br i1 %21, label %22, label %67

22:
  %23 = load ptr, ptr %2
  %24 = load i32, ptr %6
  %25 = mul i32 %24, 9
  %26 = load i32, ptr %7
  %27 = add i32 %25, %26
  %28 = sext i32 %27 to i64
  %29 = getelementptr i32, ptr %23, i64 %28
  %30 = load i32, ptr %29
  store i32 %30, ptr %8
  %31 = load i32, ptr %8
  %32 = icmp ne i32 %31, 0
  br i1 %32, label %33, label %63

33:
  %34 = load i32, ptr %8
  %35 = sub i32 %34, 1
  %36 = shl i32 1, %35
  %37 = load i32, ptr %6
  %38 = sext i32 %37 to i64
  %39 = getelementptr [9 x i32], ptr %3, i64 0, i64 %38
  %40 = load i32, ptr %39
  %41 = or i32 %40, %36
  store i32 %41, ptr %39
  %42 = load i32, ptr %8
  %43 = sub i32 %42, 1
  %44 = shl i32 1, %43
  %45 = load i32, ptr %7
  %46 = sext i32 %45 to i64
  %47 = getelementptr [9 x i32], ptr %4, i64 0, i64 %46
  %48 = load i32, ptr %47
  %49 = or i32 %48, %44
  store i32 %49, ptr %47
  %50 = load i32, ptr %8
  %51 = sub i32 %50, 1
  %52 = shl i32 1, %51
  %53 = load i32, ptr %6
  %54 = sdiv i32 %53, 3
  %55 = mul i32 %54, 3
  %56 = load i32, ptr %7
  %57 = sdiv i32 %56, 3
  %58 = add i32 %55, %57
  %59 = sext i32 %58 to i64
  %60 = getelementptr [9 x i32], ptr %5, i64 0, i64 %59
  %61 = load i32, ptr %60
  %62 = or i32 %61, %52
  store i32 %62, ptr %60
  br label %63

63:
  br label %64

64:
  %65 = load i32, ptr %7
  %66 = add i32 %65, 1
  store i32 %66, ptr %7
  br label %19

67:
  br label %68

68:
  %69 = load i32, ptr %6
  %70 = add i32 %69, 1
  store i32 %70, ptr %6
  br label %15

71:
  %72 = load ptr, ptr %2
  %73 = getelementptr [9 x i32], ptr %3, i64 0, i64 0
  %74 = getelementptr [9 x i32], ptr %4, i64 0, i64 0
  %75 = getelementptr [9 x i32], ptr %5, i64 0, i64 0
  %76 = call i32 @backtrack(ptr %72, i32 0, i32 0, ptr %73, ptr %74, ptr %75)
  ret i32 %76
}

define i32 @main() {
0:
  %1 = alloca [81 x i32]
  %2 = call ptr @memcpy(ptr %1, ptr @board, i64 324)
  %3 = call i32 @solve(ptr %1)
  call void @print_board(ptr %1)
  ret i32 0
}
