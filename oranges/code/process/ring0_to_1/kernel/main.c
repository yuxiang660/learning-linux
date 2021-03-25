
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"


/*======================================================================*
                            kernel_main
 *======================================================================*/
PUBLIC int kernel_main()
{
	disp_str("-----\"kernel_main\" begins-----\n");

	PROCESS* p_proc	= proc_table;

	p_proc->ldt_sel	= SELECTOR_LDT_FIRST; // GDT中第5个选择子: 0x28/8 = 5，由protect.c::init_prot()函数添加
	memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS>>3], sizeof(DESCRIPTOR));  // 构建LDT第一个描述符
	p_proc->ldts[0].attr1 = DA_C | PRIVILEGE_TASK << 5;	// change the DPL
	memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS>>3], sizeof(DESCRIPTOR));  // 构建LDT第二个描述符
	p_proc->ldts[1].attr1 = DA_DRW | PRIVILEGE_TASK << 5;	// change the DPL

	p_proc->regs.cs	= (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK; // 指向LDT第一个描述符
	p_proc->regs.ds	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK; // 指向LDT第二个描述符
	p_proc->regs.es	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK; // 指向LDT第二个描述符
	p_proc->regs.fs	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK; // 指向LDT第二个描述符
	p_proc->regs.ss	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK; // 指向LDT第二个描述符
	p_proc->regs.gs	= (SELECTOR_KERNEL_GS & SA_RPL_MASK) | RPL_TASK; // 指向GDT中的显存，RPL发生改变
	p_proc->regs.eip= (u32)TestA;
	p_proc->regs.esp= (u32) task_stack + STACK_SIZE_TOTAL; // global.c中定义的单独栈
	p_proc->regs.eflags = 0x1202;	// IF=1, IOPL=1, bit 2 is always 1. 这样，进程就可以使用I/O指令，并且中断会在iretd执行时被打开

	p_proc_ready	= proc_table;
	restart();

	while(1){}
}

/*======================================================================*
                               TestA
 *======================================================================*/
void TestA()
{
	int i = 0;
	while(1){
		disp_str("A");
		disp_int(i++);
		disp_str(".");
		delay(1);
	}
}
