@__format = internal global [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define void @sink1(ptr %0) noinline {
1:
  ret void
}

define void @sink2(i32 %0) noinline {
1:
  %2 = call i32 @printf(ptr @__format, i32 %0)
  ret void
}

define void @example(i32 %0) noinline {
1:
  %2 = alloca [2048 x i8]
  call void @sink1(ptr %2)
  %3 = add i32 %0, 0
  %4 = add i32 %0, 1
  %5 = add i32 %0, 2
  %6 = add i32 %0, 3
  %7 = add i32 %0, 4
  %8 = add i32 %0, 5
  %9 = add i32 %0, 6
  %10 = add i32 %0, 7
  %11 = add i32 %0, 8
  %12 = add i32 %0, 9
  %13 = add i32 %0, 10
  %14 = add i32 %0, 11
  %15 = add i32 %0, 12
  %16 = add i32 %0, 13
  %17 = add i32 %0, 14
  %18 = add i32 %0, 15
  %19 = add i32 %0, 16
  %20 = add i32 %0, 17
  %21 = add i32 %0, 18
  %22 = add i32 %0, 19
  %23 = add i32 %0, 20
  %24 = add i32 %0, 21
  %25 = add i32 %0, 22
  %26 = add i32 %0, 23
  %27 = add i32 %0, 24
  %28 = add i32 %0, 25
  %29 = add i32 %0, 26
  %30 = add i32 %0, 27
  %31 = add i32 %0, 28
  %32 = add i32 %0, 29
  %33 = add i32 %0, 30
  %34 = add i32 %0, 31
  %35 = add i32 %0, 32
  %36 = add i32 %0, 33
  %37 = add i32 %0, 34
  %38 = add i32 %0, 35
  %39 = add i32 %0, 36
  %40 = add i32 %0, 37
  %41 = add i32 %0, 38
  %42 = add i32 %0, 39
  %43 = add i32 %0, 40
  %44 = add i32 %0, 41
  %45 = add i32 %0, 42
  %46 = add i32 %0, 43
  %47 = add i32 %0, 44
  %48 = add i32 %0, 45
  %49 = add i32 %0, 46
  %50 = add i32 %0, 47
  %51 = add i32 %0, 48
  %52 = add i32 %0, 49
  call void @sink2(i32 %3)
  call void @sink2(i32 %4)
  call void @sink2(i32 %5)
  call void @sink2(i32 %6)
  call void @sink2(i32 %7)
  call void @sink2(i32 %8)
  call void @sink2(i32 %9)
  call void @sink2(i32 %10)
  call void @sink2(i32 %11)
  call void @sink2(i32 %12)
  call void @sink2(i32 %13)
  call void @sink2(i32 %14)
  call void @sink2(i32 %15)
  call void @sink2(i32 %16)
  call void @sink2(i32 %17)
  call void @sink2(i32 %18)
  call void @sink2(i32 %19)
  call void @sink2(i32 %20)
  call void @sink2(i32 %21)
  call void @sink2(i32 %22)
  call void @sink2(i32 %23)
  call void @sink2(i32 %24)
  call void @sink2(i32 %25)
  call void @sink2(i32 %26)
  call void @sink2(i32 %27)
  call void @sink2(i32 %28)
  call void @sink2(i32 %29)
  call void @sink2(i32 %30)
  call void @sink2(i32 %31)
  call void @sink2(i32 %32)
  call void @sink2(i32 %33)
  call void @sink2(i32 %34)
  call void @sink2(i32 %35)
  call void @sink2(i32 %36)
  call void @sink2(i32 %37)
  call void @sink2(i32 %38)
  call void @sink2(i32 %39)
  call void @sink2(i32 %40)
  call void @sink2(i32 %41)
  call void @sink2(i32 %42)
  call void @sink2(i32 %43)
  call void @sink2(i32 %44)
  call void @sink2(i32 %45)
  call void @sink2(i32 %46)
  call void @sink2(i32 %47)
  call void @sink2(i32 %48)
  call void @sink2(i32 %49)
  call void @sink2(i32 %50)
  call void @sink2(i32 %51)
  call void @sink2(i32 %52)
  ret void
}

define i32 @main() {
0:
  call void @example(i32 42)
  ret i32 0
}
