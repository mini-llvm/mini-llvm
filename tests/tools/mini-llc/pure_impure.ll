@counter = global i32 0
@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i32 @pure() noinline {
0:
  ret i32 0
}

define i32 @impure() noinline {
0:
  %1 = load i32, ptr @counter
  %2 = add i32 %1, 1
  store i32 %2, ptr @counter
  ret i32 %2
}

define i32 @main() {
0:
  %1 = call i32 @pure()
  %2 = call i32 @printf(ptr @format, i32 %1)
  %3 = call i32 @pure()
  %4 = call i32 @printf(ptr @format, i32 %3)
  %5 = call i32 @pure()
  %6 = call i32 @printf(ptr @format, i32 %5)
  %7 = call i32 @impure()
  %8 = call i32 @printf(ptr @format, i32 %7)
  %9 = call i32 @impure()
  %10 = call i32 @printf(ptr @format, i32 %9)
  %11 = call i32 @impure()
  %12 = call i32 @printf(ptr @format, i32 %11)
  ret i32 0
}
