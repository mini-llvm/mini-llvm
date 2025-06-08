@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i32 @mul_0(i32 %0) noinline {
1:
  %2 = mul i32 %0, 0
  ret i32 %2
}

define i32 @mul_1(i32 %0) noinline {
1:
  %2 = mul i32 %0, 1
  ret i32 %2
}

define i32 @mul_2(i32 %0) noinline {
1:
  %2 = mul i32 %0, 2
  ret i32 %2
}

define i32 @mul_3(i32 %0) noinline {
1:
  %2 = mul i32 %0, 3
  ret i32 %2
}

define i32 @mul_4(i32 %0) noinline {
1:
  %2 = mul i32 %0, 4
  ret i32 %2
}

define i32 @mul_100(i32 %0) noinline {
1:
  %2 = mul i32 %0, 100
  ret i32 %2
}

define i32 @mul_255(i32 %0) noinline {
1:
  %2 = mul i32 %0, 255
  ret i32 %2
}

define i32 @mul_256(i32 %0) noinline {
1:
  %2 = mul i32 %0, 256
  ret i32 %2
}

define i32 @mul_2147483647(i32 %0) noinline {
1:
  %2 = mul i32 %0, 2147483647
  ret i32 %2
}

define i32 @mul_n1(i32 %0) noinline {
1:
  %2 = mul i32 %0, -1
  ret i32 %2
}

define i32 @mul_n2(i32 %0) noinline {
1:
  %2 = mul i32 %0, -2
  ret i32 %2
}

define i32 @mul_n3(i32 %0) noinline {
1:
  %2 = mul i32 %0, -3
  ret i32 %2
}

define i32 @mul_n4(i32 %0) noinline {
1:
  %2 = mul i32 %0, -4
  ret i32 %2
}

define i32 @mul_n100(i32 %0) noinline {
1:
  %2 = mul i32 %0, -100
  ret i32 %2
}

define i32 @mul_n255(i32 %0) noinline {
1:
  %2 = mul i32 %0, -255
  ret i32 %2
}

define i32 @mul_n256(i32 %0) noinline {
1:
  %2 = mul i32 %0, -256
  ret i32 %2
}

define i32 @main() {
0:
  %1 = call i32 @mul_0(i32 12345)
  %2 = call i32 (ptr, ...) @printf(ptr @format, i32 %1)
  %3 = call i32 @mul_1(i32 12345)
  %4 = call i32 (ptr, ...) @printf(ptr @format, i32 %3)
  %5 = call i32 @mul_2(i32 12345)
  %6 = call i32 (ptr, ...) @printf(ptr @format, i32 %5)
  %7 = call i32 @mul_3(i32 12345)
  %8 = call i32 (ptr, ...) @printf(ptr @format, i32 %7)
  %9 = call i32 @mul_4(i32 12345)
  %10 = call i32 (ptr, ...) @printf(ptr @format, i32 %9)
  %11 = call i32 @mul_100(i32 12345)
  %12 = call i32 (ptr, ...) @printf(ptr @format, i32 %11)
  %13 = call i32 @mul_255(i32 12345)
  %14 = call i32 (ptr, ...) @printf(ptr @format, i32 %13)
  %15 = call i32 @mul_256(i32 12345)
  %16 = call i32 (ptr, ...) @printf(ptr @format, i32 %15)
  %17 = call i32 @mul_2147483647(i32 12345)
  %18 = call i32 (ptr, ...) @printf(ptr @format, i32 %17)
  %19 = call i32 @mul_n1(i32 12345)
  %20 = call i32 (ptr, ...) @printf(ptr @format, i32 %19)
  %21 = call i32 @mul_n2(i32 12345)
  %22 = call i32 (ptr, ...) @printf(ptr @format, i32 %21)
  %23 = call i32 @mul_n3(i32 12345)
  %24 = call i32 (ptr, ...) @printf(ptr @format, i32 %23)
  %25 = call i32 @mul_n4(i32 12345)
  %26 = call i32 (ptr, ...) @printf(ptr @format, i32 %25)
  %27 = call i32 @mul_n100(i32 12345)
  %28 = call i32 (ptr, ...) @printf(ptr @format, i32 %27)
  %29 = call i32 @mul_n255(i32 12345)
  %30 = call i32 (ptr, ...) @printf(ptr @format, i32 %29)
  %31 = call i32 @mul_n256(i32 12345)
  %32 = call i32 (ptr, ...) @printf(ptr @format, i32 %31)
  ret i32 0
}
