include 'ccall.inc'
include 'common.asm'
include 'config.asm'
include 'linux.inc'
format elf64

; macro readBuffer bufname {
; 	local buf
; 	buf =  bufname
; 	; mov rdi, [#bufname .WT]
; 	mov rdi, [buf.WT]
;
; 	mov rbx, [bufname.current_note_idx]
; 	mov rdx, [bufname.notes_ptr]
; 	mov rdx, [rbx+rdx]
;
;         mov rsi, [rsp+16]
; 	movsd xmm0, [bufname.current_sample_idx]
; 	ccall ReadWT
;
; 	movsd [bufname.current_note_idx], xmm0
; 	inc [bufname.current_sample_idx]
; 	
; }

section '.text' executable

public Notes
public Current_sector 
public SequencerPlay
public SequencerInit
public SequencerStop
public Buffers
public Stream_ptr
; rept BUFFERS_AMOUNT i {
;  public Buf#i
; }

include 'pa/func.inc'
extrn printf
extrn exit

extrn ReadWT
extrn SineTblWT
extrn KickTblWT
extrn SawtoothTblWT
extrn SquareTblWT 
extrn SnareTblWT 
extrn NoiseTblWT

extrn INP_InputNote
extrn INP_WasInput
extrn INP_SelectedBuf


SequencerInit:
	ccall Pa_Initialize
	;int3
	test rax, rax
	jnz .init_err

	mov rdi, Stream_ptr; 
	mov rsi, 0 ; number of input channels
	mov rdx, 1 ; number of output channels
	mov rcx, paInt8 ; type of samples
	movsd xmm0, [sampling_frequency] 
	mov r8, SECTOR_SAMPLE_SIZE ; samples per callback
	mov r9, Callback
	
	; push <user_data> - not used here
	;int3
	ccall Pa_OpenDefaultStream
	test rax, rax
	jnz .init_err
	; int3

	; syscall sys_open, urandom_name, 0 
	; mov [urandom_fd], rax

	ret

    .init_err:
    	Ccall printf, init_err_str
	ccall exit

SequencerPlay:	
	Ccall Pa_StartStream, [Stream_ptr]
	ret

SequencerStop:
	Ccall Pa_StopStream, [Stream_ptr]
	ret



;---------------------------------------------

; rdi - input buffer [ptr]
; rsi - output buffer [ptr]
; rdx - frame count
; rcx - time info [ptr]
; r8  - status flagslua/telescope/actions.lua
; r9  - user data
; EXECUTED IN OTHER THREAD!!!
Callback:

	sub rsp, 32
	mov [rsp], rbx
	mov [rsp+8], rsi
	mov [rsp+16], rdx
	; int3


	; handle input
	cmp qword[INP_WasInput],  0 
	je .after_input
	; int3
	mov rdi, [INP_SelectedBuf] ; notes_ptr offset
	mov rdi, [rdi + 0]
	; int3
	mov rbx, [Current_sector]
	mov rdx, [INP_InputNote]
	mov [rdi+rbx], rdx
	; int3

.after_input:

	; put zeros in output buffer
	mov rcx, [rsp+16]
.clear: 
	mov byte[rsi], 0 
	inc rsi
	loop .clear


	mov rcx, [rsp+16]

	; add up all tracks and put into buffer

rept BUFFERS_AMOUNT i {	

	xor rdx, rdx
	; WT ptr => rdi
	lea rdi, [Buf#i#.notes_ptr]
	mov rdi, [Buf#i#.WT]
	; current note => rdx
	mov rsi, [Buf#i#.notes_ptr]
	mov rbx, [Current_sector]
	; int3
	mov dl, byte[rsi+rbx]
	; int3
	; sample idx => xmm0
	movsd xmm0, [Buf#i#.current_sample_idx]
	; destination => rsi
	mov rsi, [rsp+8]

	mov rcx, [rsp+16]

	ccall ReadWT

	movsd [Buf#i#.current_sample_idx], xmm0
}



	inc [Current_sector]

	cmp [Current_sector], BUFFER_SIZE_SECTORS	
	jb .continue_playback 
.end_playback:
rept BUFFERS_AMOUNT i {
	mov [Buf#i#.current_sample_idx], 0
}
	mov [Current_sector], 0
	mov rax, 1
	jmp .end
.continue_playback:
	mov rax, 0
.end:
	mov rbx, [rsp]
	add rsp,32
	ret


; temporarily for test 
; main:
; 	;int3
; 	ccall SequencerInit
; 	ccall SequencerPlay
; 	; int3
; 	Ccall Pa_Sleep, 4000
; 	; int3
; 	ccall SequencerStop
; 	; int3
; 	ccall Pa_Terminate
; 	mov rax, 0
; 	ccall exit
	

section '.data' writeable 

	label Buffers at Buf1
	Buf1 Buffer SineTblWT    , Notes + BUFFER_SIZE_SECTORS * 0, 0, 0, 0 
	Buf2 Buffer SineTblWT    , Notes + BUFFER_SIZE_SECTORS * 1, 0, 0, 0 
	Buf3 Buffer SawtoothTblWT, Notes + BUFFER_SIZE_SECTORS * 2, 0, 0, 0 
	Buf4 Buffer SawtoothTblWT, Notes + BUFFER_SIZE_SECTORS * 3, 0, 0, 0 
	Buf5 Buffer SquareTblWT  , Notes + BUFFER_SIZE_SECTORS * 4, 0, 0, 0
	Buf6 Buffer SquareTblWT  , Notes + BUFFER_SIZE_SECTORS * 5, 0, 0, 0
	; Buf7 Buffer SnareTblWT   , Notes + BUFFER_SIZE_SECTORS * 6, 0, 0, 1 
	; Buf8 Buffer KickTblWT    , Notes + BUFFER_SIZE_SECTORS * 7, 0, 0, 1 


	Notes db (BUFFER_SIZE_SECTORS * BUFFERS_AMOUNT) dup 0
	; Notes db \
	; 	512 dup 0,\
	; 	10 dup 0, 5 dup 0, 5 dup 17, 10 dup 20, 482 dup 0,\
	; 	10 dup 0, 4 dup 1, 6 dup 0, 10 dup 0, 4 dup 1, 6 dup 0, 472 dup 0,\
	; 	4 dup 1, 16 dup 0, 4 dup 1, 16 dup 0, 4 dup 1, 468 dup 0

	Stream_ptr dq ?

	last_wt_frame dq 0

	Current_sector dq 0

	Callback_count dq 0


	buf_notes db BUFFER_SIZE_SECTORS dup 0


	; urandom_name db "/dev/urandom", 0
	init_err_str db "Error initializing audio",0

	sampling_frequency dq SAMPLING_FREQUENCY
