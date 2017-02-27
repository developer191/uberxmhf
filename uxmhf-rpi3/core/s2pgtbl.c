/*
	ARM 8 stage-2 page table translation functions

	author: amit vasudevan (amitvasudevan@acm.org)
*/

#include <types.h>
#include <arm8-32.h>
#include <bcm2837.h>
#include <miniuart.h>
#include <debug.h>

/* setup CPU to support stage-2 table translation */
void s2pgtbl_initialize(void){
	u32 vtcr, hdcr;

	vtcr = sysreg_read_vtcr();
	bcm2837_miniuart_puts("VTCR before= ");
	debug_hexdumpu32(vtcr);

	vtcr = 0;
	vtcr |= VTCR_RES1_MASK;	//reserved 1 bits
	vtcr |= ((0 << VTCR_T0SZ_SHIFT) & VTCR_T0SZ_MASK);	//T0SZ=0; 32 bits physical address
	vtcr |= ((0 << VTCR_S_SHIFT) & VTCR_S_MASK);		//S=0
	vtcr |= ((1 << VTCR_SL0_SHIFT) & VTCR_SL0_MASK);	//SL0=1; 3-level page table
	vtcr |= ((MEM_WRITEBACK_READALLOCATE_WRITEALLOCATE << VTCR_IRGN0_SHIFT) & VTCR_IRGN0_MASK);	//L1 cache attribute
	vtcr |= ((MEM_WRITEBACK_READALLOCATE_WRITEALLOCATE << VTCR_ORGN0_SHIFT) & VTCR_ORGN0_MASK);	//L2 cache attribute
	vtcr |= ((MEM_OUTER_SHAREABLE << VTCR_SH0_SHIFT) & VTCR_SH0_MASK);	//shareability attribute

	sysreg_write_vtcr(vtcr);

	vtcr = sysreg_read_vtcr();
	bcm2837_miniuart_puts("VTCR after= ");
	debug_hexdumpu32(vtcr);

	hdcr = sysreg_read_hdcr();
	bcm2837_miniuart_puts("HDCR before= ");
	debug_hexdumpu32(hdcr);

	hdcr &= HDCR_HPMN_MASK;
	sysreg_write_hdcr(hdcr);

	hdcr = sysreg_read_hdcr();
	bcm2837_miniuart_puts("HDCR after= ");
	debug_hexdumpu32(hdcr);

}
