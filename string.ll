; this is literally the worst

declare i8* @malloc(i32 %n) 

define i32 @string_len(i8* %str){

  %val = load i8, i8* %str
  br label %Loop
Loop:
  %cond = icmp eq i8 %val, 0
  br i1 %cond, label %End, label %Cont 
Cont:
  %len = add i32 %len, 1
  %nextptr = getelementptr i8, i8* %str, i8 1
  %nextval = load i8, i8* %nextptr
  store i8 %nextval, i8* %val
  br label %Loop
End:
  ret i32 %len
}

define i32 @string_cmp(i8* %lhs, i8* %rhs){
  entry:
    %len1 = call i32 @string_len(i8* %lhs) 
    %len2 = call i32 @string_len(i8* %rhs) 
    %start = i32 0

    ; which one is bigger?
    i32 %len = icmp sgt %len1, %len2
    br i1 %len, label %LEN1,  label %LEN2


    LEN1:
      %len = %len1
    LEN2:
      %len = %len2

    Loop:
      %cond = icmp eq %len, %start
      br i1 %cond, label %End, label %Get
    Get:
      %lval = getelementptr i8, i8* %lhs, i32 %start
      %rval = getelementptr i8, i8* %rhs, i32 %start
      %cond = icmp eq i8 %lval, %rval
      br i1 %cond, label %Eq, label %Noteq
    Eq:
      %start = add i32 %start, 1
      label %loop
    Noteq:
      %cond = icmp slt i8 %lval, %rval
      br %cond, label %Lhslt, label %Lhsgt
    Lhslt:
      ret -1
    Lhsgt:
      ret 1
    End:
      ret 0 
}

define i8* @string_chr(i8* %str, i8 %c){
  entry:
    i32 %len = call i32 @string_len(%str) 
    i32 %start = i32 0
    
    %endofstr = icmp eq i32 %len, %start
    br i1 %endofstr, label %end, label %loop
  Loop:
    %strval = getelementptr i8, i8* %str, i8 %start
    %cond = icmp eq i8 %strval, %c
    br i1 %cond, label %Eq, label %Noteq
  Eq:
    ret i8 %c
  Noteq:
    %start = add i32 %start, 1
    label %Loop
  End:
    ret null* ;? can't figure out how to ret nullptr
  
}

define i8* @string_cpy(i8* %dest, i8* %src){
  entry:
    i32 %len = call i32 @string_len(%src) 
    i32 %start = i32 0
    Loop:
      %endofsrc = icmp eq %len, %start
      br i1 %endofsrc label %End, label %Cpy
    Cpy:
      %srcval = getelementptr i8, i8* %src, i32 %start
      store i8 %srcval, i8* %dest
      %start = add i32 %start, 1
      label %Loop
    End:
      ret %dest 
}

define i8* @string_cat(i8* %dest, i8* %src){
  entry:
    i32 %len1 = call i32 @string_len(%dest) 
    i32 %len2 = call i32 @string_len(%src)
    i32 %start = i32 0 
    i32 %srcstart = add i32 %len1, 1

    %len = add i32 %len1, %len2
    i8 %ptr* = call i8* malloc(%len) 
    Loop:
      %endofstrs = icmp i32 %start, %len
      br i1 %endofstrs label %End, label %Cpy
    Cpy:
      %destval = getelementptr i8, i8* %dest, i32 %start
      %srcval = getelementptr i8, i8* %src, i32 %start
      store i8 $destval, i8* %ptr, i32 %start
      store i8 %srcval, i8* %ptr, i32 %srcstart
      %start = add i32 %start, 1
      %srcstart = add i32 %start, 1
      label %Loop
    End:
      ret %dest
}
