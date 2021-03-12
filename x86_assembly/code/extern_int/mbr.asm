         ;代码清单8-1
         ;文件名：c08_mbr.asm
         ;文件说明：硬盘主引导扇区代码（加载程序） 
         ;创建日期：2011-5-5 18:17
         
         app_lba_start equ 100           ;声明常数（用户程序起始逻辑扇区号）
                                         ;常数的声明不会占用汇编地址
                                    
SECTION mbr align=16 vstart=0x7c00       ;段内所有元素都将从0x7c00开始计算，简化了编码

         ;设置堆栈段和栈指针 
         mov ax,0      
         mov ss,ax
         mov sp,ax
      
         mov ax,[cs:phy_base]            ;计算用于加载用户程序的逻辑段地址，用CS的内容作为用户程序代码段，
         mov dx,[cs:phy_base+0x02]       ;phy_base保存32位的数，表示用户程序大小，高16位
         mov bx,16                       ;将DX:AX除以16，变成16位的段地址，并传送到DS和ES寄存器
         div bx            
         mov ds,ax                       ;令DS和ES指向该段以进行操作
         mov es,ax                        
    
         ;以下读取程序的起始部分 
         xor di,di
         mov si,app_lba_start            ;程序在硬盘上的起始逻辑扇区号 
         xor bx,bx                       ;加载到DS:0x0000处 
         call read_hard_disk_0
      
         ;以下判断整个程序有多大
         mov dx,[2]                      ;曾经把dx写成了ds，花了二十分钟排错 
         mov ax,[0]
         mov bx,512                      ;512字节每扇区
         div bx
         cmp dx,0                        ;DX是程序大小除以512的余数，AX是除以512的商，代表扇区数
         jnz @1                          ;未除尽，因此结果比实际扇区数少1，跳过AX减一，因为未除尽时，(AX+1)才是真正需要读的扇区数
         dec ax                          ;已经读了一个扇区，扇区总数减1 
   @1:
         cmp ax,0                        ;考虑实际长度小于等于512个字节的情况，此处AX等于零的情况只能是程序小于或等于512
         jz direct
         
         ;读取剩余的扇区
         push ds                         ;以下要用到并改变DS寄存器 

         mov cx,ax                       ;循环次数（剩余扇区数）
   @2:
         mov ax,ds
         add ax,0x20                     ;得到下一个以512字节为边界的段地址
         mov ds,ax                       ;512的十六进制是0x200，又移4位，正好是0x20
                              
         xor bx,bx                       ;每次读时，偏移地址始终为0x0000 
         inc si                          ;下一个逻辑扇区 
         call read_hard_disk_0
         loop @2                         ;循环读，直到读完整个功能程序 

         pop ds                          ;恢复数据段基址到用户程序头部段 
      
         ;计算入口点代码段基址 
   direct:
         mov dx,[0x08]                   ;用户程序入口点的代码段"section.code_1.start"高八位
         mov ax,[0x06]                   ;用户程序入口点的代码段"section.code_1.start"低八位
         call calc_segment_base          ;调用calc_segment_base之后，从DX:AX中的20位汇编地址，得到内存段基地址到AX中
         mov [0x06],ax                   ;回填修正后的入口点代码段基址 
      
         ;开始处理段重定位表
         mov cx,[0x0a]                   ;需要重定位的项目数量
         mov bx,0x0c                     ;重定位表首汇编地址偏移
          
 realloc:
         mov dx,[bx+0x02]                ;32位地址的高16位，code_1_segment汇编地址高16位
         mov ax,[bx]                     ;code_1_segment汇编地址低16位
         call calc_segment_base          ;调用calc_segment_base之后，从DX:AX中的20位汇编地址，得到内存段基地址到AX中
         mov [bx],ax                     ;回填段的基址
         add bx,4                        ;下一个重定位项（每项占4个字节），填回之后就低16位有效了，代表偏移地址
         loop realloc                    ;循环直至CX到零，重定位了所有需要重定位的段
      
         jmp far [0x04]                  ;转移到用户程序入口点start
                                         ;间接绝对远调用"jmp far [0x04]"，会访问段寄存器DS所指向的数据段，从偏移地址位0x04的地方取出两个字(已经被重定位)，分别传送到代码段寄存器CS和指令寄存器IP
;-------------------------------------------------------------------------------
read_hard_disk_0:                        ;从硬盘读取一个逻辑扇区
                                         ;输入：DI:SI=起始逻辑扇区号
                                         ;      DS:BX=目标缓冲区地址
         push ax                         ;保护现场AX,BX,CX,DX，在read_hard_disk_0调用结束后恢复
         push bx
         push cx
         push dx
      
         mov dx,0x1f2
         mov al,1
         out dx,al                       ;读取的扇区数，读取1个扇区

         inc dx                          ;0x1f3
         mov ax,si                       ;di:si是要读取的扇区号，在调用read_hard_disk_0前已经配置好
         out dx,al                       ;LBA地址7~0

         inc dx                          ;0x1f4
         mov al,ah
         out dx,al                       ;LBA地址15~8

         inc dx                          ;0x1f5
         mov ax,di
         out dx,al                       ;LBA地址23~16

         inc dx                          ;0x1f6
         mov al,0xe0                     ;LBA28模式，主盘
         or al,ah                        ;LBA地址27~24
         out dx,al

         inc dx                          ;0x1f7
         mov al,0x20                     ;读命令
         out dx,al

  .waits:
         in al,dx
         and al,0x88
         cmp al,0x08
         jnz .waits                      ;不忙，且硬盘已准备好数据传输 

         mov cx,256                      ;总共要读取的字数
         mov dx,0x1f0
  .readw:
         in ax,dx
         mov [bx],ax
         add bx,2
         loop .readw

         pop dx
         pop cx
         pop bx
         pop ax
      
         ret                             ;ret/call和retf/call far，ret只从堆栈中弹出一个字到IP寄存器中，retf还会弹出另一个字到CS中

;-------------------------------------------------------------------------------
calc_segment_base:                       ;计算16位段地址
                                         ;输入：DX:AX=32位物理地址
                                         ;返回：AX=16位段基地址 
         push dx                          
         
         add ax,[cs:phy_base]            ;AX原本是需要转换的段的低16位汇编地址，加上内存中物理起始地址0x10000，得到内存中段的地址低16位
         adc dx,[cs:phy_base+0x02]       ;adc是带进位加法，加上上一步的CF标志，得到内存中段的地址高16位
         shr ax,4                        ;右移，去除低4位
         ror dx,4                        ;循环右移，取低4位到高4位，用于拼接AX
         and dx,0xf000
         or ax,dx                        ;经过上述操作，目的是使AX存下20位地址中的高16位，即段基地址
         
         pop dx
         
         ret

;-------------------------------------------------------------------------------
         phy_base dd 0x10000             ;用户程序被加载的物理起始地址
         
 times 510-($-$$) db 0
                  db 0x55,0xaa