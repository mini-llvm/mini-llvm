@ptr = global ptr null

define void @test1() {
0:
  %1 = alloca i32
  store ptr %1, ptr @ptr
  ret void
}

define ptr @test2() {
0:
  br label %1

1:
  call void @test1()
  br label %1
}

define i32 @main() {
0:
  ret i32 0
}
