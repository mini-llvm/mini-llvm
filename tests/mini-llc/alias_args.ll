@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define void @test(ptr %0, ptr %1) noinline {
2:
  store i32 -1, ptr %0
  %3 = load i32, ptr %1
  store i32 1, ptr %0
  %4 = load i32, ptr %1
  %5 = call i32 (ptr, ...) @printf(ptr @format, i32 %3)
  %6 = call i32 (ptr, ...) @printf(ptr @format, i32 %4)
  ret void
}

define i32 @main() {
0:
  %1 = alloca i32
  call void @test(ptr %1, ptr %1)
  ret i32 0
}
