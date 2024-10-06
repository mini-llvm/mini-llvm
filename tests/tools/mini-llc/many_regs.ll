@__format = internal global [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define void @sink(i32 %0) noinline {
1:
  %2 = call i32 @printf(ptr @__format, i32 %0)
  ret void
}

define void @example(i32 %0) noinline {
1:
  %2 = add i32 %0, 0
  %3 = add i32 %0, 1
  %4 = add i32 %0, 2
  %5 = add i32 %0, 3
  %6 = add i32 %0, 4
  %7 = add i32 %0, 5
  %8 = add i32 %0, 6
  %9 = add i32 %0, 7
  %10 = add i32 %0, 8
  %11 = add i32 %0, 9
  %12 = add i32 %0, 10
  %13 = add i32 %0, 11
  %14 = add i32 %0, 12
  %15 = add i32 %0, 13
  %16 = add i32 %0, 14
  %17 = add i32 %0, 15
  %18 = add i32 %0, 16
  %19 = add i32 %0, 17
  %20 = add i32 %0, 18
  %21 = add i32 %0, 19
  %22 = add i32 %0, 20
  %23 = add i32 %0, 21
  %24 = add i32 %0, 22
  %25 = add i32 %0, 23
  %26 = add i32 %0, 24
  %27 = add i32 %0, 25
  %28 = add i32 %0, 26
  %29 = add i32 %0, 27
  %30 = add i32 %0, 28
  %31 = add i32 %0, 29
  %32 = add i32 %0, 30
  %33 = add i32 %0, 31
  %34 = add i32 %0, 32
  %35 = add i32 %0, 33
  %36 = add i32 %0, 34
  %37 = add i32 %0, 35
  %38 = add i32 %0, 36
  %39 = add i32 %0, 37
  %40 = add i32 %0, 38
  %41 = add i32 %0, 39
  %42 = add i32 %0, 40
  %43 = add i32 %0, 41
  %44 = add i32 %0, 42
  %45 = add i32 %0, 43
  %46 = add i32 %0, 44
  %47 = add i32 %0, 45
  %48 = add i32 %0, 46
  %49 = add i32 %0, 47
  %50 = add i32 %0, 48
  %51 = add i32 %0, 49
  call void @sink(i32 %2)
  call void @sink(i32 %3)
  call void @sink(i32 %4)
  call void @sink(i32 %5)
  call void @sink(i32 %6)
  call void @sink(i32 %7)
  call void @sink(i32 %8)
  call void @sink(i32 %9)
  call void @sink(i32 %10)
  call void @sink(i32 %11)
  call void @sink(i32 %12)
  call void @sink(i32 %13)
  call void @sink(i32 %14)
  call void @sink(i32 %15)
  call void @sink(i32 %16)
  call void @sink(i32 %17)
  call void @sink(i32 %18)
  call void @sink(i32 %19)
  call void @sink(i32 %20)
  call void @sink(i32 %21)
  call void @sink(i32 %22)
  call void @sink(i32 %23)
  call void @sink(i32 %24)
  call void @sink(i32 %25)
  call void @sink(i32 %26)
  call void @sink(i32 %27)
  call void @sink(i32 %28)
  call void @sink(i32 %29)
  call void @sink(i32 %30)
  call void @sink(i32 %31)
  call void @sink(i32 %32)
  call void @sink(i32 %33)
  call void @sink(i32 %34)
  call void @sink(i32 %35)
  call void @sink(i32 %36)
  call void @sink(i32 %37)
  call void @sink(i32 %38)
  call void @sink(i32 %39)
  call void @sink(i32 %40)
  call void @sink(i32 %41)
  call void @sink(i32 %42)
  call void @sink(i32 %43)
  call void @sink(i32 %44)
  call void @sink(i32 %45)
  call void @sink(i32 %46)
  call void @sink(i32 %47)
  call void @sink(i32 %48)
  call void @sink(i32 %49)
  call void @sink(i32 %50)
  call void @sink(i32 %51)
  ret void
}

define i32 @main() {
0:
  call void @example(i32 42)
  ret i32 0
}
