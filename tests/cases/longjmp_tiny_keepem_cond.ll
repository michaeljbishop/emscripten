;  ModuleID = '/tmp/emscripten_temp/src.cpp.o'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

@.str = private unnamed_addr constant [13 x i8] c"hello world\0A\00", align 1
@.str1 = private unnamed_addr constant [6 x i8] c"more\0A\00", align 1

define i32 @main() {
  %call0 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([6 x i8]* @.str1, i32 0, i32 0))
  %chak = icmp ne i32 %call0, 12345678
  br i1 %chak, label %middle, label %if.then

middle:
  %retval = alloca i32, align 4
  store i32 0, i32* %retval
  %wimpy = trunc i32 100 to i8
  %buffy = inttoptr i8 %wimpy to i16*
  %call = call i32 @setjmp(i16* %buffy) returns_twice ; 20
  %tobool = icmp ne i32 %call, 0 ; 20
  br i1 %tobool, label %if.else, label %if.then ; 20

if.then:                                          ;  preds = %entry
  %call1 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([13 x i8]* @.str, i32 0, i32 0)) ; 22
  call void @longjmp(i8 %wimpy, i32 10) ; 24
  br label %if.end ; 25

if.else:                                          ;  preds = %entry
  %call2 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([6 x i8]* @.str1, i32 0, i32 0)) ; 26
  br label %if.end

if.end:                                           ;  preds = %if.else, %if.then
  ret i32 0 ; 28
}

declare i32 @setjmp(i16*) returns_twice

declare i32 @printf(i8*, ...)

declare void @longjmp(i8, i32)


