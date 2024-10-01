@__format1 = internal global [4 x i8] c"%d\0A\00"
@__format2 = internal global [6 x i8] c"%lld\0A\00"
@__format3 = internal global [4 x i8] c"%f\0A\00"
@__format4 = internal global [4 x i8] c"%s\0A\00"

@__str = internal global [14 x i8] c"Hello, world!\00"

@var1 = global i8 42
@var2 = global i16 43
@var3 = global i32 44
@var4 = global i64 45
@var5 = global float 0x40091eb860000000
@var6 = global double 0x4005c28f5c28f5c3
@var7 = global ptr @__str

declare i32 @printf(ptr, ...)

define i32 @main() {
0:
  %1 = load i8, ptr @var1
  %2 = sext i8 %1 to i32
  %3 = call i32 @printf(ptr @__format1, i32 %2)
  %4 = load i16, ptr @var2
  %5 = sext i16 %4 to i32
  %6 = call i32 @printf(ptr @__format1, i32 %5)
  %7 = load i32, ptr @var3
  %8 = call i32 @printf(ptr @__format1, i32 %7)
  %9 = load i64, ptr @var4
  %10 = call i32 @printf(ptr @__format2, i64 %9)
  %11 = load float, ptr @var5
  %12 = fpext float %11 to double
  %13 = call i32 @printf(ptr @__format3, double %12)
  %14 = load double, ptr @var6
  %15 = call i32 @printf(ptr @__format3, double %14)
  %16 = load ptr, ptr @var7
  %17 = call i32 @printf(ptr @__format4, ptr %16)
  ret i32 0
}
