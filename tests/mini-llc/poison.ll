define i32 @sdiv1() {
0:
  %1 = sdiv i32 1, 0
  ret i32 %1
}

define i32 @sdiv2() {
0:
  %1 = sdiv i32 -2147483648, -1
  ret i32 %1
}

define i32 @srem() {
0:
  %1 = srem i32 1, 0
  ret i32 %1
}

define i32 @udiv() {
0:
  %1 = udiv i32 1, 0
  ret i32 %1
}

define i32 @urem() {
0:
  %1 = urem i32 1, 0
  ret i32 %1
}

define i32 @shl1() {
0:
  %1 = shl i32 1, -1
  ret i32 %1
}

define i32 @shl2() {
0:
  %1 = shl i32 1, 32
  ret i32 %1
}

define i32 @lshr1() {
0:
  %1 = lshr i32 1, -1
  ret i32 %1
}

define i32 @lshr2() {
0:
  %1 = lshr i32 1, 32
  ret i32 %1
}

define i32 @ashr1() {
0:
  %1 = ashr i32 1, -1
  ret i32 %1
}

define i32 @ashr2() {
0:
  %1 = ashr i32 1, 32
  ret i32 %1
}

define i32 @fptosi1() {
0:
  %1 = fptosi double 0xc1e0000000200000 to i32
  ret i32 %1
}

define i32 @fptosi2() {
0:
  %1 = fptosi double 0x41e0000000000000 to i32
  ret i32 %1
}

define i32 @fptoui1() {
0:
  %1 = fptoui double 0xbff0000000000000 to i32
  ret i32 %1
}

define i32 @fptoui2() {
0:
  %1 = fptoui double 0x41f0000000000000 to i32
  ret i32 %1
}

define i32 @main() {
0:
  ret i32 0
}
