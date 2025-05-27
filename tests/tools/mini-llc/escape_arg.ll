@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define void @load(ptr %0) noinline {
1:
  %2 = load i32, ptr %0
  %3 = call i32 @printf(ptr @format, i32 %2)
  ret void
}

define i32 @main() {
0:
  %1 = alloca i32
  store i32 -1, ptr %1
  call void @load(ptr %1)
  store i32 1, ptr %1
  %2 = load i32, ptr %1
  %3 = call i32 @printf(ptr @format, i32 %2)
  ret i32 0
}
