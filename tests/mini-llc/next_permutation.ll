@a = private constant [4 x i32] [i32 1, i32 1, i32 2, i32 3]
@format1 = private constant [4 x i8] c"%d\0A\00"
@format2 = private constant [13 x i8] c"%d %d %d %d\0A\00"

declare ptr @memcpy(ptr, ptr, i64) argmemonly
declare i32 @printf(ptr, ...)

define void @swap(ptr %0, ptr %1) {
2:
  %3 = alloca ptr
  %4 = alloca ptr
  %5 = alloca i32
  store ptr %0, ptr %3
  store ptr %1, ptr %4
  %6 = load ptr, ptr %3
  %7 = load i32, ptr %6
  store i32 %7, ptr %5
  %8 = load ptr, ptr %4
  %9 = load i32, ptr %8
  %10 = load ptr, ptr %3
  store i32 %9, ptr %10
  %11 = load i32, ptr %5
  %12 = load ptr, ptr %4
  store i32 %11, ptr %12
  ret void
}

define void @reverse(ptr %0, i32 %1, i32 %2) {
3:
  %4 = alloca ptr
  %5 = alloca i32
  %6 = alloca i32
  store ptr %0, ptr %4
  store i32 %1, ptr %5
  store i32 %2, ptr %6
  br label %7

7:
  %8 = load i32, ptr %5
  %9 = load i32, ptr %6
  %10 = icmp slt i32 %8, %9
  br i1 %10, label %11, label %24

11:
  %12 = load ptr, ptr %4
  %13 = load i32, ptr %5
  %14 = sext i32 %13 to i64
  %15 = getelementptr i32, ptr %12, i64 %14
  %16 = load ptr, ptr %4
  %17 = load i32, ptr %6
  %18 = sext i32 %17 to i64
  %19 = getelementptr i32, ptr %16, i64 %18
  call void @swap(ptr %15, ptr %19)
  %20 = load i32, ptr %5
  %21 = add i32 %20, 1
  store i32 %21, ptr %5
  %22 = load i32, ptr %6
  %23 = add i32 %22, -1
  store i32 %23, ptr %6
  br label %7

24:
  ret void
}

define i32 @next_permutation(ptr %0, i32 %1) {
2:
  %3 = alloca ptr
  %4 = alloca i32
  %5 = alloca i32
  %6 = alloca i32
  store ptr %0, ptr %3
  store i32 %1, ptr %4
  %7 = load i32, ptr %4
  %8 = sub i32 %7, 2
  store i32 %8, ptr %5
  br label %9

9:
  %10 = load i32, ptr %5
  %11 = icmp sge i32 %10, 0
  br i1 %11, label %12, label %25

12:
  %13 = load ptr, ptr %3
  %14 = load i32, ptr %5
  %15 = sext i32 %14 to i64
  %16 = getelementptr i32, ptr %13, i64 %15
  %17 = load i32, ptr %16
  %18 = load ptr, ptr %3
  %19 = load i32, ptr %5
  %20 = add i32 %19, 1
  %21 = sext i32 %20 to i64
  %22 = getelementptr i32, ptr %18, i64 %21
  %23 = load i32, ptr %22
  %24 = icmp sge i32 %17, %23
  br label %25

25:
  %26 = phi i1 [ false, %9 ], [ %24, %12 ]
  br i1 %26, label %27, label %30

27:
  %28 = load i32, ptr %5
  %29 = add i32 %28, -1
  store i32 %29, ptr %5
  br label %9

30:
  %31 = load i32, ptr %5
  %32 = icmp sge i32 %31, 0
  br i1 %32, label %33, label %60

33:
  %34 = load i32, ptr %4
  %35 = sub i32 %34, 1
  store i32 %35, ptr %6
  br label %36

36:
  %37 = load ptr, ptr %3
  %38 = load i32, ptr %6
  %39 = sext i32 %38 to i64
  %40 = getelementptr i32, ptr %37, i64 %39
  %41 = load i32, ptr %40
  %42 = load ptr, ptr %3
  %43 = load i32, ptr %5
  %44 = sext i32 %43 to i64
  %45 = getelementptr i32, ptr %42, i64 %44
  %46 = load i32, ptr %45
  %47 = icmp sle i32 %41, %46
  br i1 %47, label %48, label %51

48:
  %49 = load i32, ptr %6
  %50 = add i32 %49, -1
  store i32 %50, ptr %6
  br label %36

51:
  %52 = load ptr, ptr %3
  %53 = load i32, ptr %5
  %54 = sext i32 %53 to i64
  %55 = getelementptr i32, ptr %52, i64 %54
  %56 = load ptr, ptr %3
  %57 = load i32, ptr %6
  %58 = sext i32 %57 to i64
  %59 = getelementptr i32, ptr %56, i64 %58
  call void @swap(ptr %55, ptr %59)
  br label %60

60:
  %61 = load ptr, ptr %3
  %62 = load i32, ptr %5
  %63 = add i32 %62, 1
  %64 = load i32, ptr %4
  %65 = sub i32 %64, 1
  call void @reverse(ptr %61, i32 %63, i32 %65)
  %66 = load i32, ptr %5
  %67 = icmp sge i32 %66, 0
  %68 = zext i1 %67 to i32
  ret i32 %68
}

define i32 @main() {
0:
  %1 = alloca [4 x i32]
  %2 = call ptr @memcpy(ptr %1, ptr @a, i64 16)
  %3 = call i32 @next_permutation(ptr %1, i32 4)
  %4 = call i32 @printf(ptr @format1, i32 %3)
  %5 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %6 = load i32, ptr %5
  %7 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %8 = load i32, ptr %7
  %9 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %10 = load i32, ptr %9
  %11 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %12 = load i32, ptr %11
  %13 = call i32 @printf(ptr @format2, i32 %6, i32 %8, i32 %10, i32 %12)
  %14 = call i32 @next_permutation(ptr %1, i32 4)
  %15 = call i32 @printf(ptr @format1, i32 %14)
  %16 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %17 = load i32, ptr %16
  %18 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %19 = load i32, ptr %18
  %20 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %21 = load i32, ptr %20
  %22 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %23 = load i32, ptr %22
  %24 = call i32 @printf(ptr @format2, i32 %17, i32 %19, i32 %21, i32 %23)
  %25 = call i32 @next_permutation(ptr %1, i32 4)
  %26 = call i32 @printf(ptr @format1, i32 %25)
  %27 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %28 = load i32, ptr %27
  %29 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %30 = load i32, ptr %29
  %31 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %32 = load i32, ptr %31
  %33 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %34 = load i32, ptr %33
  %35 = call i32 @printf(ptr @format2, i32 %28, i32 %30, i32 %32, i32 %34)
  %36 = call i32 @next_permutation(ptr %1, i32 4)
  %37 = call i32 @printf(ptr @format1, i32 %36)
  %38 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %39 = load i32, ptr %38
  %40 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %41 = load i32, ptr %40
  %42 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %43 = load i32, ptr %42
  %44 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %45 = load i32, ptr %44
  %46 = call i32 @printf(ptr @format2, i32 %39, i32 %41, i32 %43, i32 %45)
  %47 = call i32 @next_permutation(ptr %1, i32 4)
  %48 = call i32 @printf(ptr @format1, i32 %47)
  %49 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %50 = load i32, ptr %49
  %51 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %52 = load i32, ptr %51
  %53 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %54 = load i32, ptr %53
  %55 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %56 = load i32, ptr %55
  %57 = call i32 @printf(ptr @format2, i32 %50, i32 %52, i32 %54, i32 %56)
  %58 = call i32 @next_permutation(ptr %1, i32 4)
  %59 = call i32 @printf(ptr @format1, i32 %58)
  %60 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %61 = load i32, ptr %60
  %62 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %63 = load i32, ptr %62
  %64 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %65 = load i32, ptr %64
  %66 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %67 = load i32, ptr %66
  %68 = call i32 @printf(ptr @format2, i32 %61, i32 %63, i32 %65, i32 %67)
  %69 = call i32 @next_permutation(ptr %1, i32 4)
  %70 = call i32 @printf(ptr @format1, i32 %69)
  %71 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %72 = load i32, ptr %71
  %73 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %74 = load i32, ptr %73
  %75 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %76 = load i32, ptr %75
  %77 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %78 = load i32, ptr %77
  %79 = call i32 @printf(ptr @format2, i32 %72, i32 %74, i32 %76, i32 %78)
  %80 = call i32 @next_permutation(ptr %1, i32 4)
  %81 = call i32 @printf(ptr @format1, i32 %80)
  %82 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %83 = load i32, ptr %82
  %84 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %85 = load i32, ptr %84
  %86 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %87 = load i32, ptr %86
  %88 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %89 = load i32, ptr %88
  %90 = call i32 @printf(ptr @format2, i32 %83, i32 %85, i32 %87, i32 %89)
  %91 = call i32 @next_permutation(ptr %1, i32 4)
  %92 = call i32 @printf(ptr @format1, i32 %91)
  %93 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %94 = load i32, ptr %93
  %95 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %96 = load i32, ptr %95
  %97 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %98 = load i32, ptr %97
  %99 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %100 = load i32, ptr %99
  %101 = call i32 @printf(ptr @format2, i32 %94, i32 %96, i32 %98, i32 %100)
  %102 = call i32 @next_permutation(ptr %1, i32 4)
  %103 = call i32 @printf(ptr @format1, i32 %102)
  %104 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %105 = load i32, ptr %104
  %106 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %107 = load i32, ptr %106
  %108 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %109 = load i32, ptr %108
  %110 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %111 = load i32, ptr %110
  %112 = call i32 @printf(ptr @format2, i32 %105, i32 %107, i32 %109, i32 %111)
  %113 = call i32 @next_permutation(ptr %1, i32 4)
  %114 = call i32 @printf(ptr @format1, i32 %113)
  %115 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %116 = load i32, ptr %115
  %117 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %118 = load i32, ptr %117
  %119 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %120 = load i32, ptr %119
  %121 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %122 = load i32, ptr %121
  %123 = call i32 @printf(ptr @format2, i32 %116, i32 %118, i32 %120, i32 %122)
  %124 = call i32 @next_permutation(ptr %1, i32 4)
  %125 = call i32 @printf(ptr @format1, i32 %124)
  %126 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %127 = load i32, ptr %126
  %128 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %129 = load i32, ptr %128
  %130 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %131 = load i32, ptr %130
  %132 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %133 = load i32, ptr %132
  %134 = call i32 @printf(ptr @format2, i32 %127, i32 %129, i32 %131, i32 %133)
  %135 = call i32 @next_permutation(ptr %1, i32 4)
  %136 = call i32 @printf(ptr @format1, i32 %135)
  %137 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %138 = load i32, ptr %137
  %139 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %140 = load i32, ptr %139
  %141 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %142 = load i32, ptr %141
  %143 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %144 = load i32, ptr %143
  %145 = call i32 @printf(ptr @format2, i32 %138, i32 %140, i32 %142, i32 %144)
  %146 = call i32 @next_permutation(ptr %1, i32 4)
  %147 = call i32 @printf(ptr @format1, i32 %146)
  %148 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %149 = load i32, ptr %148
  %150 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %151 = load i32, ptr %150
  %152 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %153 = load i32, ptr %152
  %154 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %155 = load i32, ptr %154
  %156 = call i32 @printf(ptr @format2, i32 %149, i32 %151, i32 %153, i32 %155)
  %157 = call i32 @next_permutation(ptr %1, i32 4)
  %158 = call i32 @printf(ptr @format1, i32 %157)
  %159 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %160 = load i32, ptr %159
  %161 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %162 = load i32, ptr %161
  %163 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %164 = load i32, ptr %163
  %165 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %166 = load i32, ptr %165
  %167 = call i32 @printf(ptr @format2, i32 %160, i32 %162, i32 %164, i32 %166)
  %168 = call i32 @next_permutation(ptr %1, i32 4)
  %169 = call i32 @printf(ptr @format1, i32 %168)
  %170 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %171 = load i32, ptr %170
  %172 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %173 = load i32, ptr %172
  %174 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %175 = load i32, ptr %174
  %176 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %177 = load i32, ptr %176
  %178 = call i32 @printf(ptr @format2, i32 %171, i32 %173, i32 %175, i32 %177)
  %179 = call i32 @next_permutation(ptr %1, i32 4)
  %180 = call i32 @printf(ptr @format1, i32 %179)
  %181 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %182 = load i32, ptr %181
  %183 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %184 = load i32, ptr %183
  %185 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %186 = load i32, ptr %185
  %187 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %188 = load i32, ptr %187
  %189 = call i32 @printf(ptr @format2, i32 %182, i32 %184, i32 %186, i32 %188)
  %190 = call i32 @next_permutation(ptr %1, i32 4)
  %191 = call i32 @printf(ptr @format1, i32 %190)
  %192 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %193 = load i32, ptr %192
  %194 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %195 = load i32, ptr %194
  %196 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %197 = load i32, ptr %196
  %198 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %199 = load i32, ptr %198
  %200 = call i32 @printf(ptr @format2, i32 %193, i32 %195, i32 %197, i32 %199)
  %201 = call i32 @next_permutation(ptr %1, i32 4)
  %202 = call i32 @printf(ptr @format1, i32 %201)
  %203 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %204 = load i32, ptr %203
  %205 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %206 = load i32, ptr %205
  %207 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %208 = load i32, ptr %207
  %209 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %210 = load i32, ptr %209
  %211 = call i32 @printf(ptr @format2, i32 %204, i32 %206, i32 %208, i32 %210)
  %212 = call i32 @next_permutation(ptr %1, i32 4)
  %213 = call i32 @printf(ptr @format1, i32 %212)
  %214 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %215 = load i32, ptr %214
  %216 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %217 = load i32, ptr %216
  %218 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %219 = load i32, ptr %218
  %220 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %221 = load i32, ptr %220
  %222 = call i32 @printf(ptr @format2, i32 %215, i32 %217, i32 %219, i32 %221)
  %223 = call i32 @next_permutation(ptr %1, i32 4)
  %224 = call i32 @printf(ptr @format1, i32 %223)
  %225 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %226 = load i32, ptr %225
  %227 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %228 = load i32, ptr %227
  %229 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %230 = load i32, ptr %229
  %231 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %232 = load i32, ptr %231
  %233 = call i32 @printf(ptr @format2, i32 %226, i32 %228, i32 %230, i32 %232)
  %234 = call i32 @next_permutation(ptr %1, i32 4)
  %235 = call i32 @printf(ptr @format1, i32 %234)
  %236 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %237 = load i32, ptr %236
  %238 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %239 = load i32, ptr %238
  %240 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %241 = load i32, ptr %240
  %242 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %243 = load i32, ptr %242
  %244 = call i32 @printf(ptr @format2, i32 %237, i32 %239, i32 %241, i32 %243)
  %245 = call i32 @next_permutation(ptr %1, i32 4)
  %246 = call i32 @printf(ptr @format1, i32 %245)
  %247 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %248 = load i32, ptr %247
  %249 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %250 = load i32, ptr %249
  %251 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %252 = load i32, ptr %251
  %253 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %254 = load i32, ptr %253
  %255 = call i32 @printf(ptr @format2, i32 %248, i32 %250, i32 %252, i32 %254)
  %256 = call i32 @next_permutation(ptr %1, i32 4)
  %257 = call i32 @printf(ptr @format1, i32 %256)
  %258 = getelementptr [4 x i32], ptr %1, i64 0, i64 0
  %259 = load i32, ptr %258
  %260 = getelementptr [4 x i32], ptr %1, i64 0, i64 1
  %261 = load i32, ptr %260
  %262 = getelementptr [4 x i32], ptr %1, i64 0, i64 2
  %263 = load i32, ptr %262
  %264 = getelementptr [4 x i32], ptr %1, i64 0, i64 3
  %265 = load i32, ptr %264
  %266 = call i32 @printf(ptr @format2, i32 %259, i32 %261, i32 %263, i32 %265)
  ret i32 0
}
