; ModuleID = 'output.ll'
source_filename = "CoreModule"

@0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@1 = private unnamed_addr constant [60 x i8] c"Elf with max calories have {maxElf} and was {indexOfMaxElf}\00", align 1

; Function Attrs: mustprogress nofree nounwind willreturn memory(argmem: read)
declare i64 @strlen(ptr nocapture) local_unnamed_addr #0

; Function Attrs: mustprogress nofree nounwind willreturn allockind("alloc,uninitialized") allocsize(0) memory(inaccessiblemem: readwrite)
declare noalias noundef ptr @malloc(i64 noundef) local_unnamed_addr #1

; Function Attrs: mustprogress nofree nounwind willreturn memory(argmem: readwrite)
declare ptr @strcpy(ptr noalias returned writeonly, ptr noalias nocapture readonly) local_unnamed_addr #2

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #3

; Function Attrs: nofree nounwind
declare noundef i32 @scanf(ptr nocapture noundef readonly, ...) local_unnamed_addr #3

; Function Attrs: mustprogress nofree nounwind willreturn
define ptr @concatStrings(ptr nocapture readonly %0, ptr nocapture readonly %1) local_unnamed_addr #4 {
entry:
  %2 = tail call i64 @strlen(ptr noundef nonnull dereferenceable(1) %0)
  %3 = tail call i64 @strlen(ptr noundef nonnull dereferenceable(1) %1)
  %4 = add i64 %3, %2
  %5 = tail call ptr @malloc(i64 %4)
  %6 = tail call ptr @strcpy(ptr noundef nonnull dereferenceable(1) %5, ptr noundef nonnull dereferenceable(1) %0)
  %7 = getelementptr ptr, ptr %5, i64 %2
  %8 = tail call ptr @strcpy(ptr noundef nonnull dereferenceable(1) %7, ptr noundef nonnull dereferenceable(1) %1)
  ret ptr %5
}

; Function Attrs: nofree nounwind
define i32 @main() local_unnamed_addr #3 {
entry:
  %0 = tail call i32 (ptr, ...) @scanf(ptr nonnull @0, i32 0)
  %gttmp10 = icmp sgt i32 %0, 0
  br i1 %gttmp10, label %while.body, label %while.end

while.body:                                       ; preds = %entry, %while.body
  %1 = tail call i32 (ptr, ...) @scanf(ptr nonnull @0, i32 0)
  %2 = tail call i32 (ptr, ...) @scanf(ptr nonnull @0, i32 0)
  %gttmp = icmp sgt i32 %2, 0
  br i1 %gttmp, label %while.body, label %while.end

while.end:                                        ; preds = %while.body, %entry
  %3 = tail call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @1)
  ret i32 0
}

attributes #0 = { mustprogress nofree nounwind willreturn memory(argmem: read) }
attributes #1 = { mustprogress nofree nounwind willreturn allockind("alloc,uninitialized") allocsize(0) memory(inaccessiblemem: readwrite) "alloc-family"="malloc" }
attributes #2 = { mustprogress nofree nounwind willreturn memory(argmem: readwrite) }
attributes #3 = { nofree nounwind }
attributes #4 = { mustprogress nofree nounwind willreturn }
