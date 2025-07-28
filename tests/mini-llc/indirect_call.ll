@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i32 @test1(i32 %0) noinline {
1:
  %2 = mul i32 %0, 2
  ret i32 %2
}

define i32 @test2(i32 %0) noinline {
1:
  %2 = mul i32 %0, 3
  ret i32 %2
}

define ptr @test(i1 %0) noinline {
1:
  br i1 %0, label %2, label %3

2:
  ret ptr @test1

3:
  ret ptr @test2
}

define i32 @main() {
0:
  %1 = call ptr @test(i1 true)
  %2 = call ptr @test(i1 false)
  %3 = call i32 %1(i32 1)
  %4 = call i32 %2(i32 1)
  %5 = call i32 (ptr, ...) @printf(ptr @format, i32 %3)
  %6 = call i32 (ptr, ...) @printf(ptr @format, i32 %4)
  ret i32 0
}
