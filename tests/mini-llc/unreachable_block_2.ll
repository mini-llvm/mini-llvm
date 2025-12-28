define i32 @test(i32 %0) {
1:
  br label %4

2:
  %3 = add i32 %0, 1
  br label %4

4:
  %5 = phi i32 [ 0, %1 ],  [ %3, %2 ]
  ret i32 %5
}

define i32 @main() {
0:
  ret i32 0
}
