         ;代码清单11-1
         ;文件名：c11_mbr.asm
         ;文件说明：硬盘主引导扇区代码 
         ;创建日期：2011-5-16 19:54

         ;设置堆栈段和栈指针 
         mov ax,cs      
         mov ss,ax
         mov sp,0x7c00
      
         ;计算GDT所在的逻辑段地址 
         mov ax,[cs:gdt_base+0x7c00]        ;低16位，因为主引导程序的实际加载位置是逻辑地址0x0000:0x7c00，而ds没有被配置，故标号bdt_base处的品阿姨地址是gdt_base+0.7c00
         mov dx,[cs:gdt_base+0x7c00+0x02]   ;高16位，加上2bytes偏移，因为此时还在实模式下，需要将线性地址转换成段地址和偏移地址
         mov bx,16        
         div bx            
         mov ds,ax                          ;令DS指向该段以进行操作，此时ds被赋值为0x07e0，20位有效地址0x07e00地址被除以16的商
         mov bx,dx                          ;段内起始偏移地址，此时bx被赋值为0x0000，20位有效地址0x07e00被除以16的余数
      
         ;创建0#描述符，它是空描述符，这是处理器的要求，此时DS::BX指向GDT起始地址，一个描述符64位
         mov dword [bx+0x00],0x00
         mov dword [bx+0x04],0x00  

         ;创建#1描述符，保护模式下的代码段描述符，结合段描述符结构，分析下面的64位数字，可知此段线性基地址为0x00007C00，长度为512(0x001FF)代码段，特权级别为0
         mov dword [bx+0x08],0x7c0001ff     ;低32位
         mov dword [bx+0x0c],0x00409800     ;高32位

         ;创建#2描述符，保护模式下的数据段描述符（文本模式下的显示缓冲区），线性基地址为0x000B8000，长度为64KB(0x0FFFF)数据段，特权级别为0。用这个段来显示字符，0x000B8000是显存的起始地址
         mov dword [bx+0x10],0x8000ffff     ;低32位，0~15：0x8000
         mov dword [bx+0x14],0x0040920b     ;高32位，16~23：0x0b

         ;创建#3描述符，保护模式下的堆栈段描述符，线性基地址为0x00000000，段界限为0x07A00
         mov dword [bx+0x18],0x00007a00
         mov dword [bx+0x1c],0x00409600

         ;初始化描述符表寄存器GDTR
         mov word [cs: gdt_size+0x7c00],31  ;描述符表的界限（总字节数减一），4*8字节-1，修改gdt_size标号位置的内容为31
                                             
         lgdt [cs: gdt_size+0x7c00]         ;把从标号gdt_size开始的6字节加载到GDTR寄存器(48位)，gdt_size标号后面紧跟标号gdt_base，其内容是GDT32位地址0x00007e00
      
         in al,0x92                         ;南桥芯片内的端口 
         or al,0000_0010B                   ;端口0x92的位1用于控制A20，置1打开A20
         out 0x92,al                        ;打开A20

         cli                                ;保护模式下中断机制尚未建立，应 
                                            ;禁止中断，保护模式下BIOS的中断都不能用了
         mov eax,cr0
         or eax,1
         mov cr0,eax                        ;设置PE位
      
         ;以下进入保护模式... ...
         jmp dword 0x0008:flush             ;16位的描述符选择子：32位偏移，选择#1号代码段，用段超越前缀定位。当指令执行时，0x0008对应的GDT#2号的基地址是0x00007C00，正好是当前程序在内存中的区域，因此能定位到flush。0x0008加载到CS选择器，并从GDT中取出对应的描述符，加载CS描述符高速缓存器。
                                            ;清流水线并串行化处理器 
         [bits 32] 

    flush:
         mov cx,00000000000_10_000B         ;加载数据段选择子(0x10)，因为上面四个段描述符，#2号是数据段，RPL是00
         mov ds,cx                          ;选择子0x10加载到段选择器DS，并自动加载描述符高速缓存器

         ;以下在屏幕上显示"Protect mode OK." 
         mov byte [0x00],'P'  
         mov byte [0x02],'r'
         mov byte [0x04],'o'
         mov byte [0x06],'t'
         mov byte [0x08],'e'
         mov byte [0x0a],'c'
         mov byte [0x0c],'t'
         mov byte [0x0e],' '
         mov byte [0x10],'m'
         mov byte [0x12],'o'
         mov byte [0x14],'d'
         mov byte [0x16],'e'
         mov byte [0x18],' '
         mov byte [0x1a],'O'
         mov byte [0x1c],'K'

         ;以下用简单的示例来帮助阐述32位保护模式下的堆栈操作 
         mov cx,00000000000_11_000B         ;加载堆栈段选择子
         mov ss,cx
         mov esp,0x7c00

         mov ebp,esp                        ;保存堆栈指针 
         push byte '.'                      ;压入立即数（字节）
         
         sub ebp,4
         cmp ebp,esp                        ;判断压入立即数时，ESP是否减4 
         jnz ghalt                          
         pop eax
         mov [0x1e],al                      ;显示句点 
      
  ghalt:     
         hlt                                ;已经禁止中断，将不会被唤醒 

;-------------------------------------------------------------------------------
     
         gdt_size         dw 0
         gdt_base         dd 0x00007e00     ;GDT的物理地址，主引导扇区共512(0x200)字节，所以0x7c00+0x200=0x7e00
                             
         times 510-($-$$) db 0
                          db 0x55,0xaa