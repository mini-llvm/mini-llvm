@format1 = private constant [4 x i8] c"%d\0A\00"
@format2 = private constant [7 x i8] c"%.17g\0A\00"

declare i32 @printf(ptr, ...)

define void @test(
  i32 %0,
  double %1,
  i32 %2,
  double %3,
  i32 %4,
  double %5,
  i32 %6,
  double %7,
  i32 %8,
  double %9,
  i32 %10,
  double %11,
  i32 %12,
  double %13,
  i32 %14,
  double %15,
  i32 %16,
  double %17,
  i32 %18,
  double %19,
  i32 %20,
  double %21,
  i32 %22,
  double %23
) noinline {
24:
  %25 = call i32 (ptr, ...) @printf(ptr @format1, i32 %0)
  %26 = call i32 (ptr, ...) @printf(ptr @format2, double %1)
  %27 = call i32 (ptr, ...) @printf(ptr @format1, i32 %2)
  %28 = call i32 (ptr, ...) @printf(ptr @format2, double %3)
  %29 = call i32 (ptr, ...) @printf(ptr @format1, i32 %4)
  %30 = call i32 (ptr, ...) @printf(ptr @format2, double %5)
  %31 = call i32 (ptr, ...) @printf(ptr @format1, i32 %6)
  %32 = call i32 (ptr, ...) @printf(ptr @format2, double %7)
  %33 = call i32 (ptr, ...) @printf(ptr @format1, i32 %8)
  %34 = call i32 (ptr, ...) @printf(ptr @format2, double %9)
  %35 = call i32 (ptr, ...) @printf(ptr @format1, i32 %10)
  %36 = call i32 (ptr, ...) @printf(ptr @format2, double %11)
  %37 = call i32 (ptr, ...) @printf(ptr @format1, i32 %12)
  %38 = call i32 (ptr, ...) @printf(ptr @format2, double %13)
  %39 = call i32 (ptr, ...) @printf(ptr @format1, i32 %14)
  %40 = call i32 (ptr, ...) @printf(ptr @format2, double %15)
  %41 = call i32 (ptr, ...) @printf(ptr @format1, i32 %16)
  %42 = call i32 (ptr, ...) @printf(ptr @format2, double %17)
  %43 = call i32 (ptr, ...) @printf(ptr @format1, i32 %18)
  %44 = call i32 (ptr, ...) @printf(ptr @format2, double %19)
  %45 = call i32 (ptr, ...) @printf(ptr @format1, i32 %20)
  %46 = call i32 (ptr, ...) @printf(ptr @format2, double %21)
  %47 = call i32 (ptr, ...) @printf(ptr @format1, i32 %22)
  %48 = call i32 (ptr, ...) @printf(ptr @format2, double %23)
  ret void
}

define i32 @main() {
0:
  call void @test(
    i32 0,
    double 0x3fe0000000000000,
    i32 1,
    double 0x3ff8000000000000,
    i32 2,
    double 0x4004000000000000,
    i32 3,
    double 0x400c000000000000,
    i32 4,
    double 0x4012000000000000,
    i32 5,
    double 0x4016000000000000,
    i32 6,
    double 0x401a000000000000,
    i32 7,
    double 0x401e000000000000,
    i32 8,
    double 0x4021000000000000,
    i32 9,
    double 0x4023000000000000,
    i32 10,
    double 0x4025000000000000,
    i32 11,
    double 0x4027000000000000
  )
  ret i32 0
}
