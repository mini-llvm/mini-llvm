@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define void @test(i64 %0, i64 %1) noinline {
2:
  %3 = alloca i32
  %4 = ptrtoint ptr %3 to i64
  %5 = add i64 %4, %0
  %6 = add i64 %4, %1
  %7 = inttoptr i64 %5 to ptr
  %8 = inttoptr i64 %6 to ptr
  store i32 -1, ptr %7
  %9 = load i32, ptr %8
  store i32 1, ptr %7
  %10 = load i32, ptr %8
  %11 = call i32 (ptr, ...) @printf(ptr @format, i32 %9)
  %12 = call i32 (ptr, ...) @printf(ptr @format, i32 %10)
  ret void
}

define i32 @main() {
0:
  call void @test(i64 0, i64 0)
  ret i32 0
}
