@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i32 @main() {
0:
  br label %1

1:
  %2 = phi i32 [ 0, %0 ], [ %3, %1 ]
  %3 = add i32 %2, 1
  %4 = icmp slt i32 %3, 5
  br i1 %4, label %1, label %5

5:
  %6 = call i32 (ptr, ...) @printf(ptr @format, i32 %2)
  ret i32 0
}
