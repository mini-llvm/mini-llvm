@ptr = global ptr null

define void @test() {
0:
  %1 = alloca i32
  store ptr %1, ptr @ptr
  call void @test()
  ret void
}

define i32 @main() {
0:
  ret i32 0
}
