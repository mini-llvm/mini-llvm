@ptr = global ptr null
@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define void @load() noinline {
0:
  %1 = load ptr, ptr @ptr
  %2 = load i32, ptr %1
  %3 = call i32 @printf(ptr @format, i32 %2)
  ret void
}

define i32 @main() {
0:
  %1 = alloca i32
  store ptr %1, ptr @ptr
  store i32 -1, ptr %1
  call void @load()
  store i32 1, ptr %1
  %2 = load i32, ptr %1
  %3 = call i32 @printf(ptr @format, i32 %2)
  ret i32 0
}
