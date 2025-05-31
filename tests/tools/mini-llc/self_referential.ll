define void @test() {
0:
  br label %1

1:
  %2 = phi i32 [ 0, %0 ], [ %2, %1 ]
  br label %1
}

define i32 @main() {
0:
  ret i32 0
}
