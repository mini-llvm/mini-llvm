@test1 = global [3 x ptr] [ptr null, ptr @test1, ptr @test2]

define ptr @test2(i1 %0) {
1:
  br i1 %0, label %2, label %3

2:
  ret ptr @test1

3:
  ret ptr @test2
}

define i32 @main() {
0:
  ret i32 0
}
