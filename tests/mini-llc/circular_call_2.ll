@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i32 @test1(i32 %0) {
1:
  %2 = icmp eq i32 %0, 0
  br i1 %2, label %3, label %4

3:
  ret i32 1

4:
  %5 = sub i32 %0, 1
  %6 = call i32 @test2(i32 %5)
  ret i32 %6
}

define i32 @test2(i32 %0) {
1:
  %2 = icmp eq i32 %0, 0
  br i1 %2, label %3, label %4

3:
  ret i32 2

4:
  %5 = sub i32 %0, 1
  %6 = call i32 @test3(i32 %5)
  ret i32 %6
}

define i32 @test3(i32 %0) {
1:
  %2 = icmp eq i32 %0, 0
  br i1 %2, label %3, label %4

3:
  ret i32 3

4:
  %5 = sub i32 %0, 1
  %6 = call i32 @test2(i32 %5)
  ret i32 %6
}

define i32 @main() {
0:
  %1 = call i32 @test1(i32 0)
  %2 = call i32 @test1(i32 1)
  %3 = call i32 @test1(i32 2)
  %4 = call i32 @test1(i32 3)
  %5 = call i32 @test1(i32 4)
  %6 = call i32 @test1(i32 5)
  %7 = call i32 (ptr, ...) @printf(ptr @format, i32 %1)
  %8 = call i32 (ptr, ...) @printf(ptr @format, i32 %2)
  %9 = call i32 (ptr, ...) @printf(ptr @format, i32 %3)
  %10 = call i32 (ptr, ...) @printf(ptr @format, i32 %4)
  %11 = call i32 (ptr, ...) @printf(ptr @format, i32 %5)
  %12 = call i32 (ptr, ...) @printf(ptr @format, i32 %6)
  ret i32 0
}
