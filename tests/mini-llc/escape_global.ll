@ptr = global ptr null
@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define void @load() noinline {
0:
  %1 = load ptr, ptr @ptr
  %2 = load i32, ptr %1
  %3 = call i32 (ptr, ...) @printf(ptr @format, i32 %2)
  ret void
}

define void @store(i32 %0) noinline {
1:
  %2 = load ptr, ptr @ptr
  store i32 %0, ptr %2
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
  %3 = call i32 (ptr, ...) @printf(ptr @format, i32 %2)
  %4 = alloca i32
  store ptr %4, ptr @ptr
  store i32 -1, ptr %4
  %5 = load i32, ptr %4
  call void @store(i32 1)
  %6 = load i32, ptr %4
  %7 = call i32 (ptr, ...) @printf(ptr @format, i32 %5)
  %8 = call i32 (ptr, ...) @printf(ptr @format, i32 %6)
  ret i32 0
}
