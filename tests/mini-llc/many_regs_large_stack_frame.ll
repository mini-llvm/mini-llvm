@counter = global i32 0
@ptr = global ptr null
@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i32 @source() noinline {
0:
  %1 = load i32, ptr @counter
  %2 = add i32 %1, 1
  store i32 %2, ptr @counter
  ret i32 %1
}

define void @sink(i32 %0) noinline {
1:
  %2 = call i32 (ptr, ...) @printf(ptr @format, i32 %0)
  ret void
}

define void @test() noinline {
1:
  %2 = alloca [4096 x i8]
  store ptr %2, ptr @ptr
  %3 = call i32 @source()
  %4 = call i32 @source()
  %5 = call i32 @source()
  %6 = call i32 @source()
  %7 = call i32 @source()
  %8 = call i32 @source()
  %9 = call i32 @source()
  %10 = call i32 @source()
  %11 = call i32 @source()
  %12 = call i32 @source()
  %13 = call i32 @source()
  %14 = call i32 @source()
  %15 = call i32 @source()
  %16 = call i32 @source()
  %17 = call i32 @source()
  %18 = call i32 @source()
  %19 = call i32 @source()
  %20 = call i32 @source()
  %21 = call i32 @source()
  %22 = call i32 @source()
  %23 = call i32 @source()
  %24 = call i32 @source()
  %25 = call i32 @source()
  %26 = call i32 @source()
  %27 = call i32 @source()
  %28 = call i32 @source()
  %29 = call i32 @source()
  %30 = call i32 @source()
  %31 = call i32 @source()
  %32 = call i32 @source()
  %33 = call i32 @source()
  %34 = call i32 @source()
  %35 = call i32 @source()
  %36 = call i32 @source()
  %37 = call i32 @source()
  %38 = call i32 @source()
  %39 = call i32 @source()
  %40 = call i32 @source()
  %41 = call i32 @source()
  %42 = call i32 @source()
  %43 = call i32 @source()
  %44 = call i32 @source()
  %45 = call i32 @source()
  %46 = call i32 @source()
  %47 = call i32 @source()
  %48 = call i32 @source()
  %49 = call i32 @source()
  %50 = call i32 @source()
  %51 = call i32 @source()
  %52 = call i32 @source()
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
  call void @sink(i32 %52)
  ret void
}

define i32 @main() {
0:
  call void @test()
  ret i32 0
}
