; ModuleID = 'output.ll'
source_filename = "CoreModule"

%"Int[]" = type { i32, i32, ptr }
%"Char[]" = type { i32, i32, ptr }

@stackscript_stack = local_unnamed_addr global [1024 x i32] zeroinitializer
@stack_pointer = local_unnamed_addr global i32 0
@0 = private unnamed_addr constant [17 x i8] c"array size : %d\0A\00", align 1
@1 = private unnamed_addr constant [21 x i8] c"array capacity : %d\0A\00", align 1
@2 = private unnamed_addr constant [12 x i8] c"array : %p\0A\00", align 1
@3 = private unnamed_addr constant [17 x i8] c"array size : %p\0A\00", align 1
@4 = private unnamed_addr constant [21 x i8] c"array capacity : %p\0A\00", align 1
@5 = private unnamed_addr constant [17 x i8] c"array data : %p\0A\00", align 1
@6 = private unnamed_addr constant [12 x i8] c"Index : %d \00", align 1
@7 = private unnamed_addr constant [5 x i8] c"%d, \00", align 1
@8 = private unnamed_addr constant [15 x i8] c" Pointer : %p\0A\00", align 1
@9 = private unnamed_addr constant [29 x i8] c"i : %d res[%c / %d ]+1 : %d\0A\00", align 1
@10 = private unnamed_addr constant [5 x i8] c"%s \0A\00", align 1
@11 = private unnamed_addr constant [24 x i8] c"a size : %d, b size %d\0A\00", align 1
@12 = private unnamed_addr constant [32 x i8] c"a capacity : %d, b capacity %d\0A\00", align 1
@13 = private unnamed_addr constant [14 x i8] c"a : %p, b %p\0A\00", align 1
@14 = private unnamed_addr constant [24 x i8] c"a size : %p, b size %p\0A\00", align 1
@15 = private unnamed_addr constant [32 x i8] c"a capacity : %p, b capacity %p\0A\00", align 1
@16 = private unnamed_addr constant [24 x i8] c"a data : %p, b data %p\0A\00", align 1
@17 = private unnamed_addr constant [15 x i8] c"%d(%p) %d(%p)\0A\00", align 1
@18 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@19 = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@20 = private unnamed_addr constant [8 x i8] c" = %d \0A\00", align 1
@21 = private unnamed_addr constant [41 x i8] c"abbbaccdeeeffggghhhhiiijjabcdeffacccfffd\00", align 1
@22 = private unnamed_addr constant [16 x i8] c"text size : %d\0A\00", align 1
@23 = private unnamed_addr constant [20 x i8] c"text capacity : %d\0A\00", align 1
@24 = private unnamed_addr constant [11 x i8] c"text : %p\0A\00", align 1
@25 = private unnamed_addr constant [16 x i8] c"text size : %p\0A\00", align 1
@26 = private unnamed_addr constant [20 x i8] c"text capacity : %p\0A\00", align 1
@27 = private unnamed_addr constant [16 x i8] c"text data : %p\0A\00", align 1
@28 = private unnamed_addr constant [39 x i8] c"nr symbolu: %d, liczba wyst\C4\85pie\C5\84 %d\0A\00", align 1
@str = private unnamed_addr constant [5 x i8] c"size\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c"malloc\00", align 1
@str.3 = private unnamed_addr constant [7 x i8] c"strlen\00", align 1
@str.4 = private unnamed_addr constant [8 x i8] c"strlen3\00", align 1
@str.5 = private unnamed_addr constant [5 x i8] c"Last\00", align 1
@str.6 = private unnamed_addr constant [11 x i8] c"printArray\00", align 1
@str.8 = private unnamed_addr constant [12 x i8] c"printArray2\00", align 1
@str.9 = private unnamed_addr constant [6 x i8] c"func1\00", align 1
@str.11 = private unnamed_addr constant [6 x i8] c"func2\00", align 1

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

; Function Attrs: mustprogress nofree nounwind willreturn allockind("alloc,uninitialized") allocsize(0) memory(inaccessiblemem: readwrite)
declare noalias noundef ptr @malloc(i64 noundef) local_unnamed_addr #1

; Function Attrs: nofree nounwind
define %"Int[]" @licz(%"Char[]" %0) local_unnamed_addr #0 {
entry:
  %1 = alloca %"Char[]", align 8
  %.fca.0.extract = extractvalue %"Char[]" %0, 0
  store i32 %.fca.0.extract, ptr %1, align 8
  %.fca.1.extract = extractvalue %"Char[]" %0, 1
  %.fca.1.gep = getelementptr inbounds %"Char[]", ptr %1, i64 0, i32 1
  store i32 %.fca.1.extract, ptr %.fca.1.gep, align 4
  %.fca.2.extract = extractvalue %"Char[]" %0, 2
  %.fca.2.gep = getelementptr inbounds %"Char[]", ptr %1, i64 0, i32 2
  store ptr %.fca.2.extract, ptr %.fca.2.gep, align 8
  %2 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 %.fca.0.extract)
  %3 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @1, i32 %.fca.1.extract)
  %4 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @5, ptr %.fca.2.extract)
  %5 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @2, ptr nonnull %1)
  %6 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @3, ptr nonnull %1)
  %7 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @4, ptr nonnull %.fca.1.gep)
  %8 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @5, ptr nonnull %.fca.2.gep)
  %.fca.2.load193 = load ptr, ptr %.fca.2.gep, align 8
  %9 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @10, ptr %.fca.2.load193)
  %.fca.2.load202 = load ptr, ptr %.fca.2.gep, align 8
  %puts = call i32 @puts(ptr nonnull dereferenceable(1) %.fca.2.load202)
  %puts240 = call i32 @puts(ptr nonnull dereferenceable(1) @str)
  %10 = call dereferenceable_or_null(40) ptr @malloc(i64 40)
  %puts241 = call i32 @puts(ptr nonnull dereferenceable(1) @str.2)
  %11 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @6, i32 0)
  %12 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @7, i32 0)
  %13 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @6, i32 1)
  %14 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @7, i32 0)
  %15 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @6, i32 2)
  %16 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @7, i32 0)
  %17 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @6, i32 3)
  %18 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @7, i32 0)
  %19 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @6, i32 4)
  %20 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @7, i32 0)
  %21 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @6, i32 5)
  %22 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @7, i32 0)
  %23 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @6, i32 6)
  %24 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @7, i32 0)
  %25 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @6, i32 7)
  %26 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @7, i32 0)
  %27 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @6, i32 8)
  %28 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @7, i32 0)
  %29 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @6, i32 9)
  store i32 0, ptr %10, align 4
  %30 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @7, i32 0)
  %puts242 = call i32 @puts(ptr nonnull dereferenceable(1) @str.2)
  %puts.i = call i32 @puts(ptr nonnull dereferenceable(1) @str.6)
  %31 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @19, i32 0)
  %32 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @20, i32 0)
  %33 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @19, i32 1)
  %34 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @20, i32 0)
  %35 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @19, i32 2)
  %36 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @20, i32 0)
  %37 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @19, i32 3)
  %38 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @20, i32 0)
  %39 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @19, i32 4)
  %40 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @20, i32 0)
  %41 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @19, i32 5)
  %42 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @20, i32 0)
  %43 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @19, i32 6)
  %44 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @20, i32 0)
  %45 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @19, i32 7)
  %46 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @20, i32 0)
  %47 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @19, i32 8)
  %48 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @20, i32 0)
  %49 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @19, i32 9)
  %50 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @20, i32 0)
  %puts16.i = call i32 @puts(ptr nonnull dereferenceable(1) @str.8)
  %puts243 = call i32 @puts(ptr nonnull dereferenceable(1) @str.3)
  %.fca.0.load205252 = load i32, ptr %1, align 8
  %lttmp74253 = icmp sgt i32 %.fca.0.load205252, 0
  br i1 %lttmp74253, label %while.body72.peel, label %while.end73

while.body72.peel:                                ; preds = %entry
  %.fca.2.load211.peel = load ptr, ptr %.fca.2.gep, align 8
  %51 = load i8, ptr %.fca.2.load211.peel, align 1
  %subtmp.peel = add i8 %51, -97
  %52 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @8, ptr nonnull %.fca.2.load211.peel)
  %.fca.2.load238.peel = load ptr, ptr %.fca.2.gep, align 8
  %53 = load i8, ptr %.fca.2.load238.peel, align 1
  %k.sroa.0.0.insert.ext257 = zext i8 %subtmp.peel to i32
  %54 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @9, i32 0, i8 %53, i32 %k.sroa.0.0.insert.ext257, i32 1)
  store i32 1, ptr %10, align 4
  %.fca.0.load205.peel = load i32, ptr %1, align 8
  %lttmp74.peel = icmp sgt i32 %.fca.0.load205.peel, 1
  br i1 %lttmp74.peel, label %while.body72, label %while.end73

while.body72:                                     ; preds = %while.body72.peel, %while.body72
  %i.1254 = phi i32 [ %59, %while.body72 ], [ 1, %while.body72.peel ]
  %.fca.2.load211 = load ptr, ptr %.fca.2.gep, align 8
  %55 = load i8, ptr %.fca.2.load211, align 1
  %subtmp = add i8 %55, -97
  %56 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @8, ptr nonnull %.fca.2.load211)
  %.fca.2.load238 = load ptr, ptr %.fca.2.gep, align 8
  %57 = load i8, ptr %.fca.2.load238, align 1
  %k.sroa.0.0.insert.ext = zext i8 %subtmp to i32
  %58 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @9, i32 %i.1254, i8 %57, i32 %k.sroa.0.0.insert.ext, i32 2)
  store i32 1, ptr %10, align 4
  %59 = add nuw nsw i32 %i.1254, 1
  %.fca.0.load205 = load i32, ptr %1, align 8
  %lttmp74 = icmp slt i32 %59, %.fca.0.load205
  br i1 %lttmp74, label %while.body72, label %while.end73, !llvm.loop !0

while.end73:                                      ; preds = %while.body72.peel, %while.body72, %entry
  %60 = phi i32 [ 0, %entry ], [ 1, %while.body72 ], [ 1, %while.body72.peel ]
  %puts.i245 = call i32 @puts(ptr nonnull dereferenceable(1) @str.6)
  %61 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @19, i32 0)
  %62 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @20, i32 %60)
  %63 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @19, i32 1)
  %64 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @20, i32 %60)
  %65 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @19, i32 2)
  %66 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @20, i32 %60)
  %67 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @19, i32 3)
  %68 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @20, i32 %60)
  %69 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @19, i32 4)
  %70 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @20, i32 %60)
  %71 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @19, i32 5)
  %72 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @20, i32 %60)
  %73 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @19, i32 6)
  %74 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @20, i32 %60)
  %75 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @19, i32 7)
  %76 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @20, i32 %60)
  %77 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @19, i32 8)
  %78 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @20, i32 %60)
  %79 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @19, i32 9)
  %80 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @20, i32 %60)
  %.fca.2.insert35 = insertvalue %"Int[]" { i32 10, i32 10, ptr poison }, ptr %10, 2
  %puts16.i249 = call i32 @puts(ptr nonnull dereferenceable(1) @str.8)
  %puts244 = call i32 @puts(ptr nonnull dereferenceable(1) @str.4)
  ret %"Int[]" %.fca.2.insert35
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, argmem: none, inaccessiblemem: none)
define i32 @square(i32 %0) local_unnamed_addr #2 {
entry:
  store i32 %0, ptr getelementptr inbounds ([1024 x i32], ptr @stackscript_stack, i64 0, i64 1), align 4
  %1 = mul i32 %0, %0
  store i32 %1, ptr @stackscript_stack, align 16
  store i32 1, ptr @stack_pointer, align 4
  ret i32 %1
}

; Function Attrs: nofree nounwind
define void @printByChars(%"Char[]" %0) local_unnamed_addr #0 {
entry:
  %.fca.2.extract = extractvalue %"Char[]" %0, 2
  %1 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @10, ptr %.fca.2.extract)
  ret void
}

; Function Attrs: nofree nounwind
define void @testArrayCopy() local_unnamed_addr #0 {
entry:
  %a = alloca %"Int[]", align 8
  %0 = tail call dereferenceable_or_null(40) ptr @malloc(i64 40)
  store i32 10, ptr %a, align 8
  %.fca.2.insert124.fca.1.gep = getelementptr inbounds %"Int[]", ptr %a, i64 0, i32 1
  store i32 10, ptr %.fca.2.insert124.fca.1.gep, align 4
  %.fca.2.insert124.fca.2.gep = getelementptr inbounds %"Int[]", ptr %a, i64 0, i32 2
  store ptr %0, ptr %.fca.2.insert124.fca.2.gep, align 8
  %b = alloca %"Int[]", align 8
  store i32 10, ptr %b, align 8
  %.fca.1.gep = getelementptr inbounds %"Int[]", ptr %b, i64 0, i32 1
  store i32 10, ptr %.fca.1.gep, align 4
  %.fca.2.gep = getelementptr inbounds %"Int[]", ptr %b, i64 0, i32 2
  store ptr %0, ptr %.fca.2.gep, align 8
  %1 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @11, i32 10, i32 10)
  %2 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @12, i32 10, i32 10)
  %3 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @16, ptr %0, ptr %0)
  %4 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @13, ptr nonnull %a, ptr nonnull %b)
  %5 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @14, ptr nonnull %a, ptr nonnull %b)
  %6 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @15, ptr nonnull %.fca.2.insert124.fca.1.gep, ptr nonnull %.fca.1.gep)
  %7 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @16, ptr nonnull %.fca.2.insert124.fca.2.gep, ptr nonnull %.fca.2.gep)
  %.fca.2.load204 = load ptr, ptr %.fca.2.insert124.fca.2.gep, align 8
  store i32 0, ptr %.fca.2.load204, align 4
  %.fca.2.load213 = load ptr, ptr %.fca.2.insert124.fca.2.gep, align 8
  %8 = load i32, ptr %.fca.2.load213, align 4
  %.fca.2.load96 = load ptr, ptr %.fca.2.gep, align 8
  %9 = load i32, ptr %.fca.2.load96, align 4
  %10 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @17, i32 %8, ptr nonnull %.fca.2.load213, i32 %9, ptr nonnull %.fca.2.load96)
  %.fca.2.load204.1 = load ptr, ptr %.fca.2.insert124.fca.2.gep, align 8
  store i32 1, ptr %.fca.2.load204.1, align 4
  %.fca.2.load213.1 = load ptr, ptr %.fca.2.insert124.fca.2.gep, align 8
  %11 = load i32, ptr %.fca.2.load213.1, align 4
  %.fca.2.load96.1 = load ptr, ptr %.fca.2.gep, align 8
  %12 = load i32, ptr %.fca.2.load96.1, align 4
  %13 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @17, i32 %11, ptr nonnull %.fca.2.load213.1, i32 %12, ptr nonnull %.fca.2.load96.1)
  %.fca.2.load204.2 = load ptr, ptr %.fca.2.insert124.fca.2.gep, align 8
  store i32 2, ptr %.fca.2.load204.2, align 4
  %.fca.2.load213.2 = load ptr, ptr %.fca.2.insert124.fca.2.gep, align 8
  %14 = load i32, ptr %.fca.2.load213.2, align 4
  %.fca.2.load96.2 = load ptr, ptr %.fca.2.gep, align 8
  %15 = load i32, ptr %.fca.2.load96.2, align 4
  %16 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @17, i32 %14, ptr nonnull %.fca.2.load213.2, i32 %15, ptr nonnull %.fca.2.load96.2)
  %.fca.2.load204.3 = load ptr, ptr %.fca.2.insert124.fca.2.gep, align 8
  store i32 3, ptr %.fca.2.load204.3, align 4
  %.fca.2.load213.3 = load ptr, ptr %.fca.2.insert124.fca.2.gep, align 8
  %17 = load i32, ptr %.fca.2.load213.3, align 4
  %.fca.2.load96.3 = load ptr, ptr %.fca.2.gep, align 8
  %18 = load i32, ptr %.fca.2.load96.3, align 4
  %19 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @17, i32 %17, ptr nonnull %.fca.2.load213.3, i32 %18, ptr nonnull %.fca.2.load96.3)
  %.fca.2.load204.4 = load ptr, ptr %.fca.2.insert124.fca.2.gep, align 8
  store i32 4, ptr %.fca.2.load204.4, align 4
  %.fca.2.load213.4 = load ptr, ptr %.fca.2.insert124.fca.2.gep, align 8
  %20 = load i32, ptr %.fca.2.load213.4, align 4
  %.fca.2.load96.4 = load ptr, ptr %.fca.2.gep, align 8
  %21 = load i32, ptr %.fca.2.load96.4, align 4
  %22 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @17, i32 %20, ptr nonnull %.fca.2.load213.4, i32 %21, ptr nonnull %.fca.2.load96.4)
  %.fca.2.load204.5 = load ptr, ptr %.fca.2.insert124.fca.2.gep, align 8
  store i32 5, ptr %.fca.2.load204.5, align 4
  %.fca.2.load213.5 = load ptr, ptr %.fca.2.insert124.fca.2.gep, align 8
  %23 = load i32, ptr %.fca.2.load213.5, align 4
  %.fca.2.load96.5 = load ptr, ptr %.fca.2.gep, align 8
  %24 = load i32, ptr %.fca.2.load96.5, align 4
  %25 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @17, i32 %23, ptr nonnull %.fca.2.load213.5, i32 %24, ptr nonnull %.fca.2.load96.5)
  %.fca.2.load204.6 = load ptr, ptr %.fca.2.insert124.fca.2.gep, align 8
  store i32 6, ptr %.fca.2.load204.6, align 4
  %.fca.2.load213.6 = load ptr, ptr %.fca.2.insert124.fca.2.gep, align 8
  %26 = load i32, ptr %.fca.2.load213.6, align 4
  %.fca.2.load96.6 = load ptr, ptr %.fca.2.gep, align 8
  %27 = load i32, ptr %.fca.2.load96.6, align 4
  %28 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @17, i32 %26, ptr nonnull %.fca.2.load213.6, i32 %27, ptr nonnull %.fca.2.load96.6)
  %.fca.2.load204.7 = load ptr, ptr %.fca.2.insert124.fca.2.gep, align 8
  store i32 7, ptr %.fca.2.load204.7, align 4
  %.fca.2.load213.7 = load ptr, ptr %.fca.2.insert124.fca.2.gep, align 8
  %29 = load i32, ptr %.fca.2.load213.7, align 4
  %.fca.2.load96.7 = load ptr, ptr %.fca.2.gep, align 8
  %30 = load i32, ptr %.fca.2.load96.7, align 4
  %31 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @17, i32 %29, ptr nonnull %.fca.2.load213.7, i32 %30, ptr nonnull %.fca.2.load96.7)
  %.fca.2.load204.8 = load ptr, ptr %.fca.2.insert124.fca.2.gep, align 8
  store i32 8, ptr %.fca.2.load204.8, align 4
  %.fca.2.load213.8 = load ptr, ptr %.fca.2.insert124.fca.2.gep, align 8
  %32 = load i32, ptr %.fca.2.load213.8, align 4
  %.fca.2.load96.8 = load ptr, ptr %.fca.2.gep, align 8
  %33 = load i32, ptr %.fca.2.load96.8, align 4
  %34 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @17, i32 %32, ptr nonnull %.fca.2.load213.8, i32 %33, ptr nonnull %.fca.2.load96.8)
  %.fca.2.load204.9 = load ptr, ptr %.fca.2.insert124.fca.2.gep, align 8
  store i32 9, ptr %.fca.2.load204.9, align 4
  %.fca.2.load213.9 = load ptr, ptr %.fca.2.insert124.fca.2.gep, align 8
  %35 = load i32, ptr %.fca.2.load213.9, align 4
  %.fca.2.load96.9 = load ptr, ptr %.fca.2.gep, align 8
  %36 = load i32, ptr %.fca.2.load96.9, align 4
  %37 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @17, i32 %35, ptr nonnull %.fca.2.load213.9, i32 %36, ptr nonnull %.fca.2.load96.9)
  %.fca.2.load114 = load ptr, ptr %.fca.2.gep, align 8
  %38 = load i32, ptr %.fca.2.load114, align 4
  %39 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @18, i32 %38)
  %.fca.2.load114.1 = load ptr, ptr %.fca.2.gep, align 8
  %40 = load i32, ptr %.fca.2.load114.1, align 4
  %41 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @18, i32 %40)
  %.fca.2.load114.2 = load ptr, ptr %.fca.2.gep, align 8
  %42 = load i32, ptr %.fca.2.load114.2, align 4
  %43 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @18, i32 %42)
  %.fca.2.load114.3 = load ptr, ptr %.fca.2.gep, align 8
  %44 = load i32, ptr %.fca.2.load114.3, align 4
  %45 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @18, i32 %44)
  %.fca.2.load114.4 = load ptr, ptr %.fca.2.gep, align 8
  %46 = load i32, ptr %.fca.2.load114.4, align 4
  %47 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @18, i32 %46)
  %.fca.2.load114.5 = load ptr, ptr %.fca.2.gep, align 8
  %48 = load i32, ptr %.fca.2.load114.5, align 4
  %49 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @18, i32 %48)
  %.fca.2.load114.6 = load ptr, ptr %.fca.2.gep, align 8
  %50 = load i32, ptr %.fca.2.load114.6, align 4
  %51 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @18, i32 %50)
  %.fca.2.load114.7 = load ptr, ptr %.fca.2.gep, align 8
  %52 = load i32, ptr %.fca.2.load114.7, align 4
  %53 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @18, i32 %52)
  %.fca.2.load114.8 = load ptr, ptr %.fca.2.gep, align 8
  %54 = load i32, ptr %.fca.2.load114.8, align 4
  %55 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @18, i32 %54)
  %.fca.2.load114.9 = load ptr, ptr %.fca.2.gep, align 8
  %56 = load i32, ptr %.fca.2.load114.9, align 4
  %57 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @18, i32 %56)
  %puts = call i32 @puts(ptr nonnull dereferenceable(1) @str.5)
  ret void
}

; Function Attrs: nofree nounwind
define void @printArray(%"Int[]" %0, i32 %1) local_unnamed_addr #0 {
entry:
  %.fca.2.extract = extractvalue %"Int[]" %0, 2
  %puts = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.6)
  %lttmp17 = icmp sgt i32 %1, 0
  br i1 %lttmp17, label %while.body, label %while.end

while.body:                                       ; preds = %entry, %while.body
  %i.018 = phi i32 [ %5, %while.body ], [ 0, %entry ]
  %2 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @19, i32 %i.018)
  %3 = load i32, ptr %.fca.2.extract, align 4
  %4 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @20, i32 %3)
  %5 = add nuw nsw i32 %i.018, 1
  %lttmp = icmp slt i32 %5, %1
  br i1 %lttmp, label %while.body, label %while.end

while.end:                                        ; preds = %while.body, %entry
  %puts16 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.8)
  ret void
}

; Function Attrs: nofree nounwind
define i32 @main2() local_unnamed_addr #0 {
entry:
  tail call void @testArrayCopy()
  %puts = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.8)
  tail call void @testArrayCopy()
  ret i32 0
}

; Function Attrs: nofree nounwind
define i32 @main() local_unnamed_addr #0 {
entry:
  %puts = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.9)
  %text = alloca %"Char[]", align 8
  store i32 41, ptr %text, align 8
  %.fca.2.insert36.fca.1.gep = getelementptr inbounds %"Char[]", ptr %text, i64 0, i32 1
  store i32 41, ptr %.fca.2.insert36.fca.1.gep, align 4
  %.fca.2.insert36.fca.2.gep = getelementptr inbounds %"Char[]", ptr %text, i64 0, i32 2
  store ptr @21, ptr %.fca.2.insert36.fca.2.gep, align 8
  %0 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @22, i32 41)
  %1 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @23, i32 41)
  %2 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @27, ptr nonnull @21)
  %3 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @24, ptr nonnull %text)
  %4 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @25, ptr nonnull %text)
  %5 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @26, ptr nonnull %.fca.2.insert36.fca.1.gep)
  %6 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @27, ptr nonnull %.fca.2.insert36.fca.2.gep)
  %.fca.0.load95 = load i32, ptr %text, align 8
  %.fca.0.insert96 = insertvalue %"Char[]" poison, i32 %.fca.0.load95, 0
  %.fca.1.load98 = load i32, ptr %.fca.2.insert36.fca.1.gep, align 4
  %.fca.1.insert99 = insertvalue %"Char[]" %.fca.0.insert96, i32 %.fca.1.load98, 1
  %.fca.2.load101 = load ptr, ptr %.fca.2.insert36.fca.2.gep, align 8
  %.fca.2.insert102 = insertvalue %"Char[]" %.fca.1.insert99, ptr %.fca.2.load101, 2
  %7 = call %"Int[]" @licz(%"Char[]" %.fca.2.insert102)
  %.fca.2.extract = extractvalue %"Int[]" %7, 2
  %puts112 = call i32 @puts(ptr nonnull dereferenceable(1) @str.11)
  %puts113 = call i32 @puts(ptr nonnull dereferenceable(1) @str.11)
  %8 = load i32, ptr %.fca.2.extract, align 4
  %9 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @28, i32 0, i32 %8)
  %10 = load i32, ptr %.fca.2.extract, align 4
  %11 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @28, i32 1, i32 %10)
  %12 = load i32, ptr %.fca.2.extract, align 4
  %13 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @28, i32 2, i32 %12)
  %14 = load i32, ptr %.fca.2.extract, align 4
  %15 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @28, i32 3, i32 %14)
  %16 = load i32, ptr %.fca.2.extract, align 4
  %17 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @28, i32 4, i32 %16)
  %18 = load i32, ptr %.fca.2.extract, align 4
  %19 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @28, i32 5, i32 %18)
  %20 = load i32, ptr %.fca.2.extract, align 4
  %21 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @28, i32 6, i32 %20)
  %22 = load i32, ptr %.fca.2.extract, align 4
  %23 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @28, i32 7, i32 %22)
  %24 = load i32, ptr %.fca.2.extract, align 4
  %25 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @28, i32 8, i32 %24)
  %26 = load i32, ptr %.fca.2.extract, align 4
  %27 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @28, i32 9, i32 %26)
  ret i32 0
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define i32 @strlen(%"Char[]" %0) local_unnamed_addr #3 {
entry:
  %.fca.0.extract = extractvalue %"Char[]" %0, 0
  ret i32 %.fca.0.extract
}

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
attributes #1 = { mustprogress nofree nounwind willreturn allockind("alloc,uninitialized") allocsize(0) memory(inaccessiblemem: readwrite) "alloc-family"="malloc" }
attributes #2 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, argmem: none, inaccessiblemem: none) }
attributes #3 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }

!0 = distinct !{!0, !1}
!1 = !{!"llvm.loop.peeled.count", i32 1}
