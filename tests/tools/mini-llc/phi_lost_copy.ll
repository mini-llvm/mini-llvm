@__format = internal global [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i32 @example(i1 %0, i1 %1) {
2:
  br i1 %0, label %3, label %4

3:
  br i1 %1, label %5, label %7

4:
  br i1 %1, label %5, label %7

5:
  %6 = phi i32 [ 42, %3 ], [ 43, %4 ]
  ret i32 %6

7:
  %8 = phi i32 [ 44, %3 ], [ 45, %4 ]
  ret i32 %8
}

define i32 @main() {
0:
  %1 = call i32 @example(i1 true, i1 true)
  %2 = call i32 @example(i1 true, i1 false)
  %3 = call i32 @example(i1 false, i1 true)
  %4 = call i32 @example(i1 false, i1 false)
  %5 = call i32 @printf(ptr @__format, i32 %1)
  %6 = call i32 @printf(ptr @__format, i32 %2)
  %7 = call i32 @printf(ptr @__format, i32 %3)
  %8 = call i32 @printf(ptr @__format, i32 %4)
  ret i32 0
}
