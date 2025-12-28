@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i32 @test(i1 %0, i1 %1, i1 %2) {
3:
  br i1 %0, label %4, label %5

4:
  br i1 %1, label %5, label %6

5:
  br i1 %2, label %4, label %7

6:
  ret i32 1

7:
  ret i32 2
}

define i32 @main() {
0:
  %1 = call i32 @test(i1 true, i1 true, i1 false)
  %2 = call i32 @test(i1 true, i1 false, i1 false)
  %3 = call i32 @test(i1 false, i1 false, i1 true)
  %4 = call i32 @test(i1 false, i1 false, i1 false)
  %5 = call i32 (ptr, ...) @printf(ptr @format, i32 %1)
  %6 = call i32 (ptr, ...) @printf(ptr @format, i32 %2)
  %7 = call i32 (ptr, ...) @printf(ptr @format, i32 %3)
  %8 = call i32 (ptr, ...) @printf(ptr @format, i32 %4)
  ret i32 0
}
