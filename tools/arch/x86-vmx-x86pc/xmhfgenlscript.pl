#!/usr/bin/perl
# script to generate XMHF linker script with appropriate section definitions
# based on the slab names provided
# author: amit vasudevan (amitvasudevan@acm.org)

my $i = 0;

print "\n/* autogenerated XMHF linker script */";
print "\n/* author: amit vasudevan (amitvasudevan@acm.org) */";

print "\n#include <xmhf.h>";
print "\n";
print "\n";
print "\nOUTPUT_ARCH(\"i386\")";
print "\n";
print "\nENTRY(xmhf_runtime_entry)";
print "\nMEMORY";
print "\n{";
print "\n  all (rwxai) : ORIGIN = 0x02000000, LENGTH = 300M";
print "\n  unaccounted (rwxai) : ORIGIN = 0, LENGTH = 0 /* see section .unaccounted at end */";
print "\n}";
print "\nSECTIONS";
print "\n{";
print "\n	. = 0x10000000;";
print "\n";

print "\n	.xmhfhic : {";
print "\n		_xmhfhic_code_start = .;";
print "\n		_objs_xmhf-hic/xmhf-hic.slo(.hiccode)";
print "\n		. = ALIGN(0x200000);";
print "\n		_xmhfhic_code_end = .;";
print "\n		_xmhfhic_rwdata_start = .;";
print "\n		_xmhfhic_sharedro_start = .;";
print "\n		_xmhfhic_rodata_start = .;";
print "\n		_objs_xmhf-hic/xmhf-hic.slo(.hicdata)";
print "\n		. = ALIGN(0x200000);";
print "\n		_xmhfhic_rwdata_end = .;";
print "\n		_xmhfhic_sharedro_end = .;";
print "\n		_xmhfhic_rodata_end = .;";
print "\n		_xmhfhic_stack_start = .;";
print "\n		_objs_xmhf-hic/xmhf-hic.slo(.hicstack)";
print "\n		. = ALIGN(0x200000);";
print "\n		_xmhfhic_stack_end = .;";
print "\n		_xmhfhic_dmadata_start = .;";
print "\n		_objs_xmhf-hic/xmhf-hic.slo(.hicdmadata)";
print "\n		. = ALIGN(0x200000);";
print "\n		_xmhfhic_dmadata_end = .;";
print "\n		. = ALIGN(0x200000);";
print "\n	} >all=0x0000";


while( $i <= $#ARGV) {
print "\n	.slab_$ARGV[$i] : {";
print "\n		_slab_$ARGV[$i]_code_start = .;";
print "\n		_slab_$ARGV[$i]_entrypoint = .;";
print "\n		. = ALIGN(1);";
print "\n		_objs_slab_$ARGV[$i]/$ARGV[$i].slo(.slabentrystub)";
print "\n		_objs_slab_$ARGV[$i]/$ARGV[$i].slo(.slabcode)";
print "\n		. = ALIGN(0x200000);";
print "\n		_slab_$ARGV[$i]_code_end = .;";
print "\n		_slab_$ARGV[$i]_rwdata_start = .;";
print "\n		_objs_slab_$ARGV[$i]/$ARGV[$i].slo(.slabrwdata)";
print "\n		. = ALIGN(0x200000);";
print "\n		_slab_$ARGV[$i]_rwdata_end = .;";
print "\n		_slab_$ARGV[$i]_rodata_start = .;";
print "\n		_objs_slab_$ARGV[$i]/$ARGV[$i].slo(.slabrodata)";
print "\n		. = ALIGN(0x200000);";
print "\n		_slab_$ARGV[$i]_rodata_end = .;";
print "\n		_slab_$ARGV[$i]_stack_start = .;";
print "\n		_objs_slab_$ARGV[$i]/$ARGV[$i].slo(.slabstack)";
print "\n		. = ALIGN(0x200000);";
print "\n		_slab_$ARGV[$i]_stack_end = .;";
print "\n		_slab_$ARGV[$i]_dmadata_start = .;";
print "\n		_objs_slab_$ARGV[$i]/$ARGV[$i].slo(.slabdmadata)";
print "\n		. = ALIGN(0x200000);";
print "\n		_slab_$ARGV[$i]_dmadata_end = .;";
print "\n	} >all=0x0000";
print "\n";

	$i++;
}

print "\n	.libxmhfdebugdata : {";
print "\n		*(.libxmhfdebugdata)";
print "\n		*(.text)";
print "\n		*(.rodata)";
print "\n		*(.rodata.str1.1)";
print "\n		*(.data)";
print "\n		*(.bss)";
print "\n		*(.comment)";
print "\n		*(.eh_frame)";
print "\n	} >all=0x0000";
print "\n";
print "\n	/* this is to cause the link to fail if there is";
print "\n	* anything we didn't explicitly place.";
print "\n	* when this does cause link to fail, temporarily comment";
print "\n	* this part out to see what sections end up in the output";
print "\n	* which are not handled above, and handle them.";
print "\n	*/";
print "\n	.unaccounted : {";
print "\n	*(*)";
print "\n	} >unaccounted";
print "\n}";
print "\n";

exit 0;
