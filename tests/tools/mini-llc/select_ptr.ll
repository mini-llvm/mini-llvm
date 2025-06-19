@x = global i32 1
@y = global i32 2
@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define ptr @select_ptr(i1 %0, ptr %1, ptr %2) {
3:
  %4 = select i1 %0, ptr %1, ptr %2
  ret ptr %4
}

define ptr @select_ptr_ni(i1 %0, ptr %1, ptr %2) noinline {
3:
  %4 = select i1 %0, ptr %1, ptr %2
  ret ptr %4
}

define i32 @main() {
0:
  %1 = call ptr @select_ptr(i1 true, ptr @x, ptr @y)
  %2 = load i32, ptr %1
  %3 = call i32 (ptr, ...) @printf(ptr @format, i32 %2)
  %4 = call ptr @select_ptr(i1 false, ptr @x, ptr @y)
  %5 = load i32, ptr %4
  %6 = call i32 (ptr, ...) @printf(ptr @format, i32 %5)
  %7 = call ptr @select_ptr_ni(i1 true, ptr @x, ptr @y)
  %8 = load i32, ptr %7
  %9 = call i32 (ptr, ...) @printf(ptr @format, i32 %8)
  %10 = call ptr @select_ptr_ni(i1 false, ptr @x, ptr @y)
  %11 = load i32, ptr %10
  %12 = call i32 (ptr, ...) @printf(ptr @format, i32 %11)
  ret i32 0
}
