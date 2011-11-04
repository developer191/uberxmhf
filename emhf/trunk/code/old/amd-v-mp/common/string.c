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
 * This file is part of the EMHF historical reference
 * codebase, and is released under the terms of the
 * GNU General Public License (GPL) version 2.
 * Please see the LICENSE file for details.
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

#include <target.h>

void *memmove(void *dst_void, const void *src_void, u32 length){
  char *dst = dst_void;
  const char *src = src_void;

  if (src < dst && dst < src + length){
      // Have to copy backwards 
      src += length;
      dst += length;
      while (length--){
	     *--dst = *--src;
	     }
  }else{
      while (length--){
	     *dst++ = *src++;
	    }
  }

  return dst_void;
}


char *strchr(const char *s, int c)
{
    long d0;
    register char *__res;
    __asm__ __volatile__ (
        "   mov  %%al,%%ah  \n"
        "1: lodsb           \n"
        "   cmp  %%ah,%%al  \n"
        "   je   2f         \n"
        "   test %%al,%%al  \n"
        "   jne  1b         \n"
        "   mov  $1,%1      \n"
        "2: mov  %1,%0      \n"
        "   dec  %0         \n"
        : "=a" (__res), "=&S" (d0) : "1" (s), "0" (c) );
    return __res;
}


u32 strnlen(const char * s, u32 count){
	const char *sc;

	for (sc = s; count-- && *sc != '\0'; ++sc);
	return (u32)(sc - s);
}


void *memcpy(void * to, const void * from, u32 n){
  int d0, d1, d2;

  __asm__ __volatile__(
  	"rep ; movsl\n\t"
  	"movl %4,%%ecx\n\t"
  	"andl $3,%%ecx\n\t"
#if 1	/* want to pay 2 byte penalty for a chance to skip microcoded rep? */
  	"jz 1f\n\t"
#endif
  	"rep ; movsb\n\t"
  	"1:"
  	: "=&c" (d0), "=&D" (d1), "=&S" (d2)
  	: "0" (n/4), "g" (n), "1" ((long) to), "2" ((long) from)
  	: "memory");
  return (to);
}

void *memset (void *str, int c, u32 len){
  register char *st = str;

  while (len-- > 0)
    *st++ = (char)c;
  return str;
}


u32 strncmp(u8 * cs, u8 * ct, u32 count)
{
        int res;
        int d0, d1, d2;
        __asm__ __volatile__( "1:\tdecl %3\n\t"
                "js 2f\n\t"
                "lodsb\n\t"
                "scasb\n\t"
                "jne 3f\n\t" 
                "testb %%al,%%al\n\t"
                "jne 1b\n"
                "2:\txorl %%eax,%%eax\n\t"
                "jmp 4f\n"
                "3:\tsbbl %%eax,%%eax\n\t"
                "orb $1,%%al\n"
                "4:"
                :"=a" (res), "=&S" (d0), "=&D" (d1), "=&c" (d2)
                :"1" (cs),"2" (ct),"3" (count)
                :"memory");
        return (u32)res;
}
