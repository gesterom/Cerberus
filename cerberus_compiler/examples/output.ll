; ModuleID = 'CoreModule'
source_filename = "CoreModule"

%0 = type { ptr, i32 }

@stackscript_stack = global [1024 x i32] zeroinitializer
@stack_pointer = global i32 0
@0 = private unnamed_addr constant [6 x i8] c"%d %c\00", align 1
@1 = private unnamed_addr constant [10 x i8] c" True %d\0A\00", align 1
@2 = private unnamed_addr constant [9 x i8] c" False \0A\00", align 1
@3 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@4 = private unnamed_addr constant [24 x i8] c"Znalezione w %d krokach\00", align 1
@5 = private unnamed_addr constant [8 x i8] c"Advent\0A\00", align 1
@6 = private unnamed_addr constant [13 x i8] c"Length : %d\0A\00", align 1
@7 = private unnamed_addr constant [4 x i8] c" > \00", align 1
@8 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@9 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@10 = private unnamed_addr constant [7 x i8] c"Mniej\0A\00", align 1
@11 = private unnamed_addr constant [8 x i8] c"Wiecej\0A\00", align 1
@12 = private unnamed_addr constant [11 x i8] c"Gratulacje\00", align 1
@13 = private unnamed_addr constant [41 x i8] c"1abc2\0Apqr3stu8vwx\0Aa1b2c3d4e5f\0Atreb7uchet\00", align 1
@14 = private unnamed_addr constant [15 x i8] c"Advent 1 : %d\0A\00", align 1
@15 = private unnamed_addr constant [13 x i8] c"Square : %d\0A\00", align 1

declare i32 @printf(ptr, ...)

declare i32 @scanf(ptr, ...)

declare i32 @rand()

; Function Attrs: nounwind
define void @fooStruct(%0 %0) #0 {
entry:
  %1 = alloca %0, align 8
  store %0 %0, ptr %1, align 8
  %2 = load %0, ptr %1, align 8
  %3 = getelementptr i32, ptr %1, i32 1
  %4 = load i32, ptr %3, align 4
  store i32 10, ptr %3, align 4
  ret void
}

; Function Attrs: nounwind
define i32 @strlen(ptr %0) #0 {
entry:
  %1 = alloca ptr, align 8
  store ptr %0, ptr %1, align 8
  %res = alloca i32, align 4
  store i32 0, ptr %res, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %2 = load ptr, ptr %1, align 8
  %3 = load i32, ptr %res, align 4
  %4 = getelementptr i8, ptr %2, i32 %3
  %5 = load i8, ptr %4, align 1
  %cmptmp = icmp ne i8 %5, 0
  br i1 %cmptmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %6 = load i32, ptr %res, align 4
  %7 = add i32 %6, 1
  store i32 %7, ptr %res, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %8 = load i32, ptr %res, align 4
  ret i32 %8
}

; Function Attrs: nounwind
define void @printByChars(ptr %0) #0 {
entry:
  %1 = alloca ptr, align 8
  store ptr %0, ptr %1, align 8
  %res = alloca i32, align 4
  store i32 0, ptr %res, align 4
  br label %while.cond

while.cond:                                       ; preds = %if.end, %entry
  %2 = load ptr, ptr %1, align 8
  %3 = load i32, ptr %res, align 4
  %4 = getelementptr i8, ptr %2, i32 %3
  %5 = load i8, ptr %4, align 1
  %cmptmp = icmp ne i8 %5, 0
  br i1 %cmptmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %6 = load ptr, ptr %1, align 8
  %7 = load i32, ptr %res, align 4
  %8 = getelementptr i8, ptr %6, i32 %7
  %9 = load i8, ptr %8, align 1
  %10 = load ptr, ptr %1, align 8
  %11 = load i32, ptr %res, align 4
  %12 = getelementptr i8, ptr %10, i32 %11
  %13 = load i8, ptr %12, align 1
  %14 = call i32 (ptr, ...) @printf(ptr @0, i8 %9, i8 %13)
  %15 = load ptr, ptr %1, align 8
  %16 = load i32, ptr %res, align 4
  %17 = getelementptr i8, ptr %15, i32 %16
  %18 = load i8, ptr %17, align 1
  %19 = call i1 @isdigit(i8 %18)
  br i1 %19, label %if, label %if.else

while.end:                                        ; preds = %while.cond
  ret void

if:                                               ; preds = %while.body
  %20 = load ptr, ptr %1, align 8
  %21 = load i32, ptr %res, align 4
  %22 = getelementptr i8, ptr %20, i32 %21
  %23 = load i8, ptr %22, align 1
  %subtmp = sub i8 %23, 48
  %24 = call i32 (ptr, ...) @printf(ptr @1, i8 %subtmp)
  br label %if.end

if.else:                                          ; preds = %while.body
  %25 = call i32 (ptr, ...) @printf(ptr @2)
  br label %if.end

if.end:                                           ; preds = %if.else, %if
  %26 = load i32, ptr %res, align 4
  %addtmp = add i32 %26, 1
  %27 = load i32, ptr %res, align 4
  store i32 %addtmp, ptr %res, align 4
  br label %while.cond
}

; Function Attrs: nounwind
define i1 @isdigit(i8 %0) #0 {
entry:
  %1 = alloca i8, align 1
  store i8 %0, ptr %1, align 1
  %2 = load i8, ptr %1, align 1
  %letmp = icmp sle i8 %2, 57
  %3 = load i8, ptr %1, align 1
  %getmp = icmp sge i8 %3, 48
  %and_result = select i1 %getmp, i1 %letmp, i1 false
  br i1 %and_result, label %if, label %if.end

if:                                               ; preds = %entry
  ret i1 true
  br label %if.end

if.end:                                           ; preds = %if, %entry
  ret i1 false
}

declare void @srand(ptr)

define i32 @foo() {
entry:
  store i32 0, ptr @stack_pointer, align 4
  %0 = load i32, ptr @stack_pointer, align 4
  %1 = getelementptr i32, ptr @stackscript_stack, i32 %0
  store i32 15, ptr %1, align 4
  %2 = add i32 %0, 1
  store i32 %2, ptr @stack_pointer, align 4
  %3 = load i32, ptr @stack_pointer, align 4
  %4 = sub i32 %3, 1
  %5 = getelementptr i32, ptr @stackscript_stack, i32 %4
  %6 = load i32, ptr %5, align 4
  ret i32 %6
}

define i32 @square(i32 %0) {
entry:
  store i32 0, ptr @stack_pointer, align 4
  %1 = load i32, ptr @stack_pointer, align 4
  %2 = getelementptr i32, ptr @stackscript_stack, i32 %1
  store i32 %0, ptr %2, align 4
  %3 = add i32 %1, 1
  store i32 %3, ptr @stack_pointer, align 4
  %4 = load i32, ptr @stack_pointer, align 4
  %5 = sub i32 %4, 1
  %6 = getelementptr i32, ptr @stackscript_stack, i32 %5
  %7 = load i32, ptr %6, align 4
  %8 = getelementptr i32, ptr @stackscript_stack, i32 %4
  store i32 %7, ptr %8, align 4
  %9 = add i32 %4, 1
  store i32 %9, ptr @stack_pointer, align 4
  %10 = load i32, ptr @stack_pointer, align 4
  %11 = sub i32 %10, 1
  %12 = sub i32 %11, 1
  %13 = getelementptr i32, ptr @stackscript_stack, i32 %11
  %14 = load i32, ptr %13, align 4
  %15 = getelementptr i32, ptr @stackscript_stack, i32 %12
  %16 = load i32, ptr %15, align 4
  %17 = mul i32 %14, %16
  %18 = getelementptr i32, ptr @stackscript_stack, i32 %12
  store i32 %17, ptr %18, align 4
  store i32 %11, ptr @stack_pointer, align 4
  %19 = load i32, ptr @stack_pointer, align 4
  %20 = sub i32 %19, 1
  %21 = getelementptr i32, ptr @stackscript_stack, i32 %20
  %22 = load i32, ptr %21, align 4
  ret i32 %22
}

define i32 @pitagoras(i32 %0, i32 %1) {
entry:
  store i32 0, ptr @stack_pointer, align 4
  %2 = load i32, ptr @stack_pointer, align 4
  %3 = getelementptr i32, ptr @stackscript_stack, i32 %2
  store i32 %0, ptr %3, align 4
  %4 = add i32 %2, 1
  store i32 %4, ptr @stack_pointer, align 4
  %5 = load i32, ptr @stack_pointer, align 4
  %6 = getelementptr i32, ptr @stackscript_stack, i32 %5
  store i32 %1, ptr %6, align 4
  %7 = add i32 %5, 1
  store i32 %7, ptr @stack_pointer, align 4
  %8 = load i32, ptr @stack_pointer, align 4
  %9 = sub i32 %8, 1
  %10 = getelementptr i32, ptr @stackscript_stack, i32 %9
  %11 = load i32, ptr %10, align 4
  %12 = getelementptr i32, ptr @stackscript_stack, i32 %8
  store i32 %11, ptr %12, align 4
  %13 = add i32 %8, 1
  store i32 %13, ptr @stack_pointer, align 4
  %14 = load i32, ptr @stack_pointer, align 4
  %15 = sub i32 %14, 1
  %16 = sub i32 %15, 1
  %17 = getelementptr i32, ptr @stackscript_stack, i32 %15
  %18 = load i32, ptr %17, align 4
  %19 = getelementptr i32, ptr @stackscript_stack, i32 %16
  %20 = load i32, ptr %19, align 4
  %21 = mul i32 %18, %20
  %22 = getelementptr i32, ptr @stackscript_stack, i32 %16
  store i32 %21, ptr %22, align 4
  store i32 %15, ptr @stack_pointer, align 4
  %23 = load i32, ptr @stack_pointer, align 4
  %24 = sub i32 %23, 1
  %25 = getelementptr i32, ptr @stackscript_stack, i32 %23
  %26 = getelementptr i32, ptr @stackscript_stack, i32 %24
  %27 = load i32, ptr %25, align 4
  %28 = load i32, ptr %26, align 4
  store i32 %27, ptr %26, align 4
  store i32 %28, ptr %25, align 4
  %29 = load i32, ptr @stack_pointer, align 4
  %30 = sub i32 %29, 1
  %31 = getelementptr i32, ptr @stackscript_stack, i32 %30
  %32 = load i32, ptr %31, align 4
  %33 = getelementptr i32, ptr @stackscript_stack, i32 %29
  store i32 %32, ptr %33, align 4
  %34 = add i32 %29, 1
  store i32 %34, ptr @stack_pointer, align 4
  %35 = load i32, ptr @stack_pointer, align 4
  %36 = sub i32 %35, 1
  %37 = sub i32 %36, 1
  %38 = getelementptr i32, ptr @stackscript_stack, i32 %36
  %39 = load i32, ptr %38, align 4
  %40 = getelementptr i32, ptr @stackscript_stack, i32 %37
  %41 = load i32, ptr %40, align 4
  %42 = mul i32 %39, %41
  %43 = getelementptr i32, ptr @stackscript_stack, i32 %37
  store i32 %42, ptr %43, align 4
  store i32 %36, ptr @stack_pointer, align 4
  %44 = load i32, ptr @stack_pointer, align 4
  %45 = sub i32 %44, 1
  %46 = sub i32 %45, 1
  %47 = getelementptr i32, ptr @stackscript_stack, i32 %45
  %48 = load i32, ptr %47, align 4
  %49 = getelementptr i32, ptr @stackscript_stack, i32 %46
  %50 = load i32, ptr %49, align 4
  %51 = add i32 %48, %50
  %52 = getelementptr i32, ptr @stackscript_stack, i32 %46
  store i32 %51, ptr %52, align 4
  store i32 %45, ptr @stack_pointer, align 4
  %53 = load i32, ptr @stack_pointer, align 4
  %54 = sub i32 %53, 1
  %55 = getelementptr i32, ptr @stackscript_stack, i32 %54
  %56 = load i32, ptr %55, align 4
  ret i32 %56
}

; Function Attrs: nounwind
define i32 @aga() #0 {
entry:
  %num = alloca i32, align 4
  store i32 0, ptr %num, align 4
  %c = alloca i1, align 1
  store i1 true, ptr %c, align 1
  %a = alloca i32, align 4
  store i32 0, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 65535, ptr %b, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  %0 = load i32, ptr %num, align 4
  %1 = call i32 (ptr, ...) @scanf(ptr @3, ptr %num)
  br label %while.cond

while.cond:                                       ; preds = %if.end, %entry
  %2 = load i1, ptr %c, align 1
  %cmptmp = icmp eq i1 %2, true
  br i1 %cmptmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %s = alloca i32, align 4
  %3 = load i32, ptr %a, align 4
  %4 = load i32, ptr %b, align 4
  %subtmp = sub i32 %3, %4
  %divtmp = sdiv exact i32 %subtmp, 2
  store i32 %divtmp, ptr %s, align 4
  %5 = load i32, ptr %s, align 4
  %6 = load i32, ptr %num, align 4
  %cmptmp1 = icmp eq i32 %5, %6
  br i1 %cmptmp1, label %if, label %if.end

while.end:                                        ; preds = %while.cond
  %7 = load i32, ptr %i, align 4
  %8 = call i32 (ptr, ...) @printf(ptr @4, i32 %7)
  ret i32 0

if:                                               ; preds = %while.body
  %9 = load i1, ptr %c, align 1
  store i1 false, ptr %c, align 1
  br label %if.end

if.end:                                           ; preds = %if, %while.body
  %10 = load i32, ptr %i, align 4
  %addtmp = add i32 %10, 1
  %11 = load i32, ptr %i, align 4
  store i32 %addtmp, ptr %i, align 4
  br label %while.cond
}

; Function Attrs: nounwind
define i32 @advent1(ptr %0) #0 {
entry:
  %1 = alloca ptr, align 8
  store ptr %0, ptr %1, align 8
  %2 = call i32 (ptr, ...) @printf(ptr @5)
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %3 = load ptr, ptr %1, align 8
  %4 = call i32 @strlen(ptr %3)
  %5 = call i32 (ptr, ...) @printf(ptr @6, i32 %4)
  br label %while.cond

while.cond:                                       ; preds = %while.end3, %entry
  %6 = load i32, ptr %i, align 4
  %7 = load ptr, ptr %1, align 8
  %8 = call i32 @strlen(ptr %7)
  %lttmp = icmp slt i32 %6, %8
  br i1 %lttmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %first = alloca i32, align 4
  store i32 0, ptr %first, align 4
  %last = alloca i32, align 4
  store i32 0, ptr %last, align 4
  br label %while.cond1

while.end:                                        ; preds = %while.cond
  %9 = load i32, ptr %result, align 4
  ret i32 %9

while.cond1:                                      ; preds = %if.end8, %while.body
  %10 = load i32, ptr %i, align 4
  %11 = load ptr, ptr %1, align 8
  %12 = call i32 @strlen(ptr %11)
  %lttmp4 = icmp slt i32 %10, %12
  %13 = load ptr, ptr %1, align 8
  %14 = load i32, ptr %i, align 4
  %15 = getelementptr i8, ptr %13, i32 %14
  %16 = load i8, ptr %15, align 1
  %cmptmp = icmp ne i8 %16, 10
  %and_result = select i1 %cmptmp, i1 %lttmp4, i1 false
  br i1 %and_result, label %while.body2, label %while.end3

while.body2:                                      ; preds = %while.cond1
  %17 = load i32, ptr %first, align 4
  %cmptmp5 = icmp eq i32 %17, 0
  %18 = load ptr, ptr %1, align 8
  %19 = load i32, ptr %i, align 4
  %20 = getelementptr i8, ptr %18, i32 %19
  %21 = load i8, ptr %20, align 1
  %22 = call i1 @isdigit(i8 %21)
  %and_result6 = select i1 %22, i1 %cmptmp5, i1 false
  br i1 %and_result6, label %if, label %if.end

while.end3:                                       ; preds = %while.cond1
  %23 = load i32, ptr %first, align 4
  %multmp = mul i32 %23, 10
  %24 = load i32, ptr %last, align 4
  %addtmp = add i32 %multmp, %24
  %25 = load i32, ptr %result, align 4
  %addtemp = add i32 %25, %addtmp
  %26 = load i32, ptr %result, align 4
  store i32 %addtmp, ptr %result, align 4
  %27 = load i32, ptr %i, align 4
  %28 = add i32 %27, 1
  store i32 %28, ptr %i, align 4
  br label %while.cond

if:                                               ; preds = %while.body2
  %29 = load ptr, ptr %1, align 8
  %30 = load i32, ptr %i, align 4
  %31 = getelementptr i8, ptr %29, i32 %30
  %32 = load i8, ptr %31, align 1
  %subtmp = sub i8 %32, 48
  %33 = load i32, ptr %first, align 4
  store i8 %subtmp, ptr %first, align 1
  br label %if.end

if.end:                                           ; preds = %if, %while.body2
  %34 = load ptr, ptr %1, align 8
  %35 = load i32, ptr %i, align 4
  %36 = getelementptr i8, ptr %34, i32 %35
  %37 = load i8, ptr %36, align 1
  %38 = call i1 @isdigit(i8 %37)
  br i1 %38, label %if7, label %if.end8

if7:                                              ; preds = %if.end
  %39 = load ptr, ptr %1, align 8
  %40 = load i32, ptr %i, align 4
  %41 = getelementptr i8, ptr %39, i32 %40
  %42 = load i8, ptr %41, align 1
  %subtmp9 = sub i8 %42, 48
  %43 = load i32, ptr %last, align 4
  store i8 %subtmp9, ptr %last, align 1
  br label %if.end8

if.end8:                                          ; preds = %if7, %if.end
  %44 = load i32, ptr %i, align 4
  %45 = add i32 %44, 1
  store i32 %45, ptr %i, align 4
  br label %while.cond1
}

; Function Attrs: nounwind
define i32 @guess() #0 {
entry:
  %a = alloca i32, align 4
  store i32 50, ptr %a, align 4
  %guess = alloca i32, align 4
  store i32 0, ptr %guess, align 4
  %quit = alloca i1, align 1
  store i1 false, ptr %quit, align 1
  br label %while.cond

while.cond:                                       ; preds = %if.end, %entry
  %0 = load i1, ptr %quit, align 1
  %cmptmp = icmp eq i1 %0, false
  br i1 %cmptmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %1 = call i32 (ptr, ...) @printf(ptr @7)
  %2 = call i32 @foo()
  %3 = call i32 (ptr, ...) @printf(ptr @8, i32 %2)
  %4 = load i32, ptr %guess, align 4
  %5 = call i32 (ptr, ...) @scanf(ptr @9, ptr %guess)
  %6 = load i32, ptr %guess, align 4
  %7 = load i32, ptr %a, align 4
  %gttmp = icmp sgt i32 %6, %7
  br i1 %gttmp, label %if, label %if.else

while.end:                                        ; preds = %while.cond
  ret i32 0

if:                                               ; preds = %while.body
  %8 = call i32 (ptr, ...) @printf(ptr @10)
  br label %if.end

if.else:                                          ; preds = %while.body
  %9 = load i32, ptr %guess, align 4
  %10 = load i32, ptr %a, align 4
  %lttmp = icmp slt i32 %9, %10
  br i1 %lttmp, label %if1, label %if.else2

if.end:                                           ; preds = %if.end3, %if
  br label %while.cond

if1:                                              ; preds = %if.else
  %11 = call i32 (ptr, ...) @printf(ptr @11)
  br label %if.end3

if.else2:                                         ; preds = %if.else
  %12 = load i1, ptr %quit, align 1
  store i1 true, ptr %quit, align 1
  %13 = call i32 (ptr, ...) @printf(ptr @12)
  br label %if.end3

if.end3:                                          ; preds = %if.else2, %if1
  br label %if.end
}

; Function Attrs: nounwind
define i32 @main() #0 {
entry:
  %in = alloca ptr, align 8
  store ptr @13, ptr %in, align 8
  %0 = load ptr, ptr %in, align 8
  %1 = call i32 @advent1(ptr %0)
  %2 = call i32 (ptr, ...) @printf(ptr @14, i32 %1)
  %3 = call i32 @square(i32 9)
  %4 = call i32 (ptr, ...) @printf(ptr @15, i32 %3)
  ret i32 0
}

attributes #0 = { nounwind }
