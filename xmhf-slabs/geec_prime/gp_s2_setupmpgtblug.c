/*
 * @XMHF_LICENSE_HEADER_START@
 *
 * eXtensible, Modular Hypervisor Framework (XMHF)
 * Copyright (c) 2009-2012 Carnegie Mellon University
 * Copyright (c) 2010-2012 VDG Inc.
 * All Rights Reserved.
 *
 * Developed by: XMHF Team
 *               Carnegie Mellon University / CyLab
 *               VDG Inc.
 *               http://xmhf.org
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in
 * the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the names of Carnegie Mellon or VDG Inc, nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * @XMHF_LICENSE_HEADER_END@
 */

#include <xmhf.h>
#include <xmhf-debug.h>

#include <xmhfgeec.h>

#include <geec_prime.h>
#include <uapi_slabmempgtbl.h>


//setup unverified guest (ug) slab memory page tables
//@ghost u64 gp_s2_setupmpgtblug_flags[1024*1024];
//@ghost u32 gp_s2_setupmpgtblug_memorytype[1024*1024];
//@ghost u64 gp_s2_setupmpgtblug_p_table_value[1024*1024];
/*@
	requires 0 <= slabid < XMHFGEEC_TOTAL_SLABS;
	assigns gp_s2_setupmpgtblug_flags[0..((1024*1024)-1)];
@*/
void gp_s2_setupmpgtblug(u32 slabid){
	u64 p_table_value;
	u64 flags;
	u32 spatype;
	u32 memorytype;
	u32 i;
	slab_params_t spl;

	spl.src_slabid = XMHFGEEC_SLAB_GEEC_PRIME;
	spl.dst_slabid = XMHFGEEC_SLAB_UAPI_SLABMEMPGTBL;
	spl.cpuid = 0; //XXX: fixme, need to plug in BSP cpuid
	spl.dst_uapifn = XMHFGEEC_UAPI_SLABMEMPGTBL_SETENTRYFORPADDR;

	/*@
		loop invariant d1: 0 <= i <= (1024*1024);
		//loop invariant d2: \forall integer x; 0 <= x < i ==> ();
		loop assigns i;
		loop assigns memorytype;
		loop assigns spatype;
		loop assigns flags;
		loop assigns gp_s2_setupmpgtblug_flags[0..((1024*1024)-1)];
		loop assigns p_table_value;
		loop assigns spl.in_out_params[0..4];
		loop variant (1024*1024) - i;
	@*/
	for(i=0; i < (1024*1024); i++){
		memorytype = gp_s2_setupmpgtblug_getmtype((u64)(i*PAGE_SIZE_4K));
		spatype = gp_s2_setupmpgtbl_getspatype(slabid, (u32)(i*PAGE_SIZE_4K));
		flags = gp_s2_setupmpgtblug_getflags(slabid, (u32)(i*PAGE_SIZE_4K), spatype);
		//@ghost gp_s2_setupmpgtblug_flags[i] = flags;


		p_table_value = (u64) ((i*PAGE_SIZE_4K))  | ((u64)memorytype << 3) |  flags ;	//present, UC

		spl.in_out_params[0] = slabid;
		spl.in_out_params[1] = (i*PAGE_SIZE_4K);
		spl.in_out_params[2] = 0;
                spl.in_out_params[3] = p_table_value;
                spl.in_out_params[4] = 0;
		XMHF_SLAB_CALLNEW(&spl);
	}
}



