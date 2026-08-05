@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i8 @udiv_i8(i8 %0, i8 %1) noinline {
2:
  %3 = udiv i8 %0, %1
  ret i8 %3
}

define i8 @urem_i8(i8 %0, i8 %1) noinline {
2:
  %3 = urem i8 %0, %1
  ret i8 %3
}

define i8 @lshr_i8(i8 %0, i8 %1) noinline {
2:
  %3 = lshr i8 %0, %1
  ret i8 %3
}

define i16 @udiv_i16(i16 %0, i16 %1) noinline {
2:
  %3 = udiv i16 %0, %1
  ret i16 %3
}

define i16 @urem_i16(i16 %0, i16 %1) noinline {
2:
  %3 = urem i16 %0, %1
  ret i16 %3
}

define i16 @lshr_i16(i16 %0, i16 %1) noinline {
2:
  %3 = lshr i16 %0, %1
  ret i16 %3
}

define i32 @main() {
0:
  %1 = call i8 @udiv_i8(i8 128, i8 4)
  %2 = zext i8 %1 to i32
  %3 = call i32 (ptr, ...) @printf(ptr @format, i32 %2)
  %4 = call i8 @urem_i8(i8 128, i8 7)
  %5 = zext i8 %4 to i32
  %6 = call i32 (ptr, ...) @printf(ptr @format, i32 %5)
  %7 = call i8 @lshr_i8(i8 128, i8 4)
  %8 = zext i8 %7 to i32
  %9 = call i32 (ptr, ...) @printf(ptr @format, i32 %8)
  %10 = call i16 @udiv_i16(i16 40000, i16 4)
  %11 = zext i16 %10 to i32
  %12 = call i32 (ptr, ...) @printf(ptr @format, i32 %11)
  %13 = call i16 @urem_i16(i16 40000, i16 11)
  %14 = zext i16 %13 to i32
  %15 = call i32 (ptr, ...) @printf(ptr @format, i32 %14)
  %16 = call i16 @lshr_i16(i16 40000, i16 4)
  %17 = zext i16 %16 to i32
  %18 = call i32 (ptr, ...) @printf(ptr @format, i32 %17)
  ret i32 0
}
