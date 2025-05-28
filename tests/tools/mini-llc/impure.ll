@counter = global i32 0
@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i32 @impure() noinline {
0:
  %1 = load i32, ptr @counter
  %2 = add i32 %1, 1
  store i32 %2, ptr @counter
  ret i32 %2
}

define i32 @main() {
0:
  %1 = call i32 @impure()
  %2 = call i32 @impure()
  %3 = call i32 @impure()
  %4 = call i32 @printf(ptr @format, i32 %1)
  %5 = call i32 @printf(ptr @format, i32 %2)
  %6 = call i32 @printf(ptr @format, i32 %3)
  ret i32 0
}
