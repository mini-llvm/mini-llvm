define void @test() {
0:
  %1 = alloca ptr
  store ptr %1, ptr %1
  %2 = load ptr, ptr %1
  %3 = load i32, ptr %2
  ret void
}

define i32 @main() {
0:
  ret i32 0
}
