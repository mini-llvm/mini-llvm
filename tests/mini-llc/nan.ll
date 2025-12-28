define i64 @fptosi() {
0:
  %1 = fptosi double 0x7ff8000000000000 to i64
  ret i64 %1
}

define i64 @fptoui() {
0:
  %1 = fptoui double 0x7ff8000000000000 to i64
  ret i64 %1
}

define i32 @main() {
0:
  ret i32 0
}
