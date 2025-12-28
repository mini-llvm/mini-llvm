@format1 = private constant [4 x i8] c"%x\0A\00"
@format2 = private constant [6 x i8] c"%llx\0A\00"

declare i32 @printf(ptr, ...)

define i32 @trunc(i64 %0) {
1:
  %2 = trunc i64 %0 to i32
  ret i32 %2
}

define i64 @sext(i32 %0) {
1:
  %2 = sext i32 %0 to i64
  ret i64 %2
}

define i64 @zext(i32 %0) {
1:
  %2 = zext i32 %0 to i64
  ret i64 %2
}

define float @fptrunc(double %0) {
1:
  %2 = fptrunc double %0 to float
  ret float %2
}

define double @fpext(float %0) {
1:
  %2 = fpext float %0 to double
  ret double %2
}

define double @sitofp(i32 %0) {
1:
  %2 = sitofp i32 %0 to double
  ret double %2
}

define double @uitofp(i32 %0) {
1:
  %2 = uitofp i32 %0 to double
  ret double %2
}

define i32 @fptosi(double %0) {
1:
  %2 = fptosi double %0 to i32
  ret i32 %2
}

define i32 @fptoui(double %0) {
1:
  %2 = fptoui double %0 to i32
  ret i32 %2
}

define i64 @bitcast(double %0) {
1:
  %2 = bitcast double %0 to i64
  ret i64 %2
}

define i32 @trunc_ni(i64 %0) noinline {
1:
  %2 = trunc i64 %0 to i32
  ret i32 %2
}

define i64 @sext_ni(i32 %0) noinline {
1:
  %2 = sext i32 %0 to i64
  ret i64 %2
}

define i64 @zext_ni(i32 %0) noinline {
1:
  %2 = zext i32 %0 to i64
  ret i64 %2
}

define float @fptrunc_ni(double %0) noinline {
1:
  %2 = fptrunc double %0 to float
  ret float %2
}

define double @fpext_ni(float %0) noinline {
1:
  %2 = fpext float %0 to double
  ret double %2
}

define double @sitofp_ni(i32 %0) noinline {
1:
  %2 = sitofp i32 %0 to double
  ret double %2
}

define double @uitofp_ni(i32 %0) noinline {
1:
  %2 = uitofp i32 %0 to double
  ret double %2
}

define i32 @fptosi_ni(double %0) noinline {
1:
  %2 = fptosi double %0 to i32
  ret i32 %2
}

define i32 @fptoui_ni(double %0) noinline {
1:
  %2 = fptoui double %0 to i32
  ret i32 %2
}

define i64 @bitcast_ni(double %0) noinline {
1:
  %2 = bitcast double %0 to i64
  ret i64 %2
}

define i32 @main() {
0:
  %1 = call i32 @trunc(i64 1311768467139281697)
  %2 = call i32 (ptr, ...) @printf(ptr @format1, i32 %1)
  %3 = call i64 @sext(i32 2271560481)
  %4 = call i32 (ptr, ...) @printf(ptr @format2, i64 %3)
  %5 = call i64 @zext(i32 2271560481)
  %6 = call i32 (ptr, ...) @printf(ptr @format2, i64 %5)
  %7 = call float @fptrunc(double 0x40091eb851eb851f)
  %8 = bitcast float %7 to i32
  %9 = call i32 (ptr, ...) @printf(ptr @format1, i32 %8)
  %10 = call double @fpext(float 0x40091eb860000000)
  %11 = bitcast double %10 to i64
  %12 = call i32 (ptr, ...) @printf(ptr @format2, i64 %11)
  %13 = call double @sitofp(i32 2271560481)
  %14 = bitcast double %13 to i64
  %15 = call i32 (ptr, ...) @printf(ptr @format2, i64 %14)
  %16 = call double @uitofp(i32 2271560481)
  %17 = bitcast double %16 to i64
  %18 = call i32 (ptr, ...) @printf(ptr @format2, i64 %17)
  %19 = call i32 @fptosi(double 0x40091eb851eb851f)
  %20 = call i32 (ptr, ...) @printf(ptr @format1, i32 %19)
  %21 = call i32 @fptoui(double 0x40091eb851eb851f)
  %22 = call i32 (ptr, ...) @printf(ptr @format1, i32 %21)
  %23 = call i64 @bitcast(double 0x40091eb851eb851f)
  %24 = call i32 (ptr, ...) @printf(ptr @format2, i64 %23)
  %25 = call i32 @trunc_ni(i64 1311768467139281697)
  %26 = call i32 (ptr, ...) @printf(ptr @format1, i32 %25)
  %27 = call i64 @sext_ni(i32 2271560481)
  %28 = call i32 (ptr, ...) @printf(ptr @format2, i64 %27)
  %29 = call i64 @zext_ni(i32 2271560481)
  %30 = call i32 (ptr, ...) @printf(ptr @format2, i64 %29)
  %31 = call float @fptrunc_ni(double 0x40091eb851eb851f)
  %32 = bitcast float %31 to i32
  %33 = call i32 (ptr, ...) @printf(ptr @format1, i32 %32)
  %34 = call double @fpext_ni(float 0x40091eb860000000)
  %35 = bitcast double %34 to i64
  %36 = call i32 (ptr, ...) @printf(ptr @format2, i64 %35)
  %37 = call double @sitofp_ni(i32 2271560481)
  %38 = bitcast double %37 to i64
  %39 = call i32 (ptr, ...) @printf(ptr @format2, i64 %38)
  %40 = call double @uitofp_ni(i32 2271560481)
  %41 = bitcast double %40 to i64
  %42 = call i32 (ptr, ...) @printf(ptr @format2, i64 %41)
  %43 = call i32 @fptosi_ni(double 0x40091eb851eb851f)
  %44 = call i32 (ptr, ...) @printf(ptr @format1, i32 %43)
  %45 = call i32 @fptoui_ni(double 0x40091eb851eb851f)
  %46 = call i32 (ptr, ...) @printf(ptr @format1, i32 %45)
  %47 = call i64 @bitcast_ni(double 0x40091eb851eb851f)
  %48 = call i32 (ptr, ...) @printf(ptr @format2, i64 %47)
  ret i32 0
}
