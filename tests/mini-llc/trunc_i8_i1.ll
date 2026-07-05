@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i1 @trunc_i8_i1(i8 %0) {
1:
  %2 = trunc i8 %0 to i1
  ret i1 %2
}

define i1 @trunc_i8_i1_ni(i8 %0) noinline {
1:
  %2 = trunc i8 %0 to i1
  ret i1 %2
}

define i32 @main() {
0:
  %1 = call i1 @trunc_i8_i1(i8 4)
  %2 = zext i1 %1 to i32
  %3 = call i32 (ptr, ...) @printf(ptr @format, i32 %2)
  %4 = call i1 @trunc_i8_i1(i8 5)
  %5 = zext i1 %4 to i32
  %6 = call i32 (ptr, ...) @printf(ptr @format, i32 %5)
  %7 = call i1 @trunc_i8_i1_ni(i8 4)
  %8 = zext i1 %7 to i32
  %9 = call i32 (ptr, ...) @printf(ptr @format, i32 %8)
  %10 = call i1 @trunc_i8_i1_ni(i8 5)
  %11 = zext i1 %10 to i32
  %12 = call i32 (ptr, ...) @printf(ptr @format, i32 %11)
  ret i32 0
}
