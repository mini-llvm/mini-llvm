@__format = internal global [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i32 @select(i1 %0, i32 %1, i32 %2) noinline {
3:
  %4 = select i1 %0, i32 %1, i32 %2
  ret i32 %4
}

define i32 @main() {
0:
  %1 = call i32 @select(i1 true, i32 42, i32 43)
  %2 = call i32 @printf(ptr @__format, i32 %1)
  %3 = call i32 @select(i1 false, i32 42, i32 43)
  %4 = call i32 @printf(ptr @__format, i32 %3)
  ret i32 0
}
