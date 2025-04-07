macro ccall procaddr{
	sub rsp, 8
	call procaddr
	add rsp, 8
}

macro Ccall proc_addr, arg0, arg1, arg2, arg3, arg4, arg5, arg6 {
  if arg0 eq
  else
    mov rdi, arg0
  end if
  if arg1 eq
  else
    mov rsi, arg1
  end if
  if arg2 eq
  else
    mov rdx, arg2
  end if
  if arg3 eq
  else
    mov rcx, arg3 
  end if
  if arg4 eq
  else
    mov r8, arg4
  end if
  if arg5 eq
  else
    mov r9, arg5
  end if
  ccall proc_addr
}

