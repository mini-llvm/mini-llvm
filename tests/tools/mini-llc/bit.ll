@format = private constant [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i32 @popcount1(i32 %0) {
1:
  %2 = alloca i32
  %3 = alloca i32
  store i32 %0, ptr %2
  store i32 0, ptr %3
  br label %4

4:
  %5 = load i32, ptr %2
  %6 = icmp ne i32 %5, 0
  br i1 %6, label %7, label %14

7:
  %8 = load i32, ptr %2
  %9 = and i32 %8, 1
  %10 = load i32, ptr %3
  %11 = add i32 %10, %9
  store i32 %11, ptr %3
  %12 = load i32, ptr %2
  %13 = lshr i32 %12, 1
  store i32 %13, ptr %2
  br label %4

14:
  %15 = load i32, ptr %3
  ret i32 %15
}

define i32 @popcount2(i32 %0) {
1:
  %2 = alloca i32
  %3 = alloca i32
  store i32 %0, ptr %2
  store i32 0, ptr %3
  br label %4

4:
  %5 = load i32, ptr %2
  %6 = icmp ne i32 %5, 0
  br i1 %6, label %7, label %14

7:
  %8 = load i32, ptr %3
  %9 = add i32 %8, 1
  store i32 %9, ptr %3
  %10 = load i32, ptr %2
  %11 = sub i32 %10, 1
  %12 = load i32, ptr %2
  %13 = and i32 %12, %11
  store i32 %13, ptr %2
  br label %4

14:
  %15 = load i32, ptr %3
  ret i32 %15
}

define i32 @clz1(i32 %0) {
1:
  %2 = alloca i32
  %3 = alloca i32
  %4 = alloca i32
  store i32 %0, ptr %3
  %5 = load i32, ptr %3
  %6 = icmp eq i32 %5, 0
  br i1 %6, label %7, label %8

7:
  store i32 32, ptr %2
  br label %21

8:
  store i32 0, ptr %4
  br label %9

9:
  %10 = load i32, ptr %3
  %11 = lshr i32 %10, 31
  %12 = and i32 %11, 1
  %13 = icmp eq i32 %12, 0
  br i1 %13, label %14, label %19

14:
  %15 = load i32, ptr %4
  %16 = add i32 %15, 1
  store i32 %16, ptr %4
  %17 = load i32, ptr %3
  %18 = shl i32 %17, 1
  store i32 %18, ptr %3
  br label %9

19:
  %20 = load i32, ptr %4
  store i32 %20, ptr %2
  br label %21

21:
  %22 = load i32, ptr %2
  ret i32 %22
}

define i32 @clz2(i32 %0) {
1:
  %2 = alloca i32
  %3 = alloca i32
  %4 = alloca i32
  store i32 %0, ptr %3
  %5 = load i32, ptr %3
  %6 = icmp eq i32 %5, 0
  br i1 %6, label %7, label %8

7:
  store i32 32, ptr %2
  br label %53

8:
  store i32 0, ptr %4
  %9 = load i32, ptr %3
  %10 = and i32 %9, -65536
  %11 = icmp eq i32 %10, 0
  br i1 %11, label %12, label %17

12:
  %13 = load i32, ptr %4
  %14 = add i32 %13, 16
  store i32 %14, ptr %4
  %15 = load i32, ptr %3
  %16 = shl i32 %15, 16
  store i32 %16, ptr %3
  br label %17

17:
  %18 = load i32, ptr %3
  %19 = and i32 %18, -16777216
  %20 = icmp eq i32 %19, 0
  br i1 %20, label %21, label %26

21:
  %22 = load i32, ptr %4
  %23 = add i32 %22, 8
  store i32 %23, ptr %4
  %24 = load i32, ptr %3
  %25 = shl i32 %24, 8
  store i32 %25, ptr %3
  br label %26

26:
  %27 = load i32, ptr %3
  %28 = and i32 %27, -268435456
  %29 = icmp eq i32 %28, 0
  br i1 %29, label %30, label %35

30:
  %31 = load i32, ptr %4
  %32 = add i32 %31, 4
  store i32 %32, ptr %4
  %33 = load i32, ptr %3
  %34 = shl i32 %33, 4
  store i32 %34, ptr %3
  br label %35

35:
  %36 = load i32, ptr %3
  %37 = and i32 %36, -1073741824
  %38 = icmp eq i32 %37, 0
  br i1 %38, label %39, label %44

39:
  %40 = load i32, ptr %4
  %41 = add i32 %40, 2
  store i32 %41, ptr %4
  %42 = load i32, ptr %3
  %43 = shl i32 %42, 2
  store i32 %43, ptr %3
  br label %44

44:
  %45 = load i32, ptr %3
  %46 = and i32 %45, -2147483648
  %47 = icmp eq i32 %46, 0
  br i1 %47, label %48, label %51

48:
  %49 = load i32, ptr %4
  %50 = add i32 %49, 1
  store i32 %50, ptr %4
  br label %51

51:
  %52 = load i32, ptr %4
  store i32 %52, ptr %2
  br label %53

53:
  %54 = load i32, ptr %2
  ret i32 %54
}

define i32 @ctz1(i32 %0) {
1:
  %2 = alloca i32
  %3 = alloca i32
  %4 = alloca i32
  store i32 %0, ptr %3
  %5 = load i32, ptr %3
  %6 = icmp eq i32 %5, 0
  br i1 %6, label %7, label %8

7:
  store i32 32, ptr %2
  br label %20

8:
  store i32 0, ptr %4
  br label %9

9:
  %10 = load i32, ptr %3
  %11 = and i32 %10, 1
  %12 = icmp eq i32 %11, 0
  br i1 %12, label %13, label %18

13:
  %14 = load i32, ptr %4
  %15 = add i32 %14, 1
  store i32 %15, ptr %4
  %16 = load i32, ptr %3
  %17 = lshr i32 %16, 1
  store i32 %17, ptr %3
  br label %9

18:
  %19 = load i32, ptr %4
  store i32 %19, ptr %2
  br label %20

20:
  %21 = load i32, ptr %2
  ret i32 %21
}

define i32 @ctz2(i32 %0) {
1:
  %2 = alloca i32
  %3 = alloca i32
  %4 = alloca i32
  store i32 %0, ptr %3
  %5 = load i32, ptr %3
  %6 = icmp eq i32 %5, 0
  br i1 %6, label %7, label %8

7:
  store i32 32, ptr %2
  br label %53

8:
  store i32 0, ptr %4
  %9 = load i32, ptr %3
  %10 = and i32 %9, 65535
  %11 = icmp eq i32 %10, 0
  br i1 %11, label %12, label %17

12:
  %13 = load i32, ptr %4
  %14 = add i32 %13, 16
  store i32 %14, ptr %4
  %15 = load i32, ptr %3
  %16 = lshr i32 %15, 16
  store i32 %16, ptr %3
  br label %17

17:
  %18 = load i32, ptr %3
  %19 = and i32 %18, 255
  %20 = icmp eq i32 %19, 0
  br i1 %20, label %21, label %26

21:
  %22 = load i32, ptr %4
  %23 = add i32 %22, 8
  store i32 %23, ptr %4
  %24 = load i32, ptr %3
  %25 = lshr i32 %24, 8
  store i32 %25, ptr %3
  br label %26

26:
  %27 = load i32, ptr %3
  %28 = and i32 %27, 15
  %29 = icmp eq i32 %28, 0
  br i1 %29, label %30, label %35

30:
  %31 = load i32, ptr %4
  %32 = add i32 %31, 4
  store i32 %32, ptr %4
  %33 = load i32, ptr %3
  %34 = lshr i32 %33, 4
  store i32 %34, ptr %3
  br label %35

35:
  %36 = load i32, ptr %3
  %37 = and i32 %36, 3
  %38 = icmp eq i32 %37, 0
  br i1 %38, label %39, label %44

39:
  %40 = load i32, ptr %4
  %41 = add i32 %40, 2
  store i32 %41, ptr %4
  %42 = load i32, ptr %3
  %43 = lshr i32 %42, 2
  store i32 %43, ptr %3
  br label %44

44:
  %45 = load i32, ptr %3
  %46 = and i32 %45, 1
  %47 = icmp eq i32 %46, 0
  br i1 %47, label %48, label %51

48:
  %49 = load i32, ptr %4
  %50 = add i32 %49, 1
  store i32 %50, ptr %4
  br label %51

51:
  %52 = load i32, ptr %4
  store i32 %52, ptr %2
  br label %53

53:
  %54 = load i32, ptr %2
  ret i32 %54
}

define i32 @main() {
0:
  %1 = call i32 @popcount1(i32 0)
  %2 = call i32 (ptr, ...) @printf(ptr @format, i32 %1)
  %3 = call i32 @popcount1(i32 -1)
  %4 = call i32 (ptr, ...) @printf(ptr @format, i32 %3)
  %5 = call i32 @popcount1(i32 1)
  %6 = call i32 (ptr, ...) @printf(ptr @format, i32 %5)
  %7 = call i32 @popcount1(i32 -2147483648)
  %8 = call i32 (ptr, ...) @printf(ptr @format, i32 %7)
  %9 = call i32 @popcount1(i32 305419896)
  %10 = call i32 (ptr, ...) @printf(ptr @format, i32 %9)
  %11 = call i32 @popcount1(i32 -1430532899)
  %12 = call i32 (ptr, ...) @printf(ptr @format, i32 %11)
  %13 = call i32 @popcount2(i32 0)
  %14 = call i32 (ptr, ...) @printf(ptr @format, i32 %13)
  %15 = call i32 @popcount2(i32 -1)
  %16 = call i32 (ptr, ...) @printf(ptr @format, i32 %15)
  %17 = call i32 @popcount2(i32 1)
  %18 = call i32 (ptr, ...) @printf(ptr @format, i32 %17)
  %19 = call i32 @popcount2(i32 -2147483648)
  %20 = call i32 (ptr, ...) @printf(ptr @format, i32 %19)
  %21 = call i32 @popcount2(i32 305419896)
  %22 = call i32 (ptr, ...) @printf(ptr @format, i32 %21)
  %23 = call i32 @popcount2(i32 -1430532899)
  %24 = call i32 (ptr, ...) @printf(ptr @format, i32 %23)
  %25 = call i32 @clz1(i32 0)
  %26 = call i32 (ptr, ...) @printf(ptr @format, i32 %25)
  %27 = call i32 @clz1(i32 -2147483648)
  %28 = call i32 (ptr, ...) @printf(ptr @format, i32 %27)
  %29 = call i32 @clz1(i32 1073741824)
  %30 = call i32 (ptr, ...) @printf(ptr @format, i32 %29)
  %31 = call i32 @clz1(i32 536870912)
  %32 = call i32 (ptr, ...) @printf(ptr @format, i32 %31)
  %33 = call i32 @clz1(i32 268435456)
  %34 = call i32 (ptr, ...) @printf(ptr @format, i32 %33)
  %35 = call i32 @clz1(i32 1)
  %36 = call i32 (ptr, ...) @printf(ptr @format, i32 %35)
  %37 = call i32 @clz1(i32 2)
  %38 = call i32 (ptr, ...) @printf(ptr @format, i32 %37)
  %39 = call i32 @clz1(i32 4)
  %40 = call i32 (ptr, ...) @printf(ptr @format, i32 %39)
  %41 = call i32 @clz1(i32 8)
  %42 = call i32 (ptr, ...) @printf(ptr @format, i32 %41)
  %43 = call i32 @clz1(i32 -1)
  %44 = call i32 (ptr, ...) @printf(ptr @format, i32 %43)
  %45 = call i32 @clz1(i32 65535)
  %46 = call i32 (ptr, ...) @printf(ptr @format, i32 %45)
  %47 = call i32 @clz1(i32 16843009)
  %48 = call i32 (ptr, ...) @printf(ptr @format, i32 %47)
  %49 = call i32 @clz1(i32 1114129)
  %50 = call i32 (ptr, ...) @printf(ptr @format, i32 %49)
  %51 = call i32 @clz2(i32 0)
  %52 = call i32 (ptr, ...) @printf(ptr @format, i32 %51)
  %53 = call i32 @clz2(i32 -2147483648)
  %54 = call i32 (ptr, ...) @printf(ptr @format, i32 %53)
  %55 = call i32 @clz2(i32 1073741824)
  %56 = call i32 (ptr, ...) @printf(ptr @format, i32 %55)
  %57 = call i32 @clz2(i32 536870912)
  %58 = call i32 (ptr, ...) @printf(ptr @format, i32 %57)
  %59 = call i32 @clz2(i32 268435456)
  %60 = call i32 (ptr, ...) @printf(ptr @format, i32 %59)
  %61 = call i32 @clz2(i32 1)
  %62 = call i32 (ptr, ...) @printf(ptr @format, i32 %61)
  %63 = call i32 @clz2(i32 2)
  %64 = call i32 (ptr, ...) @printf(ptr @format, i32 %63)
  %65 = call i32 @clz2(i32 4)
  %66 = call i32 (ptr, ...) @printf(ptr @format, i32 %65)
  %67 = call i32 @clz2(i32 8)
  %68 = call i32 (ptr, ...) @printf(ptr @format, i32 %67)
  %69 = call i32 @clz2(i32 -1)
  %70 = call i32 (ptr, ...) @printf(ptr @format, i32 %69)
  %71 = call i32 @clz2(i32 65535)
  %72 = call i32 (ptr, ...) @printf(ptr @format, i32 %71)
  %73 = call i32 @clz2(i32 16843009)
  %74 = call i32 (ptr, ...) @printf(ptr @format, i32 %73)
  %75 = call i32 @clz2(i32 1114129)
  %76 = call i32 (ptr, ...) @printf(ptr @format, i32 %75)
  %77 = call i32 @ctz1(i32 0)
  %78 = call i32 (ptr, ...) @printf(ptr @format, i32 %77)
  %79 = call i32 @ctz1(i32 1)
  %80 = call i32 (ptr, ...) @printf(ptr @format, i32 %79)
  %81 = call i32 @ctz1(i32 2)
  %82 = call i32 (ptr, ...) @printf(ptr @format, i32 %81)
  %83 = call i32 @ctz1(i32 4)
  %84 = call i32 (ptr, ...) @printf(ptr @format, i32 %83)
  %85 = call i32 @ctz1(i32 8)
  %86 = call i32 (ptr, ...) @printf(ptr @format, i32 %85)
  %87 = call i32 @ctz1(i32 -2147483648)
  %88 = call i32 (ptr, ...) @printf(ptr @format, i32 %87)
  %89 = call i32 @ctz1(i32 1073741824)
  %90 = call i32 (ptr, ...) @printf(ptr @format, i32 %89)
  %91 = call i32 @ctz1(i32 536870912)
  %92 = call i32 (ptr, ...) @printf(ptr @format, i32 %91)
  %93 = call i32 @ctz1(i32 268435456)
  %94 = call i32 (ptr, ...) @printf(ptr @format, i32 %93)
  %95 = call i32 @ctz1(i32 -1)
  %96 = call i32 (ptr, ...) @printf(ptr @format, i32 %95)
  %97 = call i32 @ctz1(i32 -65536)
  %98 = call i32 (ptr, ...) @printf(ptr @format, i32 %97)
  %99 = call i32 @ctz1(i32 269488144)
  %100 = call i32 (ptr, ...) @printf(ptr @format, i32 %99)
  %101 = call i32 @ctz1(i32 285217024)
  %102 = call i32 (ptr, ...) @printf(ptr @format, i32 %101)
  %103 = call i32 @ctz2(i32 0)
  %104 = call i32 (ptr, ...) @printf(ptr @format, i32 %103)
  %105 = call i32 @ctz2(i32 1)
  %106 = call i32 (ptr, ...) @printf(ptr @format, i32 %105)
  %107 = call i32 @ctz2(i32 2)
  %108 = call i32 (ptr, ...) @printf(ptr @format, i32 %107)
  %109 = call i32 @ctz2(i32 4)
  %110 = call i32 (ptr, ...) @printf(ptr @format, i32 %109)
  %111 = call i32 @ctz2(i32 8)
  %112 = call i32 (ptr, ...) @printf(ptr @format, i32 %111)
  %113 = call i32 @ctz2(i32 -2147483648)
  %114 = call i32 (ptr, ...) @printf(ptr @format, i32 %113)
  %115 = call i32 @ctz2(i32 1073741824)
  %116 = call i32 (ptr, ...) @printf(ptr @format, i32 %115)
  %117 = call i32 @ctz2(i32 536870912)
  %118 = call i32 (ptr, ...) @printf(ptr @format, i32 %117)
  %119 = call i32 @ctz2(i32 268435456)
  %120 = call i32 (ptr, ...) @printf(ptr @format, i32 %119)
  %121 = call i32 @ctz2(i32 -1)
  %122 = call i32 (ptr, ...) @printf(ptr @format, i32 %121)
  %123 = call i32 @ctz2(i32 -65536)
  %124 = call i32 (ptr, ...) @printf(ptr @format, i32 %123)
  %125 = call i32 @ctz2(i32 269488144)
  %126 = call i32 (ptr, ...) @printf(ptr @format, i32 %125)
  %127 = call i32 @ctz2(i32 285217024)
  %128 = call i32 (ptr, ...) @printf(ptr @format, i32 %127)
  ret i32 0
}
