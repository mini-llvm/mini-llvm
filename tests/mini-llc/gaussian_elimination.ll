@A = private constant [12 x double] [
  double 0x4000000000000000,
  double 0x3ff0000000000000,
  double 0xbff0000000000000,
  double 0x4020000000000000,
  double 0xc008000000000000,
  double 0xbff0000000000000,
  double 0x4000000000000000,
  double 0xc026000000000000,
  double 0xc000000000000000,
  double 0x3ff0000000000000,
  double 0x4000000000000000,
  double 0xc008000000000000
]
@format = private constant [19 x i8] c"%.17g %.17g %.17g\0A\00"

declare double @fabs(double)
declare ptr @memcpy(ptr, ptr, i64) argmemonly
declare i32 @printf(ptr, ...)

define void @swap(ptr %0, ptr %1) {
2:
  %3 = alloca ptr
  %4 = alloca ptr
  %5 = alloca double
  store ptr %0, ptr %3
  store ptr %1, ptr %4
  %6 = load ptr, ptr %3
  %7 = load double, ptr %6
  store double %7, ptr %5
  %8 = load ptr, ptr %4
  %9 = load double, ptr %8
  %10 = load ptr, ptr %3
  store double %9, ptr %10
  %11 = load double, ptr %5
  %12 = load ptr, ptr %4
  store double %11, ptr %12
  ret void
}

define void @gaussian_elimination(ptr %0, i32 %1, i32 %2) {
3:
  %4 = alloca ptr
  %5 = alloca i32
  %6 = alloca i32
  %7 = alloca i32
  %8 = alloca i32
  %9 = alloca i32
  %10 = alloca i32
  %11 = alloca i32
  %12 = alloca double
  %13 = alloca i32
  %14 = alloca i32
  %15 = alloca i32
  store ptr %0, ptr %4
  store i32 %1, ptr %5
  store i32 %2, ptr %6
  store i32 0, ptr %7
  store i32 0, ptr %8
  br label %16

16:
  %17 = load i32, ptr %7
  %18 = load i32, ptr %5
  %19 = icmp slt i32 %17, %18
  br i1 %19, label %20, label %24

20:
  %21 = load i32, ptr %8
  %22 = load i32, ptr %6
  %23 = icmp slt i32 %21, %22
  br label %24

24:
  %25 = phi i1 [ false, %16 ], [ %23, %20 ]
  br i1 %25, label %26, label %193

26:
  %27 = load i32, ptr %7
  store i32 %27, ptr %9
  %28 = load i32, ptr %7
  %29 = add i32 %28, 1
  store i32 %29, ptr %10
  br label %30

30:
  %31 = load i32, ptr %10
  %32 = load i32, ptr %5
  %33 = icmp slt i32 %31, %32
  br i1 %33, label %34, label %62

34:
  %35 = load ptr, ptr %4
  %36 = load i32, ptr %10
  %37 = load i32, ptr %6
  %38 = mul i32 %36, %37
  %39 = load i32, ptr %8
  %40 = add i32 %38, %39
  %41 = sext i32 %40 to i64
  %42 = getelementptr double, ptr %35, i64 %41
  %43 = load double, ptr %42
  %44 = call double @fabs(double %43)
  %45 = load ptr, ptr %4
  %46 = load i32, ptr %9
  %47 = load i32, ptr %6
  %48 = mul i32 %46, %47
  %49 = load i32, ptr %8
  %50 = add i32 %48, %49
  %51 = sext i32 %50 to i64
  %52 = getelementptr double, ptr %45, i64 %51
  %53 = load double, ptr %52
  %54 = call double @fabs(double %53)
  %55 = fcmp ogt double %44, %54
  br i1 %55, label %56, label %58

56:
  %57 = load i32, ptr %10
  store i32 %57, ptr %9
  br label %58

58:
  br label %59

59:
  %60 = load i32, ptr %10
  %61 = add i32 %60, 1
  store i32 %61, ptr %10
  br label %30

62:
  %63 = load i32, ptr %8
  store i32 %63, ptr %11
  br label %64

64:
  %65 = load i32, ptr %11
  %66 = load i32, ptr %6
  %67 = icmp slt i32 %65, %66
  br i1 %67, label %68, label %88

68:
  %69 = load ptr, ptr %4
  %70 = load i32, ptr %7
  %71 = load i32, ptr %6
  %72 = mul i32 %70, %71
  %73 = load i32, ptr %11
  %74 = add i32 %72, %73
  %75 = sext i32 %74 to i64
  %76 = getelementptr double, ptr %69, i64 %75
  %77 = load ptr, ptr %4
  %78 = load i32, ptr %9
  %79 = load i32, ptr %6
  %80 = mul i32 %78, %79
  %81 = load i32, ptr %11
  %82 = add i32 %80, %81
  %83 = sext i32 %82 to i64
  %84 = getelementptr double, ptr %77, i64 %83
  call void @swap(ptr %76, ptr %84)
  br label %85

85:
  %86 = load i32, ptr %11
  %87 = add i32 %86, 1
  store i32 %87, ptr %11
  br label %64

88:
  %89 = load ptr, ptr %4
  %90 = load i32, ptr %7
  %91 = load i32, ptr %6
  %92 = mul i32 %90, %91
  %93 = load i32, ptr %8
  %94 = add i32 %92, %93
  %95 = sext i32 %94 to i64
  %96 = getelementptr double, ptr %89, i64 %95
  %97 = load double, ptr %96
  %98 = call double @fabs(double %97)
  %99 = fcmp ogt double %98, 0x3e45798ee2308c3a
  br i1 %99, label %100, label %189

100:
  %101 = load ptr, ptr %4
  %102 = load i32, ptr %7
  %103 = load i32, ptr %6
  %104 = mul i32 %102, %103
  %105 = load i32, ptr %8
  %106 = add i32 %104, %105
  %107 = sext i32 %106 to i64
  %108 = getelementptr double, ptr %101, i64 %107
  %109 = load double, ptr %108
  store double %109, ptr %12
  %110 = load i32, ptr %8
  store i32 %110, ptr %13
  br label %111

111:
  %112 = load i32, ptr %13
  %113 = load i32, ptr %6
  %114 = icmp slt i32 %112, %113
  br i1 %114, label %115, label %130

115:
  %116 = load double, ptr %12
  %117 = load ptr, ptr %4
  %118 = load i32, ptr %7
  %119 = load i32, ptr %6
  %120 = mul i32 %118, %119
  %121 = load i32, ptr %13
  %122 = add i32 %120, %121
  %123 = sext i32 %122 to i64
  %124 = getelementptr double, ptr %117, i64 %123
  %125 = load double, ptr %124
  %126 = fdiv double %125, %116
  store double %126, ptr %124
  br label %127

127:
  %128 = load i32, ptr %13
  %129 = add i32 %128, 1
  store i32 %129, ptr %13
  br label %111

130:
  store i32 0, ptr %14
  br label %131

131:
  %132 = load i32, ptr %14
  %133 = load i32, ptr %5
  %134 = icmp slt i32 %132, %133
  br i1 %134, label %135, label %184

135:
  %136 = load i32, ptr %14
  %137 = load i32, ptr %7
  %138 = icmp ne i32 %136, %137
  br i1 %138, label %139, label %180

139:
  %140 = load ptr, ptr %4
  %141 = load i32, ptr %14
  %142 = load i32, ptr %6
  %143 = mul i32 %141, %142
  %144 = load i32, ptr %8
  %145 = add i32 %143, %144
  %146 = sext i32 %145 to i64
  %147 = getelementptr double, ptr %140, i64 %146
  %148 = load double, ptr %147
  store double %148, ptr %12
  %149 = load i32, ptr %8
  store i32 %149, ptr %15
  br label %150

150:
  %151 = load i32, ptr %15
  %152 = load i32, ptr %6
  %153 = icmp slt i32 %151, %152
  br i1 %153, label %154, label %179

154:
  %155 = load ptr, ptr %4
  %156 = load i32, ptr %7
  %157 = load i32, ptr %6
  %158 = mul i32 %156, %157
  %159 = load i32, ptr %15
  %160 = add i32 %158, %159
  %161 = sext i32 %160 to i64
  %162 = getelementptr double, ptr %155, i64 %161
  %163 = load double, ptr %162
  %164 = load double, ptr %12
  %165 = fmul double %163, %164
  %166 = load ptr, ptr %4
  %167 = load i32, ptr %14
  %168 = load i32, ptr %6
  %169 = mul i32 %167, %168
  %170 = load i32, ptr %15
  %171 = add i32 %169, %170
  %172 = sext i32 %171 to i64
  %173 = getelementptr double, ptr %166, i64 %172
  %174 = load double, ptr %173
  %175 = fsub double %174, %165
  store double %175, ptr %173
  br label %176

176:
  %177 = load i32, ptr %15
  %178 = add i32 %177, 1
  store i32 %178, ptr %15
  br label %150

179:
  br label %180

180:
  br label %181

181:
  %182 = load i32, ptr %14
  %183 = add i32 %182, 1
  store i32 %183, ptr %14
  br label %131

184:
  %185 = load i32, ptr %7
  %186 = add i32 %185, 1
  store i32 %186, ptr %7
  %187 = load i32, ptr %8
  %188 = add i32 %187, 1
  store i32 %188, ptr %8
  br label %192

189:
  %190 = load i32, ptr %8
  %191 = add i32 %190, 1
  store i32 %191, ptr %8
  br label %192

192:
  br label %16

193:
  ret void
}

define i32 @main() {
0:
  %1 = alloca [12 x double]
  %2 = call ptr @memcpy(ptr %1, ptr @A, i64 96)
  call void @gaussian_elimination(ptr %1, i32 3, i32 4)
  %3 = getelementptr [12 x double], ptr %1, i64 0, i64 3
  %4 = load double, ptr %3
  %5 = getelementptr [12 x double], ptr %1, i64 0, i64 7
  %6 = load double, ptr %5
  %7 = getelementptr [12 x double], ptr %1, i64 0, i64 11
  %8 = load double, ptr %7
  %9 = call i32 (ptr, ...) @printf(ptr @format, double %4, double %6, double %8)
  ret i32 0
}
