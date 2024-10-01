@__format = internal global [4 x i8] c"%d\0A\00"

declare i32 @printf(ptr, ...)

define i1 @icmp_eq(i32 %0, i32 %1) {
2:
  %3 = icmp eq i32 %0, %1
  ret i1 %3
}

define i1 @icmp_ne(i32 %0, i32 %1) {
2:
  %3 = icmp ne i32 %0, %1
  ret i1 %3
}

define i1 @icmp_slt(i32 %0, i32 %1) {
2:
  %3 = icmp slt i32 %0, %1
  ret i1 %3
}

define i1 @icmp_sgt(i32 %0, i32 %1) {
2:
  %3 = icmp sgt i32 %0, %1
  ret i1 %3
}

define i1 @icmp_sle(i32 %0, i32 %1) {
2:
  %3 = icmp sle i32 %0, %1
  ret i1 %3
}

define i1 @icmp_sge(i32 %0, i32 %1) {
2:
  %3 = icmp sge i32 %0, %1
  ret i1 %3
}

define i1 @icmp_ult(i32 %0, i32 %1) {
2:
  %3 = icmp ult i32 %0, %1
  ret i1 %3
}

define i1 @icmp_ugt(i32 %0, i32 %1) {
2:
  %3 = icmp ugt i32 %0, %1
  ret i1 %3
}

define i1 @icmp_ule(i32 %0, i32 %1) {
2:
  %3 = icmp ule i32 %0, %1
  ret i1 %3
}

define i1 @icmp_uge(i32 %0, i32 %1) {
2:
  %3 = icmp uge i32 %0, %1
  ret i1 %3
}

define i1 @fcmp_oeq(double %0, double %1) {
2:
  %3 = fcmp oeq double %0, %1
  ret i1 %3
}

define i1 @fcmp_one(double %0, double %1) {
2:
  %3 = fcmp one double %0, %1
  ret i1 %3
}

define i1 @fcmp_olt(double %0, double %1) {
2:
  %3 = fcmp olt double %0, %1
  ret i1 %3
}

define i1 @fcmp_ogt(double %0, double %1) {
2:
  %3 = fcmp ogt double %0, %1
  ret i1 %3
}

define i1 @fcmp_ole(double %0, double %1) {
2:
  %3 = fcmp ole double %0, %1
  ret i1 %3
}

define i1 @fcmp_oge(double %0, double %1) {
2:
  %3 = fcmp oge double %0, %1
  ret i1 %3
}

define i32 @main() {
0:
  %1 = call i1 @icmp_eq(i32 42, i32 -43)
  %2 = zext i1 %1 to i32
  %3 = call i32 @printf(ptr @__format, i32 %2)
  %4 = call i1 @icmp_eq(i32 -43, i32 42)
  %5 = zext i1 %4 to i32
  %6 = call i32 @printf(ptr @__format, i32 %5)
  %7 = call i1 @icmp_eq(i32 42, i32 42)
  %8 = zext i1 %7 to i32
  %9 = call i32 @printf(ptr @__format, i32 %8)
  %10 = call i1 @icmp_ne(i32 42, i32 -43)
  %11 = zext i1 %10 to i32
  %12 = call i32 @printf(ptr @__format, i32 %11)
  %13 = call i1 @icmp_ne(i32 -43, i32 42)
  %14 = zext i1 %13 to i32
  %15 = call i32 @printf(ptr @__format, i32 %14)
  %16 = call i1 @icmp_ne(i32 42, i32 42)
  %17 = zext i1 %16 to i32
  %18 = call i32 @printf(ptr @__format, i32 %17)
  %19 = call i1 @icmp_slt(i32 42, i32 -43)
  %20 = zext i1 %19 to i32
  %21 = call i32 @printf(ptr @__format, i32 %20)
  %22 = call i1 @icmp_slt(i32 -43, i32 42)
  %23 = zext i1 %22 to i32
  %24 = call i32 @printf(ptr @__format, i32 %23)
  %25 = call i1 @icmp_slt(i32 42, i32 42)
  %26 = zext i1 %25 to i32
  %27 = call i32 @printf(ptr @__format, i32 %26)
  %28 = call i1 @icmp_sgt(i32 42, i32 -43)
  %29 = zext i1 %28 to i32
  %30 = call i32 @printf(ptr @__format, i32 %29)
  %31 = call i1 @icmp_sgt(i32 -43, i32 42)
  %32 = zext i1 %31 to i32
  %33 = call i32 @printf(ptr @__format, i32 %32)
  %34 = call i1 @icmp_sgt(i32 42, i32 42)
  %35 = zext i1 %34 to i32
  %36 = call i32 @printf(ptr @__format, i32 %35)
  %37 = call i1 @icmp_sle(i32 42, i32 -43)
  %38 = zext i1 %37 to i32
  %39 = call i32 @printf(ptr @__format, i32 %38)
  %40 = call i1 @icmp_sle(i32 -43, i32 42)
  %41 = zext i1 %40 to i32
  %42 = call i32 @printf(ptr @__format, i32 %41)
  %43 = call i1 @icmp_sle(i32 42, i32 42)
  %44 = zext i1 %43 to i32
  %45 = call i32 @printf(ptr @__format, i32 %44)
  %46 = call i1 @icmp_sge(i32 42, i32 -43)
  %47 = zext i1 %46 to i32
  %48 = call i32 @printf(ptr @__format, i32 %47)
  %49 = call i1 @icmp_sge(i32 -43, i32 42)
  %50 = zext i1 %49 to i32
  %51 = call i32 @printf(ptr @__format, i32 %50)
  %52 = call i1 @icmp_sge(i32 42, i32 42)
  %53 = zext i1 %52 to i32
  %54 = call i32 @printf(ptr @__format, i32 %53)
  %55 = call i1 @icmp_ult(i32 42, i32 -43)
  %56 = zext i1 %55 to i32
  %57 = call i32 @printf(ptr @__format, i32 %56)
  %58 = call i1 @icmp_ult(i32 -43, i32 42)
  %59 = zext i1 %58 to i32
  %60 = call i32 @printf(ptr @__format, i32 %59)
  %61 = call i1 @icmp_ult(i32 42, i32 42)
  %62 = zext i1 %61 to i32
  %63 = call i32 @printf(ptr @__format, i32 %62)
  %64 = call i1 @icmp_ugt(i32 42, i32 -43)
  %65 = zext i1 %64 to i32
  %66 = call i32 @printf(ptr @__format, i32 %65)
  %67 = call i1 @icmp_ugt(i32 -43, i32 42)
  %68 = zext i1 %67 to i32
  %69 = call i32 @printf(ptr @__format, i32 %68)
  %70 = call i1 @icmp_ugt(i32 42, i32 42)
  %71 = zext i1 %70 to i32
  %72 = call i32 @printf(ptr @__format, i32 %71)
  %73 = call i1 @icmp_ule(i32 42, i32 -43)
  %74 = zext i1 %73 to i32
  %75 = call i32 @printf(ptr @__format, i32 %74)
  %76 = call i1 @icmp_ule(i32 -43, i32 42)
  %77 = zext i1 %76 to i32
  %78 = call i32 @printf(ptr @__format, i32 %77)
  %79 = call i1 @icmp_ule(i32 42, i32 42)
  %80 = zext i1 %79 to i32
  %81 = call i32 @printf(ptr @__format, i32 %80)
  %82 = call i1 @icmp_uge(i32 42, i32 -43)
  %83 = zext i1 %82 to i32
  %84 = call i32 @printf(ptr @__format, i32 %83)
  %85 = call i1 @icmp_uge(i32 -43, i32 42)
  %86 = zext i1 %85 to i32
  %87 = call i32 @printf(ptr @__format, i32 %86)
  %88 = call i1 @icmp_uge(i32 42, i32 42)
  %89 = zext i1 %88 to i32
  %90 = call i32 @printf(ptr @__format, i32 %89)
  %91 = call i1 @fcmp_oeq(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %92 = zext i1 %91 to i32
  %93 = call i32 @printf(ptr @__format, i32 %92)
  %94 = call i1 @fcmp_oeq(double 0x4005c28f5c28f5c3, double 0x40091eb851eb851f)
  %95 = zext i1 %94 to i32
  %96 = call i32 @printf(ptr @__format, i32 %95)
  %97 = call i1 @fcmp_oeq(double 0x40091eb851eb851f, double 0x40091eb851eb851f)
  %98 = zext i1 %97 to i32
  %99 = call i32 @printf(ptr @__format, i32 %98)
  %100 = call i1 @fcmp_one(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %101 = zext i1 %100 to i32
  %102 = call i32 @printf(ptr @__format, i32 %101)
  %103 = call i1 @fcmp_one(double 0x4005c28f5c28f5c3, double 0x40091eb851eb851f)
  %104 = zext i1 %103 to i32
  %105 = call i32 @printf(ptr @__format, i32 %104)
  %106 = call i1 @fcmp_one(double 0x40091eb851eb851f, double 0x40091eb851eb851f)
  %107 = zext i1 %106 to i32
  %108 = call i32 @printf(ptr @__format, i32 %107)
  %109 = call i1 @fcmp_olt(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %110 = zext i1 %109 to i32
  %111 = call i32 @printf(ptr @__format, i32 %110)
  %112 = call i1 @fcmp_olt(double 0x4005c28f5c28f5c3, double 0x40091eb851eb851f)
  %113 = zext i1 %112 to i32
  %114 = call i32 @printf(ptr @__format, i32 %113)
  %115 = call i1 @fcmp_olt(double 0x40091eb851eb851f, double 0x40091eb851eb851f)
  %116 = zext i1 %115 to i32
  %117 = call i32 @printf(ptr @__format, i32 %116)
  %118 = call i1 @fcmp_ogt(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %119 = zext i1 %118 to i32
  %120 = call i32 @printf(ptr @__format, i32 %119)
  %121 = call i1 @fcmp_ogt(double 0x4005c28f5c28f5c3, double 0x40091eb851eb851f)
  %122 = zext i1 %121 to i32
  %123 = call i32 @printf(ptr @__format, i32 %122)
  %124 = call i1 @fcmp_ogt(double 0x40091eb851eb851f, double 0x40091eb851eb851f)
  %125 = zext i1 %124 to i32
  %126 = call i32 @printf(ptr @__format, i32 %125)
  %127 = call i1 @fcmp_ole(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %128 = zext i1 %127 to i32
  %129 = call i32 @printf(ptr @__format, i32 %128)
  %130 = call i1 @fcmp_ole(double 0x4005c28f5c28f5c3, double 0x40091eb851eb851f)
  %131 = zext i1 %130 to i32
  %132 = call i32 @printf(ptr @__format, i32 %131)
  %133 = call i1 @fcmp_ole(double 0x40091eb851eb851f, double 0x40091eb851eb851f)
  %134 = zext i1 %133 to i32
  %135 = call i32 @printf(ptr @__format, i32 %134)
  %136 = call i1 @fcmp_oge(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %137 = zext i1 %136 to i32
  %138 = call i32 @printf(ptr @__format, i32 %137)
  %139 = call i1 @fcmp_oge(double 0x4005c28f5c28f5c3, double 0x40091eb851eb851f)
  %140 = zext i1 %139 to i32
  %141 = call i32 @printf(ptr @__format, i32 %140)
  %142 = call i1 @fcmp_oge(double 0x40091eb851eb851f, double 0x40091eb851eb851f)
  %143 = zext i1 %142 to i32
  %144 = call i32 @printf(ptr @__format, i32 %143)
  ret i32 0
}
