#
# - delpages.pl index.hhc
#
$buf = <STDIN>;	# discard first line
for ($buf = <STDIN> ; $buf !~ /\"pages\"/; $buf= <STDIN>){
	print "\t", $buf;
}
