	TITLE	demo.c
	.386P
include listing.inc
if @Version gt 510
.model FLAT
else
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
_BSS	SEGMENT DWORD USE32 PUBLIC 'BSS'
_BSS	ENDS
_TLS	SEGMENT DWORD USE32 PUBLIC 'TLS'
_TLS	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	_writeFile
EXTRN	_fopen:NEAR
EXTRN	_fprintf:NEAR
_DATA	SEGMENT
$SG710	DB	'w', 00H
	ORG $+2
$SG711	DB	'%s', 00H
_DATA	ENDS
_TEXT	SEGMENT
_InputFileName$ = 8
_infor$ = 12
_fp$ = -4
_writeFile PROC NEAR
; File demo.c
; Line 12
	push	ebp
	mov	ebp, esp
	push	ecx
; Line 15
	push	OFFSET FLAT:$SG710
	mov	eax, DWORD PTR _InputFileName$[ebp]
	push	eax
	call	_fopen
	add	esp, 8
	mov	DWORD PTR _fp$[ebp], eax
; Line 16
	mov	ecx, DWORD PTR _infor$[ebp]
	push	ecx
	push	OFFSET FLAT:$SG711
	mov	edx, DWORD PTR _fp$[ebp]
	push	edx
	call	_fprintf
	add	esp, 12					; 0000000cH
; Line 17
	mov	esp, ebp
	pop	ebp
	ret	0
_writeFile ENDP
_TEXT	ENDS
PUBLIC	_swapData
_TEXT	SEGMENT
_x$ = 8
_y$ = 12
_swapData PROC NEAR
; Line 20
	push	ebp
	mov	ebp, esp
; Line 21
	mov	eax, DWORD PTR _x$[ebp]
	movsx	ecx, BYTE PTR [eax]
	mov	edx, DWORD PTR _y$[ebp]
	movsx	eax, BYTE PTR [edx]
	xor	ecx, eax
	mov	edx, DWORD PTR _x$[ebp]
	mov	BYTE PTR [edx], cl
; Line 22
	mov	eax, DWORD PTR _x$[ebp]
	movsx	ecx, BYTE PTR [eax]
	mov	edx, DWORD PTR _y$[ebp]
	movsx	eax, BYTE PTR [edx]
	xor	ecx, eax
	mov	edx, DWORD PTR _y$[ebp]
	mov	BYTE PTR [edx], cl
; Line 23
	mov	eax, DWORD PTR _x$[ebp]
	movsx	ecx, BYTE PTR [eax]
	mov	edx, DWORD PTR _y$[ebp]
	movsx	eax, BYTE PTR [edx]
	xor	ecx, eax
	mov	edx, DWORD PTR _x$[ebp]
	mov	BYTE PTR [edx], cl
; Line 24
	pop	ebp
	ret	0
_swapData ENDP
_TEXT	ENDS
PUBLIC	_getFileLength
EXTRN	_fclose:NEAR
EXTRN	_fseek:NEAR
EXTRN	_ftell:NEAR
_DATA	SEGMENT
	ORG $+1
$SG722	DB	'r', 00H
_DATA	ENDS
_TEXT	SEGMENT
_fileName$ = 8
_fp$ = -4
_file_length$ = -8
_getFileLength PROC NEAR
; Line 26
	push	ebp
	mov	ebp, esp
	sub	esp, 8
; Line 28
	mov	DWORD PTR _file_length$[ebp], 0
; Line 30
	push	OFFSET FLAT:$SG722
	mov	eax, DWORD PTR _fileName$[ebp]
	push	eax
	call	_fopen
	add	esp, 8
	mov	DWORD PTR _fp$[ebp], eax
; Line 31
	push	2
	push	0
	mov	ecx, DWORD PTR _fp$[ebp]
	push	ecx
	call	_fseek
	add	esp, 12					; 0000000cH
; Line 32
	mov	edx, DWORD PTR _fp$[ebp]
	push	edx
	call	_ftell
	add	esp, 4
	mov	DWORD PTR _file_length$[ebp], eax
; Line 33
	mov	eax, DWORD PTR _fp$[ebp]
	push	eax
	call	_fclose
	add	esp, 4
; Line 35
	mov	eax, DWORD PTR _file_length$[ebp]
; Line 36
	mov	esp, ebp
	pop	ebp
	ret	0
_getFileLength ENDP
_TEXT	ENDS
PUBLIC	_quickSortOnce
_TEXT	SEGMENT
_array$ = 8
_left$ = 12
_right$ = 16
_i$ = -8
_j$ = -12
_s$ = -4
_quickSortOnce PROC NEAR
; Line 38
	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
; Line 43
	mov	eax, DWORD PTR _left$[ebp]
	cmp	eax, DWORD PTR _right$[ebp]
	jge	$L733
; Line 44
	mov	eax, DWORD PTR _left$[ebp]
	add	eax, DWORD PTR _right$[ebp]
	cdq
	sub	eax, edx
	sar	eax, 1
	mov	ecx, DWORD PTR _array$[ebp]
	movsx	edx, BYTE PTR [ecx+eax]
	mov	DWORD PTR _s$[ebp], edx
; Line 45
	mov	eax, DWORD PTR _left$[ebp]
	sub	eax, 1
	mov	DWORD PTR _i$[ebp], eax
; Line 46
	mov	ecx, DWORD PTR _right$[ebp]
	add	ecx, 1
	mov	DWORD PTR _j$[ebp], ecx
$L735:
; Line 47
	mov	edx, 1
	test	edx, edx
	je	SHORT $L736
$L738:
; Line 48
	mov	eax, DWORD PTR _i$[ebp]
	add	eax, 1
	mov	DWORD PTR _i$[ebp], eax
	mov	ecx, DWORD PTR _array$[ebp]
	add	ecx, DWORD PTR _i$[ebp]
	movsx	edx, BYTE PTR [ecx]
	cmp	edx, DWORD PTR _s$[ebp]
	jge	SHORT $L739
	jmp	SHORT $L738
$L739:
; Line 49
	mov	eax, DWORD PTR _j$[ebp]
	sub	eax, 1
	mov	DWORD PTR _j$[ebp], eax
	mov	ecx, DWORD PTR _array$[ebp]
	add	ecx, DWORD PTR _j$[ebp]
	movsx	edx, BYTE PTR [ecx]
	cmp	edx, DWORD PTR _s$[ebp]
	jle	SHORT $L742
	jmp	SHORT $L739
$L742:
; Line 50
	mov	eax, DWORD PTR _i$[ebp]
	cmp	eax, DWORD PTR _j$[ebp]
	jl	SHORT $L743
; Line 51
	jmp	SHORT $L736
$L743:
; Line 53
	mov	ecx, DWORD PTR _array$[ebp]
	add	ecx, DWORD PTR _j$[ebp]
	push	ecx
	mov	edx, DWORD PTR _array$[ebp]
	add	edx, DWORD PTR _i$[ebp]
	push	edx
	call	_swapData
	add	esp, 8
; Line 54
	jmp	SHORT $L735
$L736:
; Line 55
	mov	eax, DWORD PTR _i$[ebp]
	sub	eax, 1
	push	eax
	mov	ecx, DWORD PTR _left$[ebp]
	push	ecx
	mov	edx, DWORD PTR _array$[ebp]
	push	edx
	call	_quickSortOnce
	add	esp, 12					; 0000000cH
; Line 56
	mov	eax, DWORD PTR _right$[ebp]
	push	eax
	mov	ecx, DWORD PTR _j$[ebp]
	add	ecx, 1
	push	ecx
	mov	edx, DWORD PTR _array$[ebp]
	push	edx
	call	_quickSortOnce
	add	esp, 12					; 0000000cH
$L733:
; Line 58
	mov	esp, ebp
	pop	ebp
	ret	0
_quickSortOnce ENDP
_TEXT	ENDS
PUBLIC	_QuickSort
_TEXT	SEGMENT
_infor$ = 8
_str_len$ = 12
_QuickSort PROC NEAR
; Line 60
	push	ebp
	mov	ebp, esp
; Line 61
	mov	eax, DWORD PTR _str_len$[ebp]
	sub	eax, 1
	push	eax
	push	0
	mov	ecx, DWORD PTR _infor$[ebp]
	push	ecx
	call	_quickSortOnce
	add	esp, 12					; 0000000cH
; Line 62
	pop	ebp
	ret	0
_QuickSort ENDP
_TEXT	ENDS
PUBLIC	_readFileInfor
EXTRN	_fread:NEAR
EXTRN	_printf:NEAR
EXTRN	_calloc:NEAR
_DATA	SEGMENT
	ORG $+2
$SG755	DB	'r', 00H
	ORG $+2
$SG758	DB	'%s file is not exist !', 0aH, 00H
_DATA	ENDS
_TEXT	SEGMENT
_fileName$ = 8
_fp$ = -4
_file_length$ = -12
_infor$ = -8
_readFileInfor PROC NEAR
; Line 64
	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
; Line 69
	push	OFFSET FLAT:$SG755
	mov	eax, DWORD PTR _fileName$[ebp]
	push	eax
	call	_fopen
	add	esp, 8
	mov	DWORD PTR _fp$[ebp], eax
; Line 70
	cmp	DWORD PTR _fp$[ebp], 0
	jne	SHORT $L757
; Line 71
	mov	ecx, DWORD PTR _fileName$[ebp]
	push	ecx
	push	OFFSET FLAT:$SG758
	call	_printf
	add	esp, 8
$L757:
; Line 73
	push	2
	push	0
	mov	edx, DWORD PTR _fp$[ebp]
	push	edx
	call	_fseek
	add	esp, 12					; 0000000cH
; Line 74
	mov	eax, DWORD PTR _fp$[ebp]
	push	eax
	call	_ftell
	add	esp, 4
	mov	DWORD PTR _file_length$[ebp], eax
; Line 75
	mov	ecx, DWORD PTR _file_length$[ebp]
	add	ecx, 1
	push	ecx
	push	1
	call	_calloc
	add	esp, 8
	mov	DWORD PTR _infor$[ebp], eax
; Line 76
	push	0
	push	0
	mov	edx, DWORD PTR _fp$[ebp]
	push	edx
	call	_fseek
	add	esp, 12					; 0000000cH
; Line 77
	mov	eax, DWORD PTR _fp$[ebp]
	push	eax
	mov	ecx, DWORD PTR _file_length$[ebp]
	push	ecx
	push	1
	mov	edx, DWORD PTR _infor$[ebp]
	push	edx
	call	_fread
	add	esp, 16					; 00000010H
; Line 78
	mov	eax, DWORD PTR _infor$[ebp]
	add	eax, DWORD PTR _file_length$[ebp]
	mov	BYTE PTR [eax], 0
; Line 80
	mov	ecx, DWORD PTR _fp$[ebp]
	push	ecx
	call	_fclose
	add	esp, 4
; Line 82
	mov	eax, DWORD PTR _infor$[ebp]
; Line 83
	mov	esp, ebp
	pop	ebp
	ret	0
_readFileInfor ENDP
_TEXT	ENDS
PUBLIC	_main
EXTRN	_puts:NEAR
EXTRN	_free:NEAR
_DATA	SEGMENT
$SG766	DB	'data.txt', 00H
	ORG $+3
$SG768	DB	'Result.txt', 00H
_DATA	ENDS
_TEXT	SEGMENT
_fileName$ = -4
_outFileName$ = -12
_fileInfor$ = -8
_file_length$ = -16
_main	PROC NEAR
; Line 85
	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
; Line 86
	mov	DWORD PTR _fileName$[ebp], OFFSET FLAT:$SG766
; Line 87
	mov	DWORD PTR _outFileName$[ebp], OFFSET FLAT:$SG768
; Line 88
	mov	DWORD PTR _fileInfor$[ebp], 0
; Line 89
	mov	DWORD PTR _file_length$[ebp], 0
; Line 91
	mov	eax, DWORD PTR _fileName$[ebp]
	push	eax
	call	_readFileInfor
	add	esp, 4
	mov	DWORD PTR _fileInfor$[ebp], eax
; Line 92
	mov	ecx, DWORD PTR _fileInfor$[ebp]
	push	ecx
	call	_puts
	add	esp, 4
; Line 93
	mov	edx, DWORD PTR _fileName$[ebp]
	push	edx
	call	_getFileLength
	add	esp, 4
	mov	DWORD PTR _file_length$[ebp], eax
; Line 94
	mov	eax, DWORD PTR _file_length$[ebp]
	push	eax
	mov	ecx, DWORD PTR _fileInfor$[ebp]
	push	ecx
	call	_QuickSort
	add	esp, 8
; Line 95
	mov	edx, DWORD PTR _fileInfor$[ebp]
	push	edx
	call	_puts
	add	esp, 4
; Line 96
	mov	eax, DWORD PTR _fileInfor$[ebp]
	push	eax
	mov	ecx, DWORD PTR _outFileName$[ebp]
	push	ecx
	call	_writeFile
	add	esp, 8
; Line 99
	mov	edx, DWORD PTR _fileInfor$[ebp]
	push	edx
	call	_free
	add	esp, 4
; Line 101
	xor	eax, eax
; Line 102
	mov	esp, ebp
	pop	ebp
	ret	0
_main	ENDP
_TEXT	ENDS
END
