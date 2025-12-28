@format = private constant [4 x i8] c"%d\0A\00"

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

define i1 @icmp_eq_ni(i32 %0, i32 %1) noinline {
2:
  %3 = icmp eq i32 %0, %1
  ret i1 %3
}

define i1 @icmp_ne_ni(i32 %0, i32 %1) noinline {
2:
  %3 = icmp ne i32 %0, %1
  ret i1 %3
}

define i1 @icmp_slt_ni(i32 %0, i32 %1) noinline {
2:
  %3 = icmp slt i32 %0, %1
  ret i1 %3
}

define i1 @icmp_sgt_ni(i32 %0, i32 %1) noinline {
2:
  %3 = icmp sgt i32 %0, %1
  ret i1 %3
}

define i1 @icmp_sle_ni(i32 %0, i32 %1) noinline {
2:
  %3 = icmp sle i32 %0, %1
  ret i1 %3
}

define i1 @icmp_sge_ni(i32 %0, i32 %1) noinline {
2:
  %3 = icmp sge i32 %0, %1
  ret i1 %3
}

define i1 @icmp_ult_ni(i32 %0, i32 %1) noinline {
2:
  %3 = icmp ult i32 %0, %1
  ret i1 %3
}

define i1 @icmp_ugt_ni(i32 %0, i32 %1) noinline {
2:
  %3 = icmp ugt i32 %0, %1
  ret i1 %3
}

define i1 @icmp_ule_ni(i32 %0, i32 %1) noinline {
2:
  %3 = icmp ule i32 %0, %1
  ret i1 %3
}

define i1 @icmp_uge_ni(i32 %0, i32 %1) noinline {
2:
  %3 = icmp uge i32 %0, %1
  ret i1 %3
}

define i1 @fcmp_oeq_ni(double %0, double %1) noinline {
2:
  %3 = fcmp oeq double %0, %1
  ret i1 %3
}

define i1 @fcmp_one_ni(double %0, double %1) noinline {
2:
  %3 = fcmp one double %0, %1
  ret i1 %3
}

define i1 @fcmp_olt_ni(double %0, double %1) noinline {
2:
  %3 = fcmp olt double %0, %1
  ret i1 %3
}

define i1 @fcmp_ogt_ni(double %0, double %1) noinline {
2:
  %3 = fcmp ogt double %0, %1
  ret i1 %3
}

define i1 @fcmp_ole_ni(double %0, double %1) noinline {
2:
  %3 = fcmp ole double %0, %1
  ret i1 %3
}

define i1 @fcmp_oge_ni(double %0, double %1) noinline {
2:
  %3 = fcmp oge double %0, %1
  ret i1 %3
}

define i32 @main() {
0:
  %1 = call i1 @icmp_eq(i32 42, i32 -43)
  %2 = zext i1 %1 to i32
  %3 = call i32 (ptr, ...) @printf(ptr @format, i32 %2)
  %4 = call i1 @icmp_eq(i32 -43, i32 42)
  %5 = zext i1 %4 to i32
  %6 = call i32 (ptr, ...) @printf(ptr @format, i32 %5)
  %7 = call i1 @icmp_eq(i32 42, i32 42)
  %8 = zext i1 %7 to i32
  %9 = call i32 (ptr, ...) @printf(ptr @format, i32 %8)
  %10 = call i1 @icmp_ne(i32 42, i32 -43)
  %11 = zext i1 %10 to i32
  %12 = call i32 (ptr, ...) @printf(ptr @format, i32 %11)
  %13 = call i1 @icmp_ne(i32 -43, i32 42)
  %14 = zext i1 %13 to i32
  %15 = call i32 (ptr, ...) @printf(ptr @format, i32 %14)
  %16 = call i1 @icmp_ne(i32 42, i32 42)
  %17 = zext i1 %16 to i32
  %18 = call i32 (ptr, ...) @printf(ptr @format, i32 %17)
  %19 = call i1 @icmp_slt(i32 42, i32 -43)
  %20 = zext i1 %19 to i32
  %21 = call i32 (ptr, ...) @printf(ptr @format, i32 %20)
  %22 = call i1 @icmp_slt(i32 -43, i32 42)
  %23 = zext i1 %22 to i32
  %24 = call i32 (ptr, ...) @printf(ptr @format, i32 %23)
  %25 = call i1 @icmp_slt(i32 42, i32 42)
  %26 = zext i1 %25 to i32
  %27 = call i32 (ptr, ...) @printf(ptr @format, i32 %26)
  %28 = call i1 @icmp_sgt(i32 42, i32 -43)
  %29 = zext i1 %28 to i32
  %30 = call i32 (ptr, ...) @printf(ptr @format, i32 %29)
  %31 = call i1 @icmp_sgt(i32 -43, i32 42)
  %32 = zext i1 %31 to i32
  %33 = call i32 (ptr, ...) @printf(ptr @format, i32 %32)
  %34 = call i1 @icmp_sgt(i32 42, i32 42)
  %35 = zext i1 %34 to i32
  %36 = call i32 (ptr, ...) @printf(ptr @format, i32 %35)
  %37 = call i1 @icmp_sle(i32 42, i32 -43)
  %38 = zext i1 %37 to i32
  %39 = call i32 (ptr, ...) @printf(ptr @format, i32 %38)
  %40 = call i1 @icmp_sle(i32 -43, i32 42)
  %41 = zext i1 %40 to i32
  %42 = call i32 (ptr, ...) @printf(ptr @format, i32 %41)
  %43 = call i1 @icmp_sle(i32 42, i32 42)
  %44 = zext i1 %43 to i32
  %45 = call i32 (ptr, ...) @printf(ptr @format, i32 %44)
  %46 = call i1 @icmp_sge(i32 42, i32 -43)
  %47 = zext i1 %46 to i32
  %48 = call i32 (ptr, ...) @printf(ptr @format, i32 %47)
  %49 = call i1 @icmp_sge(i32 -43, i32 42)
  %50 = zext i1 %49 to i32
  %51 = call i32 (ptr, ...) @printf(ptr @format, i32 %50)
  %52 = call i1 @icmp_sge(i32 42, i32 42)
  %53 = zext i1 %52 to i32
  %54 = call i32 (ptr, ...) @printf(ptr @format, i32 %53)
  %55 = call i1 @icmp_ult(i32 42, i32 -43)
  %56 = zext i1 %55 to i32
  %57 = call i32 (ptr, ...) @printf(ptr @format, i32 %56)
  %58 = call i1 @icmp_ult(i32 -43, i32 42)
  %59 = zext i1 %58 to i32
  %60 = call i32 (ptr, ...) @printf(ptr @format, i32 %59)
  %61 = call i1 @icmp_ult(i32 42, i32 42)
  %62 = zext i1 %61 to i32
  %63 = call i32 (ptr, ...) @printf(ptr @format, i32 %62)
  %64 = call i1 @icmp_ugt(i32 42, i32 -43)
  %65 = zext i1 %64 to i32
  %66 = call i32 (ptr, ...) @printf(ptr @format, i32 %65)
  %67 = call i1 @icmp_ugt(i32 -43, i32 42)
  %68 = zext i1 %67 to i32
  %69 = call i32 (ptr, ...) @printf(ptr @format, i32 %68)
  %70 = call i1 @icmp_ugt(i32 42, i32 42)
  %71 = zext i1 %70 to i32
  %72 = call i32 (ptr, ...) @printf(ptr @format, i32 %71)
  %73 = call i1 @icmp_ule(i32 42, i32 -43)
  %74 = zext i1 %73 to i32
  %75 = call i32 (ptr, ...) @printf(ptr @format, i32 %74)
  %76 = call i1 @icmp_ule(i32 -43, i32 42)
  %77 = zext i1 %76 to i32
  %78 = call i32 (ptr, ...) @printf(ptr @format, i32 %77)
  %79 = call i1 @icmp_ule(i32 42, i32 42)
  %80 = zext i1 %79 to i32
  %81 = call i32 (ptr, ...) @printf(ptr @format, i32 %80)
  %82 = call i1 @icmp_uge(i32 42, i32 -43)
  %83 = zext i1 %82 to i32
  %84 = call i32 (ptr, ...) @printf(ptr @format, i32 %83)
  %85 = call i1 @icmp_uge(i32 -43, i32 42)
  %86 = zext i1 %85 to i32
  %87 = call i32 (ptr, ...) @printf(ptr @format, i32 %86)
  %88 = call i1 @icmp_uge(i32 42, i32 42)
  %89 = zext i1 %88 to i32
  %90 = call i32 (ptr, ...) @printf(ptr @format, i32 %89)
  %91 = call i1 @fcmp_oeq(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %92 = zext i1 %91 to i32
  %93 = call i32 (ptr, ...) @printf(ptr @format, i32 %92)
  %94 = call i1 @fcmp_oeq(double 0x4005c28f5c28f5c3, double 0x40091eb851eb851f)
  %95 = zext i1 %94 to i32
  %96 = call i32 (ptr, ...) @printf(ptr @format, i32 %95)
  %97 = call i1 @fcmp_oeq(double 0x40091eb851eb851f, double 0x40091eb851eb851f)
  %98 = zext i1 %97 to i32
  %99 = call i32 (ptr, ...) @printf(ptr @format, i32 %98)
  %100 = call i1 @fcmp_one(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %101 = zext i1 %100 to i32
  %102 = call i32 (ptr, ...) @printf(ptr @format, i32 %101)
  %103 = call i1 @fcmp_one(double 0x4005c28f5c28f5c3, double 0x40091eb851eb851f)
  %104 = zext i1 %103 to i32
  %105 = call i32 (ptr, ...) @printf(ptr @format, i32 %104)
  %106 = call i1 @fcmp_one(double 0x40091eb851eb851f, double 0x40091eb851eb851f)
  %107 = zext i1 %106 to i32
  %108 = call i32 (ptr, ...) @printf(ptr @format, i32 %107)
  %109 = call i1 @fcmp_olt(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %110 = zext i1 %109 to i32
  %111 = call i32 (ptr, ...) @printf(ptr @format, i32 %110)
  %112 = call i1 @fcmp_olt(double 0x4005c28f5c28f5c3, double 0x40091eb851eb851f)
  %113 = zext i1 %112 to i32
  %114 = call i32 (ptr, ...) @printf(ptr @format, i32 %113)
  %115 = call i1 @fcmp_olt(double 0x40091eb851eb851f, double 0x40091eb851eb851f)
  %116 = zext i1 %115 to i32
  %117 = call i32 (ptr, ...) @printf(ptr @format, i32 %116)
  %118 = call i1 @fcmp_ogt(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %119 = zext i1 %118 to i32
  %120 = call i32 (ptr, ...) @printf(ptr @format, i32 %119)
  %121 = call i1 @fcmp_ogt(double 0x4005c28f5c28f5c3, double 0x40091eb851eb851f)
  %122 = zext i1 %121 to i32
  %123 = call i32 (ptr, ...) @printf(ptr @format, i32 %122)
  %124 = call i1 @fcmp_ogt(double 0x40091eb851eb851f, double 0x40091eb851eb851f)
  %125 = zext i1 %124 to i32
  %126 = call i32 (ptr, ...) @printf(ptr @format, i32 %125)
  %127 = call i1 @fcmp_ole(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %128 = zext i1 %127 to i32
  %129 = call i32 (ptr, ...) @printf(ptr @format, i32 %128)
  %130 = call i1 @fcmp_ole(double 0x4005c28f5c28f5c3, double 0x40091eb851eb851f)
  %131 = zext i1 %130 to i32
  %132 = call i32 (ptr, ...) @printf(ptr @format, i32 %131)
  %133 = call i1 @fcmp_ole(double 0x40091eb851eb851f, double 0x40091eb851eb851f)
  %134 = zext i1 %133 to i32
  %135 = call i32 (ptr, ...) @printf(ptr @format, i32 %134)
  %136 = call i1 @fcmp_oge(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %137 = zext i1 %136 to i32
  %138 = call i32 (ptr, ...) @printf(ptr @format, i32 %137)
  %139 = call i1 @fcmp_oge(double 0x4005c28f5c28f5c3, double 0x40091eb851eb851f)
  %140 = zext i1 %139 to i32
  %141 = call i32 (ptr, ...) @printf(ptr @format, i32 %140)
  %142 = call i1 @fcmp_oge(double 0x40091eb851eb851f, double 0x40091eb851eb851f)
  %143 = zext i1 %142 to i32
  %144 = call i32 (ptr, ...) @printf(ptr @format, i32 %143)
  %145 = call i1 @icmp_eq_ni(i32 42, i32 -43)
  %146 = zext i1 %145 to i32
  %147 = call i32 (ptr, ...) @printf(ptr @format, i32 %146)
  %148 = call i1 @icmp_eq_ni(i32 -43, i32 42)
  %149 = zext i1 %148 to i32
  %150 = call i32 (ptr, ...) @printf(ptr @format, i32 %149)
  %151 = call i1 @icmp_eq_ni(i32 42, i32 42)
  %152 = zext i1 %151 to i32
  %153 = call i32 (ptr, ...) @printf(ptr @format, i32 %152)
  %154 = call i1 @icmp_ne_ni(i32 42, i32 -43)
  %155 = zext i1 %154 to i32
  %156 = call i32 (ptr, ...) @printf(ptr @format, i32 %155)
  %157 = call i1 @icmp_ne_ni(i32 -43, i32 42)
  %158 = zext i1 %157 to i32
  %159 = call i32 (ptr, ...) @printf(ptr @format, i32 %158)
  %160 = call i1 @icmp_ne_ni(i32 42, i32 42)
  %161 = zext i1 %160 to i32
  %162 = call i32 (ptr, ...) @printf(ptr @format, i32 %161)
  %163 = call i1 @icmp_slt_ni(i32 42, i32 -43)
  %164 = zext i1 %163 to i32
  %165 = call i32 (ptr, ...) @printf(ptr @format, i32 %164)
  %166 = call i1 @icmp_slt_ni(i32 -43, i32 42)
  %167 = zext i1 %166 to i32
  %168 = call i32 (ptr, ...) @printf(ptr @format, i32 %167)
  %169 = call i1 @icmp_slt_ni(i32 42, i32 42)
  %170 = zext i1 %169 to i32
  %171 = call i32 (ptr, ...) @printf(ptr @format, i32 %170)
  %172 = call i1 @icmp_sgt_ni(i32 42, i32 -43)
  %173 = zext i1 %172 to i32
  %174 = call i32 (ptr, ...) @printf(ptr @format, i32 %173)
  %175 = call i1 @icmp_sgt_ni(i32 -43, i32 42)
  %176 = zext i1 %175 to i32
  %177 = call i32 (ptr, ...) @printf(ptr @format, i32 %176)
  %178 = call i1 @icmp_sgt_ni(i32 42, i32 42)
  %179 = zext i1 %178 to i32
  %180 = call i32 (ptr, ...) @printf(ptr @format, i32 %179)
  %181 = call i1 @icmp_sle_ni(i32 42, i32 -43)
  %182 = zext i1 %181 to i32
  %183 = call i32 (ptr, ...) @printf(ptr @format, i32 %182)
  %184 = call i1 @icmp_sle_ni(i32 -43, i32 42)
  %185 = zext i1 %184 to i32
  %186 = call i32 (ptr, ...) @printf(ptr @format, i32 %185)
  %187 = call i1 @icmp_sle_ni(i32 42, i32 42)
  %188 = zext i1 %187 to i32
  %189 = call i32 (ptr, ...) @printf(ptr @format, i32 %188)
  %190 = call i1 @icmp_sge_ni(i32 42, i32 -43)
  %191 = zext i1 %190 to i32
  %192 = call i32 (ptr, ...) @printf(ptr @format, i32 %191)
  %193 = call i1 @icmp_sge_ni(i32 -43, i32 42)
  %194 = zext i1 %193 to i32
  %195 = call i32 (ptr, ...) @printf(ptr @format, i32 %194)
  %196 = call i1 @icmp_sge_ni(i32 42, i32 42)
  %197 = zext i1 %196 to i32
  %198 = call i32 (ptr, ...) @printf(ptr @format, i32 %197)
  %199 = call i1 @icmp_ult_ni(i32 42, i32 -43)
  %200 = zext i1 %199 to i32
  %201 = call i32 (ptr, ...) @printf(ptr @format, i32 %200)
  %202 = call i1 @icmp_ult_ni(i32 -43, i32 42)
  %203 = zext i1 %202 to i32
  %204 = call i32 (ptr, ...) @printf(ptr @format, i32 %203)
  %205 = call i1 @icmp_ult_ni(i32 42, i32 42)
  %206 = zext i1 %205 to i32
  %207 = call i32 (ptr, ...) @printf(ptr @format, i32 %206)
  %208 = call i1 @icmp_ugt_ni(i32 42, i32 -43)
  %209 = zext i1 %208 to i32
  %210 = call i32 (ptr, ...) @printf(ptr @format, i32 %209)
  %211 = call i1 @icmp_ugt_ni(i32 -43, i32 42)
  %212 = zext i1 %211 to i32
  %213 = call i32 (ptr, ...) @printf(ptr @format, i32 %212)
  %214 = call i1 @icmp_ugt_ni(i32 42, i32 42)
  %215 = zext i1 %214 to i32
  %216 = call i32 (ptr, ...) @printf(ptr @format, i32 %215)
  %217 = call i1 @icmp_ule_ni(i32 42, i32 -43)
  %218 = zext i1 %217 to i32
  %219 = call i32 (ptr, ...) @printf(ptr @format, i32 %218)
  %220 = call i1 @icmp_ule_ni(i32 -43, i32 42)
  %221 = zext i1 %220 to i32
  %222 = call i32 (ptr, ...) @printf(ptr @format, i32 %221)
  %223 = call i1 @icmp_ule_ni(i32 42, i32 42)
  %224 = zext i1 %223 to i32
  %225 = call i32 (ptr, ...) @printf(ptr @format, i32 %224)
  %226 = call i1 @icmp_uge_ni(i32 42, i32 -43)
  %227 = zext i1 %226 to i32
  %228 = call i32 (ptr, ...) @printf(ptr @format, i32 %227)
  %229 = call i1 @icmp_uge_ni(i32 -43, i32 42)
  %230 = zext i1 %229 to i32
  %231 = call i32 (ptr, ...) @printf(ptr @format, i32 %230)
  %232 = call i1 @icmp_uge_ni(i32 42, i32 42)
  %233 = zext i1 %232 to i32
  %234 = call i32 (ptr, ...) @printf(ptr @format, i32 %233)
  %235 = call i1 @fcmp_oeq_ni(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %236 = zext i1 %235 to i32
  %237 = call i32 (ptr, ...) @printf(ptr @format, i32 %236)
  %238 = call i1 @fcmp_oeq_ni(double 0x4005c28f5c28f5c3, double 0x40091eb851eb851f)
  %239 = zext i1 %238 to i32
  %240 = call i32 (ptr, ...) @printf(ptr @format, i32 %239)
  %241 = call i1 @fcmp_oeq_ni(double 0x40091eb851eb851f, double 0x40091eb851eb851f)
  %242 = zext i1 %241 to i32
  %243 = call i32 (ptr, ...) @printf(ptr @format, i32 %242)
  %244 = call i1 @fcmp_one_ni(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %245 = zext i1 %244 to i32
  %246 = call i32 (ptr, ...) @printf(ptr @format, i32 %245)
  %247 = call i1 @fcmp_one_ni(double 0x4005c28f5c28f5c3, double 0x40091eb851eb851f)
  %248 = zext i1 %247 to i32
  %249 = call i32 (ptr, ...) @printf(ptr @format, i32 %248)
  %250 = call i1 @fcmp_one_ni(double 0x40091eb851eb851f, double 0x40091eb851eb851f)
  %251 = zext i1 %250 to i32
  %252 = call i32 (ptr, ...) @printf(ptr @format, i32 %251)
  %253 = call i1 @fcmp_olt_ni(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %254 = zext i1 %253 to i32
  %255 = call i32 (ptr, ...) @printf(ptr @format, i32 %254)
  %256 = call i1 @fcmp_olt_ni(double 0x4005c28f5c28f5c3, double 0x40091eb851eb851f)
  %257 = zext i1 %256 to i32
  %258 = call i32 (ptr, ...) @printf(ptr @format, i32 %257)
  %259 = call i1 @fcmp_olt_ni(double 0x40091eb851eb851f, double 0x40091eb851eb851f)
  %260 = zext i1 %259 to i32
  %261 = call i32 (ptr, ...) @printf(ptr @format, i32 %260)
  %262 = call i1 @fcmp_ogt_ni(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %263 = zext i1 %262 to i32
  %264 = call i32 (ptr, ...) @printf(ptr @format, i32 %263)
  %265 = call i1 @fcmp_ogt_ni(double 0x4005c28f5c28f5c3, double 0x40091eb851eb851f)
  %266 = zext i1 %265 to i32
  %267 = call i32 (ptr, ...) @printf(ptr @format, i32 %266)
  %268 = call i1 @fcmp_ogt_ni(double 0x40091eb851eb851f, double 0x40091eb851eb851f)
  %269 = zext i1 %268 to i32
  %270 = call i32 (ptr, ...) @printf(ptr @format, i32 %269)
  %271 = call i1 @fcmp_ole_ni(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %272 = zext i1 %271 to i32
  %273 = call i32 (ptr, ...) @printf(ptr @format, i32 %272)
  %274 = call i1 @fcmp_ole_ni(double 0x4005c28f5c28f5c3, double 0x40091eb851eb851f)
  %275 = zext i1 %274 to i32
  %276 = call i32 (ptr, ...) @printf(ptr @format, i32 %275)
  %277 = call i1 @fcmp_ole_ni(double 0x40091eb851eb851f, double 0x40091eb851eb851f)
  %278 = zext i1 %277 to i32
  %279 = call i32 (ptr, ...) @printf(ptr @format, i32 %278)
  %280 = call i1 @fcmp_oge_ni(double 0x40091eb851eb851f, double 0x4005c28f5c28f5c3)
  %281 = zext i1 %280 to i32
  %282 = call i32 (ptr, ...) @printf(ptr @format, i32 %281)
  %283 = call i1 @fcmp_oge_ni(double 0x4005c28f5c28f5c3, double 0x40091eb851eb851f)
  %284 = zext i1 %283 to i32
  %285 = call i32 (ptr, ...) @printf(ptr @format, i32 %284)
  %286 = call i1 @fcmp_oge_ni(double 0x40091eb851eb851f, double 0x40091eb851eb851f)
  %287 = zext i1 %286 to i32
  %288 = call i32 (ptr, ...) @printf(ptr @format, i32 %287)
  ret i32 0
}
