@format = private constant [6 x i8] c"%lld\0A\00"

declare i32 @printf(ptr, ...)

define i64 @udiv_3(i64 %0) noinline {
1:
  %2 = udiv i64 %0, 3
  ret i64 %2
}

define i64 @sdiv_3(i64 %0) noinline {
1:
  %2 = sdiv i64 %0, 3
  ret i64 %2
}

define i32 @main() {
0:
  %1 = call i64 @udiv_3(i64 12345)
  %2 = call i32 (ptr, ...) @printf(ptr @format, i64 %1)
  %3 = call i64 @sdiv_3(i64 12345)
  %4 = call i32 (ptr, ...) @printf(ptr @format, i64 %3)
  ret i32 0
}
