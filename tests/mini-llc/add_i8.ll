@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i8 @add_i8(i8 %0, i8 %1) noinline {
2:
  %3 = add i8 %0, %1
  ret i8 %3
}

define i32 @main() {
0:
  %1 = call i8 @add_i8(i8 100, i8 100)
  %2 = sext i8 %1 to i32
  %3 = call i32 (ptr, ...) @printf(ptr @format, i32 %2)
  ret i32 0
}
