; ModuleID = 'output.ll'
source_filename = "CoreModule"

@stackscript_stack = local_unnamed_addr global [1024 x i32] zeroinitializer
@stack_pointer = local_unnamed_addr global i32 0
@0 = private unnamed_addr constant [24 x i8] c"Znalezione w %d krokach\00", align 1
@1 = private unnamed_addr constant [4 x i8] c" > \00", align 1
@2 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@3 = private unnamed_addr constant [11 x i8] c"Gratulacje\00", align 1
@str = private unnamed_addr constant [7 x i8] c"Wiecej\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"Mniej\00", align 1

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

; Function Attrs: nofree nounwind
declare noundef i32 @scanf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

declare i32 @strlen(ptr) local_unnamed_addr

declare i1 @isdigit(i8) local_unnamed_addr

; Function Attrs: nofree nounwind
define i32 @aga() local_unnamed_addr #0 {
entry:
  %num = alloca i32, align 4
  store i32 0, ptr %num, align 4
  %0 = call i32 (ptr, ...) @scanf(ptr nonnull @2, ptr nonnull %num)
  %1 = load i32, ptr %num, align 4
  %cmptmp1.not = icmp eq i32 %1, -32767
  br i1 %cmptmp1.not, label %while.body, label %while.body.us

while.body.us:                                    ; preds = %entry, %while.body.us
  br label %while.body.us

while.body:                                       ; preds = %entry
  %2 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @0, i32 1)
  ret i32 0
}

; Function Attrs: nounwind
define i32 @advent1(ptr %0) local_unnamed_addr #1 {
entry:
  %1 = tail call i32 @strlen(ptr %0) #1
  %lttmp19 = icmp sgt i32 %1, 0
  br i1 %lttmp19, label %while.body, label %while.end

while.body:                                       ; preds = %entry, %while.end3
  %first = alloca i32, align 4
  store i32 -1, ptr %first, align 4
  %last = alloca i32, align 4
  store i32 -1, ptr %last, align 4
  %2 = load i8, ptr %0, align 1
  %cmptmp.not17 = icmp eq i8 %2, 10
  br i1 %cmptmp.not17, label %while.end3, label %while.body2

while.cond.while.end_crit_edge:                   ; preds = %while.end3
  %multmp.le = mul i32 %6, 10
  %addtmp.le = add i32 %multmp.le, %7
  br label %while.end

while.end:                                        ; preds = %while.cond.while.end_crit_edge, %entry
  %result.0.lcssa = phi i32 [ %addtmp.le, %while.cond.while.end_crit_edge ], [ 0, %entry ]
  ret i32 %result.0.lcssa

while.body2:                                      ; preds = %while.body, %if.end
  %3 = phi i8 [ %.pre23, %if.end ], [ %2, %while.body ]
  %subtmp71618 = phi i8 [ %spec.select, %if.end ], [ -1, %while.body ]
  %4 = load i32, ptr %first, align 4
  %cmptmp4 = icmp eq i32 %4, -1
  %5 = tail call i1 @isdigit(i8 %3) #1
  %or_result = select i1 %5, i1 %cmptmp4, i1 false
  %.pre = load i8, ptr %0, align 1
  br i1 %or_result, label %if, label %if.end

while.end3.loopexit:                              ; preds = %if.end
  %.pre24 = load i32, ptr %first, align 4
  br label %while.end3

while.end3:                                       ; preds = %while.end3.loopexit, %while.body
  %6 = phi i32 [ -1, %while.body ], [ %.pre24, %while.end3.loopexit ]
  %subtmp716.lcssa = phi i8 [ -1, %while.body ], [ %spec.select, %while.end3.loopexit ]
  store i8 %subtmp716.lcssa, ptr %last, align 4
  %7 = load i32, ptr %last, align 4
  %8 = tail call i32 @strlen(ptr nonnull %0) #1
  %lttmp = icmp sgt i32 %8, 0
  br i1 %lttmp, label %while.body, label %while.cond.while.end_crit_edge

if:                                               ; preds = %while.body2
  %subtmp = add i8 %.pre, -48
  store i8 %subtmp, ptr %first, align 4
  br label %if.end

if.end:                                           ; preds = %if, %while.body2
  %9 = tail call i1 @isdigit(i8 %.pre) #1
  %.pre23 = load i8, ptr %0, align 1
  %subtmp7 = add i8 %.pre23, -48
  %spec.select = select i1 %9, i8 %subtmp7, i8 %subtmp71618
  %cmptmp.not = icmp eq i8 %.pre23, 10
  br i1 %cmptmp.not, label %while.end3.loopexit, label %while.body2
}

; Function Attrs: nofree nounwind
define i32 @main2() local_unnamed_addr #0 {
entry:
  %guess = alloca i32, align 4
  store i32 0, ptr %guess, align 4
  br label %while.body

while.body:                                       ; preds = %while.body.backedge, %entry
  %0 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @1)
  %1 = call i32 (ptr, ...) @scanf(ptr nonnull @2, ptr nonnull %guess)
  %2 = load i32, ptr %guess, align 4
  %gttmp = icmp sgt i32 %2, 50
  br i1 %gttmp, label %while.body.backedge, label %if.else

if.else:                                          ; preds = %while.body
  %lttmp.not = icmp eq i32 %2, 50
  br i1 %lttmp.not, label %if.else2, label %while.body.backedge

while.body.backedge:                              ; preds = %if.else, %while.body
  %str.sink = phi ptr [ @str.1, %while.body ], [ @str, %if.else ]
  %puts = call i32 @puts(ptr nonnull dereferenceable(1) %str.sink)
  br label %while.body

if.else2:                                         ; preds = %if.else
  %3 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @3)
  ret i32 0
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, argmem: none, inaccessiblemem: none)
define i32 @main() local_unnamed_addr #2 {
entry:
  %0 = load i32, ptr @stack_pointer, align 4
  %1 = sext i32 %0 to i64
  %2 = getelementptr i32, ptr @stackscript_stack, i64 %1
  store i32 1, ptr %2, align 4
  %3 = add i32 %0, 1
  %4 = sext i32 %3 to i64
  %5 = getelementptr i32, ptr @stackscript_stack, i64 %4
  %6 = add i32 %0, 2
  %7 = sext i32 %6 to i64
  %8 = getelementptr i32, ptr @stackscript_stack, i64 %7
  %9 = load i32, ptr %8, align 4
  %10 = add i32 %9, 1
  store i32 %10, ptr %5, align 4
  store i32 %3, ptr @stack_pointer, align 4
  ret i32 %10
}

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
attributes #1 = { nounwind }
attributes #2 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, argmem: none, inaccessiblemem: none) }
