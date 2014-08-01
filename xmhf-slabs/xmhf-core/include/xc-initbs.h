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

// XMHF core startup component declarations
// author: amit vasudevan (amitvasudevan@acm.org)

#ifndef __XMHF_STARTUP_H__
#define __XMHF_STARTUP_H__

#define	XMHF_SLAB_INITBS_FNXMHFRUNTIMEENTRY	0


#ifndef __ASSEMBLY__

#ifdef __XMHF_SLAB_CALLER_INDEX__ 

XMHF_SLAB_DEFIMPORTFN(void, xmhf_runtime_entry, (void), XMHF_SLAB_DEFIMPORTFNSTUB(__XMHF_SLAB_CALLER_INDEX__, XMHF_SLAB_INITBS_INDEX, XMHF_SLAB_INITBS_FNXMHFRUNTIMEENTRY, (0), 0, XMHF_SLAB_FN_RETTYPE_NORMAL))

#else 	//!__XMHF_SLAB_CALLER_INDEX__

void xmhf_runtime_entry(void);
void xmhf_apihub_arch_initialize(void);
#define xmhf_apihub_initialize	xmhf_apihub_arch_initialize
void xcinitbs_arch_initialize_exception_handling(void);

//initialize SMP
void xmhf_baseplatform_arch_smpinitialize(void);
#define xmhf_baseplatform_smpinitialize xmhf_baseplatform_arch_smpinitialize

//re-initialize DMA protections (if needed) for the runtime
bool xmhf_dmaprot_arch_reinitialize(void);
#define xmhf_dmaprot_reinitialize xmhf_dmaprot_arch_reinitialize


#endif	//__XMHF_SLAB_CALLER_INDEX__

#endif //__ASSEMBLY__



#endif //__XMHF_STARTUP_H__