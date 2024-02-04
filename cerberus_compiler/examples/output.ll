; ModuleID = 'CoreModule'
source_filename = "CoreModule"

%"Char[]" = type { i32, i32, ptr }
%"Char[].0" = type { i32, i32, ptr }
%"Char[].1" = type { i32, i32, ptr }
%"Char[].2" = type { i32, i32, ptr }

@stackscript_stack = global [1024 x i32] zeroinitializer
@stack_pointer = global i32 0
@0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@1 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@2 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@3 = private unnamed_addr constant [5 x i8] c"try\0A\00", align 1
@4 = private unnamed_addr constant [13 x i8] c"\0Atry harder\0A\00", align 1
@5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(ptr, ...)

declare i32 @scanf(ptr, ...)

declare ptr @malloc(i64)

define i32 @printer() {
entry:
  store i32 0, ptr @stack_pointer, align 4
  %0 = load i32, ptr @stack_pointer, align 4
  %1 = getelementptr i32, ptr @stackscript_stack, i32 %0
  store i32 6, ptr %1, align 4
  %2 = add i32 %0, 1
  store i32 %2, ptr @stack_pointer, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %3 = load i32, ptr @stack_pointer, align 4
  %4 = sub i32 %3, 1
  %5 = getelementptr i32, ptr @stackscript_stack, i32 %4
  %6 = load i32, ptr %5, align 4
  %cmptmp = icmp ne i32 %6, 0
  br i1 %cmptmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %7 = load i32, ptr @stack_pointer, align 4
  %8 = sub i32 %7, 1
  %9 = getelementptr i32, ptr @stackscript_stack, i32 %8
  %10 = load i32, ptr %9, align 4
  %11 = call i32 (ptr, ...) @printf(ptr @0, i32 %10)
  %12 = load i32, ptr @stack_pointer, align 4
  %13 = getelementptr i32, ptr @stackscript_stack, i32 %12
  store i32 1, ptr %13, align 4
  %14 = add i32 %12, 1
  store i32 %14, ptr @stack_pointer, align 4
  %15 = load i32, ptr @stack_pointer, align 4
  %16 = sub i32 %15, 1
  %17 = sub i32 %16, 1
  %18 = getelementptr i32, ptr @stackscript_stack, i32 %16
  %19 = load i32, ptr %18, align 4
  %20 = getelementptr i32, ptr @stackscript_stack, i32 %17
  %21 = load i32, ptr %20, align 4
  %22 = sub i32 %21, %19
  %23 = getelementptr i32, ptr @stackscript_stack, i32 %17
  store i32 %22, ptr %23, align 4
  store i32 %16, ptr @stack_pointer, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %24 = load i32, ptr @stack_pointer, align 4
  %25 = sub i32 %24, 1
  %26 = getelementptr i32, ptr @stackscript_stack, i32 %25
  %27 = load i32, ptr %26, align 4
  ret i32 %27
}

define i32 @test() {
entry:
  store i32 0, ptr @stack_pointer, align 4
  %0 = load i32, ptr @stack_pointer, align 4
  %1 = getelementptr i32, ptr @stackscript_stack, i32 %0
  store i32 3, ptr %1, align 4
  %2 = add i32 %0, 1
  store i32 %2, ptr @stack_pointer, align 4
  %3 = load i32, ptr @stack_pointer, align 4
  %4 = sub i32 %3, 1
  %5 = getelementptr i32, ptr @stackscript_stack, i32 %4
  %6 = load i32, ptr %5, align 4
  %cmptmp = icmp ne i32 %6, 0
  br i1 %cmptmp, label %if, label %if.else

if:                                               ; preds = %entry
  %7 = load i32, ptr @stack_pointer, align 4
  %8 = getelementptr i32, ptr @stackscript_stack, i32 %7
  store i32 1, ptr %8, align 4
  %9 = add i32 %7, 1
  store i32 %9, ptr @stack_pointer, align 4
  %10 = load i32, ptr @stack_pointer, align 4
  %11 = sub i32 %10, 1
  %12 = getelementptr i32, ptr @stackscript_stack, i32 %11
  %13 = load i32, ptr %12, align 4
  %14 = call i32 (ptr, ...) @printf(ptr @1, i32 %13)
  br label %if.end

if.else:                                          ; preds = %entry
  %15 = load i32, ptr @stack_pointer, align 4
  %16 = getelementptr i32, ptr @stackscript_stack, i32 %15
  store i32 0, ptr %16, align 4
  %17 = add i32 %15, 1
  store i32 %17, ptr @stack_pointer, align 4
  %18 = load i32, ptr @stack_pointer, align 4
  %19 = sub i32 %18, 1
  %20 = getelementptr i32, ptr @stackscript_stack, i32 %19
  %21 = load i32, ptr %20, align 4
  %22 = call i32 (ptr, ...) @printf(ptr @2, i32 %21)
  br label %if.end

if.end:                                           ; preds = %if.else, %if
  %23 = load i32, ptr @stack_pointer, align 4
  %24 = sub i32 %23, 1
  %25 = getelementptr i32, ptr @stackscript_stack, i32 %24
  %26 = load i32, ptr %25, align 4
  ret i32 %26
}

define i32 @main() {
entry:
  %arrayStruct = alloca %"Char[]", align 8
  %sizePtr = getelementptr inbounds %"Char[]", ptr %arrayStruct, i32 0, i32 0
  %capacityPtr = getelementptr inbounds %"Char[]", ptr %arrayStruct, i32 0, i32 1
  %ptrPtr = getelementptr inbounds %"Char[]", ptr %arrayStruct, i32 0, i32 2
  store i32 5, ptr %sizePtr, align 4
  store i32 5, ptr %capacityPtr, align 4
  store ptr @3, ptr %ptrPtr, align 8
  %0 = load %"Char[]", ptr %arrayStruct, align 8
  %"Char[]->Ptr" = getelementptr inbounds %"Char[].0", ptr %arrayStruct, i32 0, i32 2
  %1 = load ptr, ptr %"Char[]->Ptr", align 8
  %2 = call i32 (ptr, ...) @printf(ptr %1)
  %3 = call i32 @printer()
  %arrayStruct1 = alloca %"Char[]", align 8
  %sizePtr2 = getelementptr inbounds %"Char[]", ptr %arrayStruct1, i32 0, i32 0
  %capacityPtr3 = getelementptr inbounds %"Char[]", ptr %arrayStruct1, i32 0, i32 1
  %ptrPtr4 = getelementptr inbounds %"Char[]", ptr %arrayStruct1, i32 0, i32 2
  store i32 13, ptr %sizePtr2, align 4
  store i32 13, ptr %capacityPtr3, align 4
  store ptr @4, ptr %ptrPtr4, align 8
  %4 = load %"Char[]", ptr %arrayStruct1, align 8
  %"Char[]->Ptr5" = getelementptr inbounds %"Char[].1", ptr %arrayStruct1, i32 0, i32 2
  %5 = load ptr, ptr %"Char[]->Ptr5", align 8
  %6 = call i32 (ptr, ...) @printf(ptr %5)
  %7 = call i32 @test()
  %arrayStruct6 = alloca %"Char[]", align 8
  %sizePtr7 = getelementptr inbounds %"Char[]", ptr %arrayStruct6, i32 0, i32 0
  %capacityPtr8 = getelementptr inbounds %"Char[]", ptr %arrayStruct6, i32 0, i32 1
  %ptrPtr9 = getelementptr inbounds %"Char[]", ptr %arrayStruct6, i32 0, i32 2
  store i32 2, ptr %sizePtr7, align 4
  store i32 2, ptr %capacityPtr8, align 4
  store ptr @5, ptr %ptrPtr9, align 8
  %8 = load %"Char[]", ptr %arrayStruct6, align 8
  %"Char[]->Ptr10" = getelementptr inbounds %"Char[].2", ptr %arrayStruct6, i32 0, i32 2
  %9 = load ptr, ptr %"Char[]->Ptr10", align 8
  %10 = call i32 (ptr, ...) @printf(ptr %9)
  ret i32 0
}
