define i32 @test() {
0:
  %1 = alloca i32
  store i32 0, ptr %1
  br label %4

2:
  %3 = load i32, ptr %1
  br label %4

4:
  %5 = load i32, ptr %1
  ret i32 %5
}

define i32 @main() {
0:
  ret i32 0
}
