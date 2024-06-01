declare i32 @__isoc99_scanf(i8* noundef, ...)
declare i32 @printf(i8* noundef, ...)

@.str = private unnamed_addr constant [4 x i8] c"%lf\00", align 1
@.str.1 = private unnamed_addr constant [5 x i8] c"%lf\0A\00", align 1

define dso_local double @func0(double %var0)
{
%var0p = alloca double, align 8
store double %var0, double* %var0p, align 8
%1 = alloca double, align 8
store double 0x3FF0000000000000, double* %1, align 8
%var1p = alloca double, align 8
%2 = load double, double* %1, align 8
store double %2, double* %var1p, align 8
br label %while0
while0:
%3 = alloca double, align 8
%4 = load double, double* %var0p, align 8
store double %4, double* %3, align 8
%5 = alloca double, align 8
store double 0x3FF0000000000000, double* %5, align 8
%6 = load double, double* %3, align 8
%7 = load double, double* %5, align 8
%8 = fcmp ogt double %6, %7
br i1 %8, label %body_while0, label %end_while0
body_while0:
%9 = alloca double, align 8
%10 = load double, double* %var1p, align 8
store double %10, double* %9, align 8
%11 = alloca double, align 8
%12 = load double, double* %var0p, align 8
store double %12, double* %11, align 8
%13 = load double, double* %9, align 8
%14 = load double, double* %11, align 8
%15 = fmul double %13, %14
%16 = alloca double, align 8
store double %15, double* %16, align 8
%17 = load double, double* %16, align 8
store double %17, double* %var1p, align 8
%18 = alloca double, align 8
%19 = load double, double* %var0p, align 8
store double %19, double* %18, align 8
%20 = alloca double, align 8
store double 0x3FF0000000000000, double* %20, align 8
%21 = load double, double* %18, align 8
%22 = load double, double* %20, align 8
%23 = fsub double %21, %22
%24 = alloca double, align 8
store double %23, double* %24, align 8
%25 = load double, double* %24, align 8
store double %25, double* %var0p, align 8
br label %while0
end_while0:
%26 = load double, double* %var1p, align 8
ret double %26
}

define dso_local i32 @main()
{
%1 = alloca double, align 8
store double 0x4020000000000000, double* %1, align 8
%var0p = alloca double, align 8
%2 = load double, double* %1, align 8
store double %2, double* %var0p, align 8
%3 = alloca double, align 8
store double 0x40C3880000000000, double* %3, align 8
%var1p = alloca double, align 8
%4 = load double, double* %3, align 8
store double %4, double* %var1p, align 8
%5 = alloca double, align 8
store double 0x3FF0000000000000, double* %5, align 8
%var2p = alloca double, align 8
%6 = load double, double* %5, align 8
store double %6, double* %var2p, align 8
br label %while1
while1:
%7 = alloca double, align 8
%8 = load double, double* %var1p, align 8
store double %8, double* %7, align 8
%9 = alloca double, align 8
store double 0x0, double* %9, align 8
%10 = load double, double* %7, align 8
%11 = load double, double* %9, align 8
%12 = fcmp ogt double %10, %11
br i1 %12, label %body_while1, label %end_while1
body_while1:
%13 = load double, double* %var0p, align 8
%14 = call double @func0(double noundef %13)
%15 = alloca double, align 8
store double %14, double* %15, align 8
%16 = load double, double* %15, align 8
store double %16, double* %var2p, align 8
%17 = alloca double, align 8
%18 = load double, double* %var1p, align 8
store double %18, double* %17, align 8
%19 = alloca double, align 8
store double 0x3FF0000000000000, double* %19, align 8
%20 = load double, double* %17, align 8
%21 = load double, double* %19, align 8
%22 = fsub double %20, %21
%23 = alloca double, align 8
store double %22, double* %23, align 8
%24 = load double, double* %23, align 8
store double %24, double* %var1p, align 8
br label %while1
end_while1:
%25 = load double, double* %var2p, align 8
%26 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([5 x i8], [5 x i8]* @.str.1, i64 0, i64 0), double noundef %25)
ret i32 0
}

