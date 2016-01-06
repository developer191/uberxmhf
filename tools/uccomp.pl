#!/usr/bin/perl
# script to generate composition check files
# author: amit vasudevan (amitvasudevan@acm.org)

use Tie::File;
use File::Basename;
use Data::Dumper;

use lib dirname (__FILE__);
use upmf;	#load up the manifest parsing module


# command line inputs
my $g_slabsfile = $ARGV[0];
my $g_memoffsets = $ARGV[1];
my $g_ccompdriverfile = $ARGV[2];
my $g_ccompcheckfile = $ARGV[3];

$g_maxincldevlistentries = $ARGV[4];
$g_maxexcldevlistentries = $ARGV[5];
$g_maxmemoffsetentries = $ARGV[6];


my $g_rootdir;





$g_rootdir = dirname($g_slabsfile)."/";

#print "slabsfile:", $g_slabsfile, "\n";
#print "rootdir:", $g_rootdir, "\n";

print "parsing slab manifests...\n";
upmf_init($g_slabsfile, $g_memoffsets, $g_rootdir);
print "slab manifests parsed\n";



my $i = 0;
my $uapi_key;
my $uapi_fndef;



my $fh_ccompcheckfile;


open($fh_ccompcheckfile, '>', $g_ccompcheckfile) or die "Could not open file '$g_ccompcheckfile' $!";
print $fh_ccompcheckfile "\n/* autogenerated XMHF composition check file */";
print $fh_ccompcheckfile "\n/* author: amit vasudevan (amitvasudevan@acm.org) */";
print $fh_ccompcheckfile "\n\n";
print $fh_ccompcheckfile "#include <xmhf.h>\r\n";
print $fh_ccompcheckfile "#include <xmhf-debug.h>\r\n";
print $fh_ccompcheckfile "#include <xmhfgeec.h>\r\n";
print $fh_ccompcheckfile "\n\n";


#plug in header files
while($i < $g_totalslabs){
    print $fh_ccompcheckfile "#include <".$slab_idtoname{$i}.".h>\r\n";
    $i=$i+1;
}

print $fh_ccompcheckfile "\n\n";


# iterate over the uapi_fndef hashtable
while ( ($uapi_key, $uapi_fndef) = each %uapi_fndef )
{
	# fore each uapi_key we find, write function definition followed by assertions into check file
        print $fh_ccompcheckfile "$uapi_fndef{$uapi_key} { \r\n";
        print $fh_ccompcheckfile "$uapi_fnccomppre{$uapi_key} \r\n";
        print $fh_ccompcheckfile "$uapi_fnccompasserts{$uapi_key} \r\n";
        print $fh_ccompcheckfile "} \r\n\r\n";

	# for each uapi key we find, write function driver code into driver file
}

close $fh_ccompcheckfile;





exit 0;
















