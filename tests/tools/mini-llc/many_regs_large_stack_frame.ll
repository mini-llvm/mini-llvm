@__format1 = internal global [6 x i8] c"%lld\0A\00"
@__format2 = internal global [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i32 @source(i32 %0) noinline {
1:
  ret i32 %0
}

define void @sink1(ptr %0, ptr %1) noinline {
2:
  %3 = ptrtoint ptr %0 to i64
  %4 = ptrtoint ptr %1 to i64
  %5 = sub i64 %3, %4
  %6 = call i32 @printf(ptr @__format1, i64 %5)
  ret void
}

define void @sink2(i32 %0) noinline {
1:
  %2 = call i32 @printf(ptr @__format2, i32 %0)
  ret void
}

define void @example() noinline {
1:
  %2 = alloca [2048 x i8]
  call void @sink1(ptr %2, ptr %2)
  %3 = call i32 @source(i32 0)
  %4 = call i32 @source(i32 1)
  %5 = call i32 @source(i32 2)
  %6 = call i32 @source(i32 3)
  %7 = call i32 @source(i32 4)
  %8 = call i32 @source(i32 5)
  %9 = call i32 @source(i32 6)
  %10 = call i32 @source(i32 7)
  %11 = call i32 @source(i32 8)
  %12 = call i32 @source(i32 9)
  %13 = call i32 @source(i32 10)
  %14 = call i32 @source(i32 11)
  %15 = call i32 @source(i32 12)
  %16 = call i32 @source(i32 13)
  %17 = call i32 @source(i32 14)
  %18 = call i32 @source(i32 15)
  %19 = call i32 @source(i32 16)
  %20 = call i32 @source(i32 17)
  %21 = call i32 @source(i32 18)
  %22 = call i32 @source(i32 19)
  %23 = call i32 @source(i32 20)
  %24 = call i32 @source(i32 21)
  %25 = call i32 @source(i32 22)
  %26 = call i32 @source(i32 23)
  %27 = call i32 @source(i32 24)
  %28 = call i32 @source(i32 25)
  %29 = call i32 @source(i32 26)
  %30 = call i32 @source(i32 27)
  %31 = call i32 @source(i32 28)
  %32 = call i32 @source(i32 29)
  %33 = call i32 @source(i32 30)
  %34 = call i32 @source(i32 31)
  %35 = call i32 @source(i32 32)
  %36 = call i32 @source(i32 33)
  %37 = call i32 @source(i32 34)
  %38 = call i32 @source(i32 35)
  %39 = call i32 @source(i32 36)
  %40 = call i32 @source(i32 37)
  %41 = call i32 @source(i32 38)
  %42 = call i32 @source(i32 39)
  %43 = call i32 @source(i32 40)
  %44 = call i32 @source(i32 41)
  %45 = call i32 @source(i32 42)
  %46 = call i32 @source(i32 43)
  %47 = call i32 @source(i32 44)
  %48 = call i32 @source(i32 45)
  %49 = call i32 @source(i32 46)
  %50 = call i32 @source(i32 47)
  %51 = call i32 @source(i32 48)
  %52 = call i32 @source(i32 49)
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
  call void @example()
  ret i32 0
}
