
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                              klib.asm
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                                                       Forrest Yu, 2005
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


[SECTION .data]

disp_pos			dd	0

[SECTION .text]

; 导出函数
global	disp_str

; ========================================================================
;                  void disp_str(char * pszInfo);
; ========================================================================
disp_str:
	push	ebp
	mov	ebp, esp

	mov	esi, [ebp + 8]	; pszInfo
	mov	edi, [disp_pos]
	mov	ah, 0Fh
.1:
	lodsb				; DS:(E)SI into AL
	test	al, al	; 是零
	jz	.2
	cmp	al, 0Ah	; 是回车吗?
	jnz	.3
	push	eax
	push	ebx		;必须用ebx做除数，原来是用bl，是无法存下160的
	mov	eax, edi
	xor	edi, edi
	mov	ebx, 160
	div	ebx
	and	eax, 0FFh;得到当前行号，160 = 80 * 2
	inc	eax		;行数加1
	mov	ebx, 160
	mul	ebx
	mov	edi, eax
	pop	ebx
	pop	eax
	jmp	.1
.3:
	mov	[gs:edi], ax	;gs指向了显存地址映射0xB8000
	add	edi, 2
	jmp	.1

.2:
	mov	[disp_pos], edi

	pop	ebp
	ret

