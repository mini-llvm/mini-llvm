@x = private constant [10 x double] [
  double 0x3ffe672f7f9ffd14,
  double 0x4006f0a4b41926ec,
  double 0x3fcf8c2266fcb900,
  double 0xc0134f440ccd3412,
  double 0xc002e9caa6e03984,
  double 0x3ff1574364aaaf20,
  double 0x400cdc9f35cfc850,
  double 0xbff36e4234d78324,
  double 0xc00a4fe38605d777,
  double 0xbfc5c1a186cef500
]

@y = private constant [10 x double] [
  double 0x401311029dfa37df,
  double 0x401ab46e298cc8f8,
  double 0x3ff9f779b05b3bf0,
  double 0xc0211bcee884279c,
  double 0xc00e40992b112964,
  double 0x400ae83792a95c40,
  double 0x40208619702ded7d,
  double 0xbff6f1434a16d55d,
  double 0xc0161983898156ca,
  double 0x3fe7a039f0884e0b
]

@format = private constant [13 x i8] c"%.17g %.17g\0A\00"

declare ptr @memcpy(ptr, ptr, i64) argmemonly
declare i32 @printf(ptr, ...)

define double @mean(ptr %0, i32 %1) {
2:
  %3 = alloca ptr
  %4 = alloca i32
  %5 = alloca double
  %6 = alloca i32
  store ptr %0, ptr %3
  store i32 %1, ptr %4
  store double 0x0000000000000000, ptr %5
  store i32 0, ptr %6
  br label %7

7:
  %8 = load i32, ptr %6
  %9 = load i32, ptr %4
  %10 = icmp slt i32 %8, %9
  br i1 %10, label %11, label %22

11:
  %12 = load ptr, ptr %3
  %13 = load i32, ptr %6
  %14 = sext i32 %13 to i64
  %15 = getelementptr double, ptr %12, i64 %14
  %16 = load double, ptr %15
  %17 = load double, ptr %5
  %18 = fadd double %17, %16
  store double %18, ptr %5
  br label %19

19:
  %20 = load i32, ptr %6
  %21 = add i32 %20, 1
  store i32 %21, ptr %6
  br label %7

22:
  %23 = load double, ptr %5
  %24 = load i32, ptr %4
  %25 = sitofp i32 %24 to double
  %26 = fdiv double %23, %25
  ret double %26
}

define void @linear_regression(ptr %0, ptr %1, i32 %2, ptr %3, ptr %4) {
5:
  %6 = alloca ptr
  %7 = alloca ptr
  %8 = alloca i32
  %9 = alloca ptr
  %10 = alloca ptr
  %11 = alloca double
  %12 = alloca double
  %13 = alloca double
  %14 = alloca double
  %15 = alloca i32
  store ptr %0, ptr %6
  store ptr %1, ptr %7
  store i32 %2, ptr %8
  store ptr %3, ptr %9
  store ptr %4, ptr %10
  %16 = load ptr, ptr %6
  %17 = load i32, ptr %8
  %18 = call double @mean(ptr %16, i32 %17)
  store double %18, ptr %11
  %19 = load ptr, ptr %7
  %20 = load i32, ptr %8
  %21 = call double @mean(ptr %19, i32 %20)
  store double %21, ptr %12
  store double 0x0000000000000000, ptr %13
  store double 0x0000000000000000, ptr %14
  store i32 0, ptr %15
  br label %22

22:
  %23 = load i32, ptr %15
  %24 = load i32, ptr %8
  %25 = icmp slt i32 %23, %24
  br i1 %25, label %26, label %64

26:
  %27 = load ptr, ptr %6
  %28 = load i32, ptr %15
  %29 = sext i32 %28 to i64
  %30 = getelementptr double, ptr %27, i64 %29
  %31 = load double, ptr %30
  %32 = load double, ptr %11
  %33 = fsub double %31, %32
  %34 = load ptr, ptr %7
  %35 = load i32, ptr %15
  %36 = sext i32 %35 to i64
  %37 = getelementptr double, ptr %34, i64 %36
  %38 = load double, ptr %37
  %39 = load double, ptr %12
  %40 = fsub double %38, %39
  %41 = fmul double %33, %40
  %42 = load double, ptr %13
  %43 = fadd double %42, %41
  store double %43, ptr %13
  %44 = load ptr, ptr %6
  %45 = load i32, ptr %15
  %46 = sext i32 %45 to i64
  %47 = getelementptr double, ptr %44, i64 %46
  %48 = load double, ptr %47
  %49 = load double, ptr %11
  %50 = fsub double %48, %49
  %51 = load ptr, ptr %6
  %52 = load i32, ptr %15
  %53 = sext i32 %52 to i64
  %54 = getelementptr double, ptr %51, i64 %53
  %55 = load double, ptr %54
  %56 = load double, ptr %11
  %57 = fsub double %55, %56
  %58 = fmul double %50, %57
  %59 = load double, ptr %14
  %60 = fadd double %59, %58
  store double %60, ptr %14
  br label %61

61:
  %62 = load i32, ptr %15
  %63 = add i32 %62, 1
  store i32 %63, ptr %15
  br label %22

64:
  %65 = load double, ptr %13
  %66 = load double, ptr %14
  %67 = fdiv double %65, %66
  %68 = load ptr, ptr %9
  store double %67, ptr %68
  %69 = load double, ptr %12
  %70 = load ptr, ptr %9
  %71 = load double, ptr %70
  %72 = load double, ptr %11
  %73 = fmul double %71, %72
  %74 = fsub double %69, %73
  %75 = load ptr, ptr %10
  store double %74, ptr %75
  ret void
}

define i32 @main() {
0:
  %1 = alloca [10 x double]
  %2 = alloca [10 x double]
  %3 = alloca double
  %4 = alloca double
  %5 = call ptr @memcpy(ptr %1, ptr @x, i64 80)
  %6 = call ptr @memcpy(ptr %2, ptr @y, i64 80)
  call void @linear_regression(ptr %1, ptr %2, i32 10, ptr %3, ptr %4)
  %7 = load double, ptr %3
  %8 = load double, ptr %4
  %9 = call i32 (ptr, ...) @printf(ptr @format, double %7, double %8)
  ret i32 0
}
