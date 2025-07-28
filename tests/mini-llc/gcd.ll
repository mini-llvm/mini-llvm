@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i32 @gcd(i32 %0, i32 %1) {
2:
  %3 = icmp eq i32 %1, 0
  br i1 %3, label %4, label %5

4:
  ret i32 %0

5:
  %6 = srem i32 %0, %1
  %7 = call i32 @gcd(i32 %1, i32 %6)
  ret i32 %7
}

define i32 @main() {
0:
  %1 = call i32 @gcd(i32 1, i32 0)
  %2 = call i32 (ptr, ...) @printf(ptr @format, i32 %1)
  %3 = call i32 @gcd(i32 30, i32 42)
  %4 = call i32 (ptr, ...) @printf(ptr @format, i32 %3)
  %5 = call i32 @gcd(i32 2136, i32 3456)
  %6 = call i32 (ptr, ...) @printf(ptr @format, i32 %5)
  ret i32 0
}
