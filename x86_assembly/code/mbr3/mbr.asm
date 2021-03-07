         ;代码清单7-1
         ;文件名：c07_mbr.asm
         ;文件说明：硬盘主引导扇区代码
         ;创建日期：2011-4-13 18:02
         
         jmp near start
	
 message db '1+2+3+...+100='    ;编译器会字符串拆开，形成一个个单独的字节
        
 start:
         mov ax,0x7c0           ;设置数据段的段基地址 
         mov ds,ax

         mov ax,0xb800          ;设置附加段基址到显示缓冲区
         mov es,ax

         ;以下显示字符串 
         mov si,message          
         mov di,0
         mov cx,start-message   ;计算@g的循环次数
     @g:
         mov al,[si]            ;从DS:SI中取出一个字节，messge位置
         mov [es:di],al         ;往ES:DI中写入一个字节，显示缓冲区
         inc di
         mov byte [es:di],0x07  ;黑底白字
         inc di                 ;下一个目标位置
         inc si                 ;下一个源位置
         loop @g                ;循环直至CX为0

         ;以下计算1到100的和 
         xor ax,ax              ;清零AX，用于存储每步的计算结果
         mov cx,1               ;需要累加的数
     @f:
         add ax,cx
         inc cx
         cmp cx,100
         jle @f                 ;不大于less or equal, CX大于100，结束循环，累加结果已经存在AX中

         ;以下将AX中的结果各个位数分解出来，并每一位放入堆中
         xor cx,cx              ;设置堆栈段的段基地址
         mov ss,cx              ;初始化段寄存器SS为0x0000
         mov sp,cx              ;初始化堆栈指针SP为0x0000

         mov bx,10              ;BX是除数
         xor cx,cx              ;CX用于累计有多少个数位
     @d:
         inc cx
         xor dx,dx
         div bx                 ;DX:AX是被除数，BX是除数，DX存余数，AX存商
         or dl,0x30             ;因为余数8位就能存了，所以用低位DL就可用了，转换成数字ASCII
         push dx                ;把余数送入堆，当push指令第一次执行时，SP的内容减2，即0x0000-0x0002=0xFFFE，于实数据被压入0000:FFFF的高位。以后每次push，SP都要依次减2
         cmp ax,0               ;商是否为零
         jne @d                 ;如果商不是零，继续循环

         ;以下从堆中pop出每一位，显示在屏幕上，CX中存在显示数据的位数
     @a:
         pop dx                 ;从SS:SP处弹处一个字到寄存器DX中，并将SP的内容加上操作数的字长2
         mov [es:di],dl
         inc di
         mov byte [es:di],0x07
         inc di
         loop @a                ;每次循环CX减一，直到CX到零时，结束循环

         jmp near $ 
       

times 510-($-$$) db 0
                 db 0x55,0xaa