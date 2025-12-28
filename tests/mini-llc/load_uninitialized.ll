define i32 @test() {
0:
  %1 = alloca i32
  %2 = load i32, ptr %1
  ret i32 %2
}

define i32 @main() {
0:
  ret i32 0
}
