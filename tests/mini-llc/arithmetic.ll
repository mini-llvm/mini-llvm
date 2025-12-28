@format1 = private constant [4 x i8] c"%d\0A\00"
@format2 = private constant [6 x i8] c"%llx\0A\00"

declare i32 @printf(ptr, ...)

define i32 @add(i32 %0, i32 %1) {
2:
  %3 = add i32 %0, %1
  ret i32 %3
}

define i32 @sub(i32 %0, i32 %1) {
2:
  %3 = sub i32 %0, %1
  ret i32 %3
}

define i32 @mul(i32 %0, i32 %1) {
2:
  %3 = mul i32 %0, %1
  ret i32 %3
}

define i32 @sdiv(i32 %0, i32 %1) {
2:
  %3 = sdiv i32 %0, %1
  ret i32 %3
}

define i32 @srem(i32 %0, i32 %1) {
2:
  %3 = srem i32 %0, %1
  ret i32 %3
}

define i32 @udiv(i32 %0, i32 %1) {
2:
  %3 = udiv i32 %0, %1
  ret i32 %3
}

define i32 @urem(i32 %0, i32 %1) {
2:
  %3 = urem i32 %0, %1
  ret i32 %3
}

define i32 @and(i32 %0, i32 %1) {
2:
  %3 = and i32 %0, %1
  ret i32 %3
}

define i32 @or(i32 %0, i32 %1) {
2:
  %3 = or i32 %0, %1
  ret i32 %3
}

define i32 @xor(i32 %0, i32 %1) {
2:
  %3 = xor i32 %0, %1
  ret i32 %3
}

define i32 @shl(i32 %0, i32 %1) {
2:
  %3 = shl i32 %0, %1
  ret i32 %3
}

define i32 @lshr(i32 %0, i32 %1) {
2:
  %3 = lshr i32 %0, %1
  ret i32 %3
}

define i32 @ashr(i32 %0, i32 %1) {
2:
  %3 = ashr i32 %0, %1
  ret i32 %3
}

define double @fadd(double %0, double %1) {
2:
  %3 = fadd double %0, %1
  ret double %3
}

define double @fsub(double %0, double %1) {
2:
  %3 = fsub double %0, %1
  ret double %3
}

define double @fmul(double %0, double %1) {
2:
  %3 = fmul double %0, %1
  ret double %3
}

define double @fdiv(double %0, double %1) {
2:
  %3 = fdiv double %0, %1
  ret double %3
}

define double @fneg(double %0) {
1:
  %2 = fneg double %0
  ret double %2
}

define i32 @add_ni(i32 %0, i32 %1) noinline {
2:
  %3 = add i32 %0, %1
  ret i32 %3
}

define i32 @sub_ni(i32 %0, i32 %1) noinline {
2:
  %3 = sub i32 %0, %1
  ret i32 %3
}

define i32 @mul_ni(i32 %0, i32 %1) noinline {
2:
  %3 = mul i32 %0, %1
  ret i32 %3
}

define i32 @sdiv_ni(i32 %0, i32 %1) noinline {
2:
  %3 = sdiv i32 %0, %1
  ret i32 %3
}

define i32 @srem_ni(i32 %0, i32 %1) noinline {
2:
  %3 = srem i32 %0, %1
  ret i32 %3
}

define i32 @udiv_ni(i32 %0, i32 %1) noinline {
2:
  %3 = udiv i32 %0, %1
  ret i32 %3
}

define i32 @urem_ni(i32 %0, i32 %1) noinline {
2:
  %3 = urem i32 %0, %1
  ret i32 %3
}

define i32 @and_ni(i32 %0, i32 %1) noinline {
2:
  %3 = and i32 %0, %1
  ret i32 %3
}

define i32 @or_ni(i32 %0, i32 %1) noinline {
2:
  %3 = or i32 %0, %1
  ret i32 %3
}

define i32 @xor_ni(i32 %0, i32 %1) noinline {
2:
  %3 = xor i32 %0, %1
  ret i32 %3
}

define i32 @shl_ni(i32 %0, i32 %1) noinline {
2:
  %3 = shl i32 %0, %1
  ret i32 %3
}

define i32 @lshr_ni(i32 %0, i32 %1) noinline {
2:
  %3 = lshr i32 %0, %1
  ret i32 %3
}

define i32 @ashr_ni(i32 %0, i32 %1) noinline {
2:
  %3 = ashr i32 %0, %1
  ret i32 %3
}

define double @fadd_ni(double %0, double %1) noinline {
2:
  %3 = fadd double %0, %1
  ret double %3
}

define double @fsub_ni(double %0, double %1) noinline {
2:
  %3 = fsub double %0, %1
  ret double %3
}

define double @fmul_ni(double %0, double %1) noinline {
2:
  %3 = fmul double %0, %1
  ret double %3
}

define double @fdiv_ni(double %0, double %1) noinline {
2:
  %3 = fdiv double %0, %1
  ret double %3
}

define double @fneg_ni(double %0) noinline {
1:
  %2 = fneg double %0
  ret double %2
}

define i32 @main() {
0:
  %1 = call i32 @add(i32 42, i32 43)
  %2 = call i32 (ptr, ...) @printf(ptr @format1, i32 %1)
  %3 = call i32 @sub(i32 42, i32 43)
  %4 = call i32 (ptr, ...) @printf(ptr @format1, i32 %3)
  %5 = call i32 @mul(i32 42, i32 43)
  %6 = call i32 (ptr, ...) @printf(ptr @format1, i32 %5)
  %7 = call i32 @sdiv(i32 -42, i32 5)
  %8 = call i32 (ptr, ...) @printf(ptr @format1, i32 %7)
  %9 = call i32 @srem(i32 -42, i32 5)
  %10 = call i32 (ptr, ...) @printf(ptr @format1, i32 %9)
  %11 = call i32 @udiv(i32 -42, i32 5)
  %12 = call i32 (ptr, ...) @printf(ptr @format1, i32 %11)
  %13 = call i32 @urem(i32 -42, i32 5)
  %14 = call i32 (ptr, ...) @printf(ptr @format1, i32 %13)
  %15 = call i32 @and(i32 305419896, i32 2271560481)
  %16 = call i32 (ptr, ...) @printf(ptr @format1, i32 %15)
  %17 = call i32 @or(i32 305419896, i32 2271560481)
  %18 = call i32 (ptr, ...) @printf(ptr @format1, i32 %17)
  %19 = call i32 @xor(i32 305419896, i32 2271560481)
  %20 = call i32 (ptr, ...) @printf(ptr @format1, i32 %19)
  %21 = call i32 @shl(i32 -42, i32 2)
  %22 = call i32 (ptr, ...) @printf(ptr @format1, i32 %21)
  %23 = call i32 @lshr(i32 -42, i32 2)
  %24 = call i32 (ptr, ...) @printf(ptr @format1, i32 %23)
  %25 = call i32 @ashr(i32 -42, i32 2)
  %26 = call i32 (ptr, ...) @printf(ptr @format1, i32 %25)
  %27 = call double @fadd(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %28 = bitcast double %27 to i64
  %29 = call i32 (ptr, ...) @printf(ptr @format2, i64 %28)
  %30 = call double @fsub(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %31 = bitcast double %30 to i64
  %32 = call i32 (ptr, ...) @printf(ptr @format2, i64 %31)
  %33 = call double @fmul(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %34 = bitcast double %33 to i64
  %35 = call i32 (ptr, ...) @printf(ptr @format2, i64 %34)
  %36 = call double @fdiv(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %37 = bitcast double %36 to i64
  %38 = call i32 (ptr, ...) @printf(ptr @format2, i64 %37)
  %39 = call double @fneg(double 0x40091eb851eb851f)
  %40 = bitcast double %39 to i64
  %41 = call i32 (ptr, ...) @printf(ptr @format2, i64 %40)
  %42 = call i32 @add_ni(i32 42, i32 43)
  %43 = call i32 (ptr, ...) @printf(ptr @format1, i32 %42)
  %44 = call i32 @sub_ni(i32 42, i32 43)
  %45 = call i32 (ptr, ...) @printf(ptr @format1, i32 %44)
  %46 = call i32 @mul_ni(i32 42, i32 43)
  %47 = call i32 (ptr, ...) @printf(ptr @format1, i32 %46)
  %48 = call i32 @sdiv_ni(i32 -42, i32 5)
  %49 = call i32 (ptr, ...) @printf(ptr @format1, i32 %48)
  %50 = call i32 @srem_ni(i32 -42, i32 5)
  %51 = call i32 (ptr, ...) @printf(ptr @format1, i32 %50)
  %52 = call i32 @udiv_ni(i32 -42, i32 5)
  %53 = call i32 (ptr, ...) @printf(ptr @format1, i32 %52)
  %54 = call i32 @urem_ni(i32 -42, i32 5)
  %55 = call i32 (ptr, ...) @printf(ptr @format1, i32 %54)
  %56 = call i32 @and_ni(i32 305419896, i32 2271560481)
  %57 = call i32 (ptr, ...) @printf(ptr @format1, i32 %56)
  %58 = call i32 @or_ni(i32 305419896, i32 2271560481)
  %59 = call i32 (ptr, ...) @printf(ptr @format1, i32 %58)
  %60 = call i32 @xor_ni(i32 305419896, i32 2271560481)
  %61 = call i32 (ptr, ...) @printf(ptr @format1, i32 %60)
  %62 = call i32 @shl_ni(i32 -42, i32 2)
  %63 = call i32 (ptr, ...) @printf(ptr @format1, i32 %62)
  %64 = call i32 @lshr_ni(i32 -42, i32 2)
  %65 = call i32 (ptr, ...) @printf(ptr @format1, i32 %64)
  %66 = call i32 @ashr_ni(i32 -42, i32 2)
  %67 = call i32 (ptr, ...) @printf(ptr @format1, i32 %66)
  %68 = call double @fadd_ni(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %69 = bitcast double %68 to i64
  %70 = call i32 (ptr, ...) @printf(ptr @format2, i64 %69)
  %71 = call double @fsub_ni(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %72 = bitcast double %71 to i64
  %73 = call i32 (ptr, ...) @printf(ptr @format2, i64 %72)
  %74 = call double @fmul_ni(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %75 = bitcast double %74 to i64
  %76 = call i32 (ptr, ...) @printf(ptr @format2, i64 %75)
  %77 = call double @fdiv_ni(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %78 = bitcast double %77 to i64
  %79 = call i32 (ptr, ...) @printf(ptr @format2, i64 %78)
  %80 = call double @fneg_ni(double 0x40091eb851eb851f)
  %81 = bitcast double %80 to i64
  %82 = call i32 (ptr, ...) @printf(ptr @format2, i64 %81)
  ret i32 0
}
