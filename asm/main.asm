
include 'linux.inc'
include 'ccall.inc'
include 'sdl/def.inc'
include 'config.asm'

format elf64


section '.text' executable

public main
extrn exit
extrn printf
include 'sdl/func.inc' ; SDL external declarations

init:
	Ccall SDL_Init, SDL_INIT_VIDEO
	test rax, rax
	jl error

	Ccall SDL_CreateWindow, window_title, SDL_WINDOWPOS_UNDEFINED_MASK, SDL_WINDOWPOS_UNDEFINED_MASK,\
				WINDOW_SIZE_X, WINDOW_SIZE_Y, 4
	mov [window], rax
	ret

terminate:

	Ccall SDL_DestroyWindow, [window]
	ccall SDL_Quit

main:
	call init
	mov rcx, 999000000
	loop $
	call terminate
	mov rax, 0
	ccall exit

error:
	Ccall printf, init_err_str 
	ccall exit
	
	
section '.data' writeable
	init_err_str db 'Error initialising SDL context', 0
	window_title db 'ASM MUSIC :3'
	window dq ?
