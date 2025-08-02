@format = private constant [6 x i8] c"%llx\0A\00"

declare i32 @printf(ptr, ...)

define i32 @main() {
0:
  %1 = alloca double
  store double 0x40091eb860000000, ptr %1
  %2 = load i64, ptr %1
  %3 = call i32 @printf(ptr @format, i64 %2)
  ret i32 0
}
