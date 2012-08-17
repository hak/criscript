#
# PdxFilter.pl
# Preprocesses the CRI documentation format in a file for doxygen.
#
if ($#ARGV < 2) {
	print "Converts one language to doxygen format and filters others from a source file.";
	die "usage: perl doxyfilter.pl [language-to-keep] [language-to-delete-1] [language-to-delete-2] ... [source-file]\n";
}

# This line changes the read to input the entire source file into the variable
# at one time.
undef $/;

open SOURCEFILE, "< $ARGV[$#ARGV]" or die "Can't open sourcefile $ARGV[$#ARGV]\n";
$_ = <SOURCEFILE>;
close SOURCEFILE;

# Now we're ready to do any regex changes to the file.

s/
	\/\*		(?# Match against the forward slash and asterisk	)
	$ARGV[0]	(?# Match the first language string					)
 /\/\*!/gsx;	# replace Globally, treat file as Single line, use Extended comments

# For each remaining language, delete the comment field.

$i = 0;
while (++$i < $#ARGV) {
	s/
		\/\*		(?# Match against the forward slash and asterisk	)	
		$ARGV[$i]	(?# Match the Ith language string					)
		.*?			(?# Match any number of characters across newlines	)
		\*\/		(?# Match asterisk and forward slash				)
	//gsx;
}


# Print the entire file and exit

#
# ============= TABLE FILTER ==================
#
#  This block  generates <table> tag for html.
#  A table definition is an extent between
#  key string "\table" and "\endtable" and both
#  of them should start from 3rd column of a line.
#
# \table "This is 1st table title string" boarder="0"
# { header 1 | header 2   | header3     | }
# [ row 1    | row1 data1 | row1 data 2 | ]
# [ row 2    | row2 data1 | row2 data 2 | ]
# [ row 3    | row3 data1 | row3 data 2 | ]
# \endtable
#
# \table "これは日本語の表" boarder="1"
# { 見出し １ | 見出し　２   |見出し３     | }
# [ 行  １  | これはデータ１ | row1 data 2 日英混在 | ]
# [ 行 の ２    | なんちゃら | かんちゃら | ]
# [ 第 ３ 行    | あっと驚く | ため五郎 | ]
# \endtable
#

@lines=split(/\n/, $_);

@out=();
$table_mode = 0;

foreach $ln (@lines) {
	if ($table_mode == 1) { # TABLE 処理中
		if ( $ln =~ m%^[^\\]*\\endtable$%i ) {	# ENDTABLEが来たので	
			$table_mode = 0;					# TABLE処理中モード OFF
			put_table();						# そしてテーブルコードを出力
			push (@out, "</TABLE>");
			push (@out, "\\endhtmlonly");
		} else {								# それ以外は
			push (@tbuf, $ln);					# TABLE処理中なのでバッファに溜める
		}
	} else {				# TABLE処理中ではない
		if ($ln =~ m%^[^\\]*\\table(.*)$%i ) {
								#行頭から￥以外の文字が0個以上あって
			$tmp = $1;			#"\table"文字列が見つかったら。
			$caption="";
			$param ="";
			$table_mode = 1;	#"\teble"文字列が見つかったら。
			@tbuf = ();

			if ($tmp =~ /\"(.+?)\"\s*(.*?)$/ ) {
				$caption = $1;
				$param = $2;
			}
			push (@out, "\\htmlonly");
			push (@out, "<TABLE $param>");
			if ($caption ne "") {
#				push(@out, " <CAPTION CLASS=\"tblCapt\"><U>$caption</U><\/CAPTION>");
				push(@out, " <CAPTION ><STRONG>$caption</STRONG><\/CAPTION>");
			}

			$valign = "valign=middle";			# 後で各行単位でvalignをセットするため
			if ($param =~ /valign=(\w+?)/i) {
				$valign = "valign=$1";
			}
		} else {	# TABLE 処理が始まらないので
			push (@out, $ln);
		}
	}
}


#
# ============= END OF TABLE FILTER ==================
#

$_ = join("\n", @out);


#-----------------------------------------
@inbuf=split(/\n/, $_);
$tree_mode = 0;

@out=();

foreach $ln (@inbuf) {
	if ($ln =~ m%^[^\\]*\\tree(.*)$%i ) {
							#行頭から￥以外の文字が0個以上あって
		$tmp = $1;			#"\tree"文字列が見つかったら。
		$caption="";
		$param ="";
		if ($tmp =~ /\"(.+?)\"\s*(.*?)$/ ) {
			$caption = $1;
			$param = $2;
		}
		$tree_mode = 1;
		$width = "width=30%";
		if ($param =~ /width=([0-9]+%)/i) {
			$width = "width=$1";
		}
		@lines = ();
		push (@out, "\\htmlonly");
		push (@out, "<TABLE width=100% border=0 cellpadding=0 $param>");
		push (@out, "<CAPTION CLASS=\"treeCapt\"><U>$caption</U><\/CAPTION>");
	} elsif ($ln =~ m%^[^\\]*\\endtree(.*)$%i ) {
#		print (DEBUG "endtree found\n");	
		$tree_mode = 0;
		tree_process();
		push (@out, "</TABLE>");
		push (@out, "\\endhtmlonly");
	} elsif ($tree_mode == 0) {
		push (@out, $ln);
	} else {
		push (@lines, $ln);
	}
}

$_ = join("\n", @out);

print $_;

print $ARGV[$i];


exit;

#
# ===== put_table : put table codes between <TABLE> and </TABLE>
#
sub put_table{
	my ($ln);
	my ($multiple_lines); #複数行テーブル継続中(=1)フラグ
	my ($i, @cols, $ncols);

$multiple_lines = 0;

foreach $ln (@tbuf) {
	if ($multiple_lines == 1) {
		if ( $ln =~ /^\s*\[(.+)\]/ ) {
			foreach $col (@columns) {
				$col =~ s/^\s*$/&nbsp;/;
				push (@out, "<TD> $col </TD>");
			}
			push (@out, "</TR>");
			$multiple_lines = 0;

			push (@out, "<TR class=\"tblItem\" $valign>");
			@columns = split(/\s\|/, $1);
			foreach $col (@columns) {
				$col =~ s/^\s*$/&nbsp;/;
				push (@out, "<TD> $col </TD>");
			}
			push (@out, "</TR>");

		} elsif ( $ln =~ /^\s*\{(.+)\}/ ) {
			foreach $col (@columns) {
				$col =~ s/^\s*$/&nbsp;/;
				push (@out, "<TD> $col </TD>");
			}
			push (@out, "</TR>");
			$multiple_lines = 0;

			push (@out, "<TR class=\"tblHead\">");
			@columns = split(/\s\|/, $1);
			foreach $col (@columns) {
				$col =~ s/^\s*$/&nbsp;/;
				push (@out, "<TH> $col </TH>");
			}
			push (@out, "</TR>");

		} elsif ( $ln =~ /^\s*\|(.+)\|/ ) {	# 複数行テーブルの行か？
			@cols = split(/\s\|/, $1);
			$ncols = @cols;
			for ($i=0; $i<$ncols; $i++) {
				$cols[$i] =~ s/^\s*$/&nbsp;/;
				$columns[$i] = $columns[$i] . $cols[$i];
			}


		} else {
			foreach $col (@columns) {
				$col =~ s/^\s*$/&nbsp;/;
				push (@out, "<TD> $col </TD>");
			}
			push (@out, "</TR>");
			$multiple_lines = 0;
		}
	} else { #複数行テーブル処理中でない時、

		if ( $ln =~ /^\s*\[(.+)\]/ ) {
			push (@out, "<TR class=\"tblItem\" $valign>");
			@columns = split(/\s\|/, $1);
			foreach $col (@columns) {
				$col =~ s/^\s*$/&nbsp;/;
				push (@out, "<TD> $col </TD>");
			}
			push (@out, "</TR>");

		} elsif ( $ln =~ /^\s*\{(.+)\}/ ) {
			push (@out, "<TR class=\"tblHead\">");
			@columns = split(/\s\|/, $1);
			foreach $col (@columns) {
				$col =~ s/^\s*$/&nbsp;/;
				push (@out, "<TH> $col </TH>");
			}
			push (@out, "</TR>");

		} elsif ( $ln =~ /^\s*\|(.+)\|/ ) {	# 複数行テーブルの行か？
			@columns = split(/\s\|/, $1);
			$multiple_lines = 1;
			push (@out, "<TR class=\"tblItem\" $valign>");

		}
	}
}

if ($multiple_lines == 1) { #複数行TABLE処理未クローズ？
	foreach $col (@columns) {
		$col =~ s/^\s*$/&nbsp;/;
		push (@out, "<TD> $col </TD>");
	}
	push (@out, "</TR>");
	$multiple_lines = 0;
}

}
#
# =====  tree_process
#
sub tree_process{

my ($ln);

#foreach $ln (@lines) {
#	print (DEBUG "*** $ln\n");
#}


conv_tab_to_struct();	#input = @lines, output = @buf
#
# --- at this point, a hierarchy is in @buf
#
struct_to_tables();		#make tables from @buf

put_tree();

}


sub conv_tab_to_struct {
#
# conv_tab_to_struct analyze TAB leveled hierarchy
# and insert open(<UL>) and close (</UL>) tag pairs
# between entries which have n-TABs at the begining of line.
#
# reg-expr for n-TABs at the beginning of a line.
# These reg-expr match [TAB...]"title-string" "file-name-string"
# 
	my ($ln);

@ntabexpr_org = (
	"^(.+?)\\s+?(.+?)\$",								# 0-TAB,
	"^\\t(.+?)\\s+?(.+?)\$",								# 1-TAB
	"^\\t\\t(.+?)\\s+?(.+?)\$",							# 2
	"^\\t\\t\\t(.+?)\\s+?(.+?)\$",						# 3
	"^\\t\\t\\t\\t(.+?)\\s+?(.+?)\$",					# 4
	"^\\t\\t\\t\\t\\t(.+?)\\s+?(.+?)\$",					# 5
	"^\\t\\t\\t\\t\\t\\t(.+?)\\s+?(.+?)\$",				# 6
	"^\\t\\t\\t\\t\\t\\t\\t(.+?)\\s+?(.+?)\$",			# 7
	"^\\t\\t\\t\\t\\t\\t\\t\\t(.+?)\\s+?(.+?)\$",		# 8
	"^\\t\\t\\t\\t\\t\\t\\t\\t\\t(.+?)\\s+?(.+?)\$");	# 9
@ntabexpr = (
	"^(.+?)((?:\$)|(?:\\s+(.*)\$))",								# 0-TAB,
	"^\\t(.+?)((?:\$)|(?:\\s+(.*)\$))",								# 1-TAB
	"^\\t\\t(.+?)((?:\$)|(?:\\s+(.*)\$))",							# 2
	"^\\t\\t\\t(.+?)((?:\$)|(?:\\s+(.*)\$))",						# 3
	"^\\t\\t\\t\\t(.+?)((?:\$)|(?:\\s+(.*)\$))",					# 4
	"^\\t\\t\\t\\t\\t(.+?)((?:\$)|(?:\\s+(.*)\$))",					# 5
	"^\\t\\t\\t\\t\\t\\t(.+?)((?:\$)|(?:\\s+(.*)\$))",				# 6
	"^\\t\\t\\t\\t\\t\\t\\t(.+?)((?:\$)|(?:\\s+(.*)\$))",			# 7
	"^\\t\\t\\t\\t\\t\\t\\t\\t(.+?)((?:\$)|(?:\\s+(.*)\$))",		# 8
	"^\\t\\t\\t\\t\\t\\t\\t\\t\\t(.+?)((?:\$)|(?:\\s+(.*)\$))");	# 9
#
	@buf=();
#

	$level=0;
	
	foreach $ln (@lines) {
		for ($ntab=0; $ntab < $#ntabexpr; $ntab++) {	# chech if the line has TABs
			if ($ln =~ /$ntabexpr[$ntab]/) {			# at the start of line ?
				$node = $1;
				$desc = $2;
				$rank = $ntab;
				break;
			}
		}
	
		if ( $level > $rank) { 					# if a lower rank entry appears
			for (; $level > $rank; $level--) {	# then close higher level brackets
				push( @buf, "</UL>");
			}
		} elsif ($level < $rank) {				#if a higher rank entry appears
			for (; $level< $rank; $level++) {	# then open new brackets
				push( @buf, "<UL>");					# only one level higher
			}
		}
		if ($rank >= 0) {
			push(@buf, "<OBJECT> <param name=\"Local\" value=\"$node\"> <param name=\"Name\" value=\"$desc\"> </OBJECT>");
		}
	}
	
	for (; $level > 0; $level--) {
		push(@buf, "</UL>");
	}
	
	
#	foreach $ln (@buf) {
#		print (DEBUG "$ln\n");
#	}
}

sub struct_to_tables{
#
# struct_to_tables configurates tables from <UL> <OBJCET> ... </OBJECT> </UL>
# here, <UL></UL> pair indicate includion of children.
#
# for example,
# <UL>
#    a
#    <UL>
#       b
#    </UL> 
#    <UL>
#       c
#    </UL> 
# </UL>
#
# shows that a parent 'a' has two children 'b' and 'c'.
#
#
# --- at this point, a hierarchy is in @buf
#

@pgtbl = (); # page table
@titbl = (); # title table
@fntbl = (); # file name table
@intbl = (); # image number table
%tihash = ();
%fnhash = ();
@children = ();
@parent = ();

$ulopn = "\<UL\>";
$ulcls = "\</UL\>";
$object = "\<OBJECT(.|\n)+?\<\/OBJECT\>"; # (.|\n) is necessary to match across lines

$level = 0;
$pgidx = 0;
$title="";
$fname="";
@curparent=();
$curparent[$level] = 0;
$curchildren[$level]= "";

$buffer = join(/\n/, @buf);

	while ( $buffer =~ /($ulopn)|($ulcls)|($object)/ig ) {
		if ($1 ne "") {			# <ul> found
			$level++;
			$curchildren[$level]= "";
			$curparent[$level] = $pgidx-1;
		} elsif ($2 ne "") {	# </ul> found
			$tmp = $curchildren[$level]; # get children id list
			@tmplist = split(/;/, $tmp); # discard the first NUL element
			$tmp = shift(@tmplist);
			$tmp = join(";", @tmplist);
			$children[$curparent[$level]] = $tmp;
			$level--;
		} else { # <object> ... </object>
			$pgtbl[$pgidx] = $3; # whole string for <object> ... </object>
			$tmp = $3;
			$title = "";
			$fname = "";
			$imgnum = "";
			if ( $tmp =~ m%param\s*name="Local"\s*value="(.+?)"%i) {
					$fname = $1;
			}
			if ( $tmp =~ m%param\s*name="Name"\s*value="(.+?)"%i) {
					$title = $1;
			}
			if ( $tmp =~ m%param\s*name="ImageNumber"\s*value="(.+?)"%i) {
					$imgnum = $1;
			}
			$titbl[$pgidx] = $title;
			$fntbl[$pgidx] = $fname;
			$intbl[$pgidx] = $imgnum;
			$parent[$pgidx] = $curparent[$level];
			$children[$pgidx] = "";
			$curchildren[$level] = $curchildren[$level] . ";" . $pgidx;
			$pgidx++;
		}
	}
	
#	for($i=0; $i<$pgidx; $i++) {
#		print (DEBUG "$i : $fntbl[$i] P=$parent[$i] C=$children[$i]\n");
#	}
	
	$nspan = $pgidx+1;
}


sub put_tree{

	$level=0;
	push(@levelbar, "　　");

	push (@out,  "<tr valign=top>");
	push (@out,  "<td width=5% rowspan=$nspan></td>");
	push (@out,  "<td $width rowspan=$nspan valign=top>");

	push (@out,  "<div class=\"directory\">");

#
	push (@out,  "<div style=\"display: block;\">");
#
	put_branch($levelbar[0]);
	put_directory($fntbl[0]);
	prt_child(0);
#
	push (@out,  "</td>");
#
	prt_comment();

}



sub prt_child {
	my ($idx) = @_;
	my (@kids, $nkids);
	my ($kid);
	my ($lvl, $i, $fn);
	my ($spacer_flag);
	my ($lastkid);

	$spacer_mark = "\\\#";
#	$spacer_mark = "[\\\-\\;\\@\\_\\%\\*\\#\\\$]"; # for '-','$','@','_','%' ...
	$spacer_rexpr = "^" . $spacer_mark . "(.*)";

	$title = $titbl[$idx];
	$fname = $fntbl[$idx];
	$imgnum = $intbl[$idx];
	if ( $fname =~ /^(.+?)\..+?/ ) {
		$fname = $1;
	}
#	print (DEBUG "$idx : $fname \"$title\"\n");
	@kids = split(/;/, $children[$idx]);

	$lastkid = @kids -1;
	for ($i=$lastkid; $i >= 0; $i--) {
		$fn= $fntbl[$kids[$i]];
		if ($fn !~ $spacer_rexpr ) { # if it does NOT start from spacer_mark
			# then this is the last entry
#			print (DEBUG "spacer unmatch :'$fn'\n");
			$lastkid = $i;
			last;
		} else {
#			print (DEBUG "spacer match :'$fn'\n");
		}
	}
	
	$nkids = @kids;
#	print (DEBUG "nkids=$nkids, lastkid=$lastkid \n");
	
	for ($i=0; $i<$nkids; $i++) {
		$level++;
		$levelbar[$level] = "┃　";
#
		push (@out,  "<p>");

		for ($lvl=0; $lvl<$level; $lvl++) {
			put_branch($levelbar[$lvl]);
		}

		$fn= $fntbl[$kids[$i]];
		$spacer_flag = 0;
		if ($fn =~ $spacer_rexpr ) { # if it starts from '#'
#			print (DEBUG "spacer matched '$fn'\n"); 
			$spacer_flag = 1;
			$fn = $1;
			if ($fn eq "") {
				$fn = "&nbsp;";
			}
		}

		if ( $i == $lastkid ) {
			put_branch("┗━");
			$levelbar[$level] = "　　";
			put_directory($fn);
		} elsif ( $i < $lastkid ) {
			if ( $spacer_flag == 1) { # if it is a spacer
				put_branch("┃　");			# non-last kids
				push (@out,  "$fn");
			} else {
				put_branch("┣━");			# non-last kids
				put_directory($fn);
			}
		} else { # after lastkid, all entries are spacers
#				print (DEBUG "after lastkid '$fn'\n");
				put_branch("　　");			# non-last kids
				push (@out,  "$fn");
		}
		push (@out,  "</p>");
		prt_child($kids[$i]);

		$level--;
	}
}

sub put_branch {
	my ($branch) = @_;

	if ($branch eq "┣━") {
#		print "┣━";
		push (@out,  '<img src="ftv2node.png" alt="o" width=16 height=22 />');
	} elsif ($branch eq "┗━") {
#		print "┗━";
		push (@out,  '<img src="ftv2lastnode.png" alt="\" width=16 height=22 />');
	} elsif ($branch eq "＋━") {
#		print "＋━";
		push (@out,  '<img src="ftv2pnode.png" alt="o" width=16 height=22 onclick="toggleFolder(');
		push (@out,  "'folder$folder_idx', this)");
		push (@out,  '"/>');
		push (@out,  '<img src="ftv2folderclosed.png" alt="+" width=24 height=22 onclick="toggleFolder(');
		push (@out,  "'folder$folder_idx', this)");
		push (@out,  '"/>');
	} elsif ($branch eq "┃　") {
#		print "┃　";
		push (@out,  '<img src="ftv2vertline.png" alt="|" width=16 height=22 />');
	} elsif ($branch eq "　　") {
#		print "　　";
		push (@out,  '<img src="ftv2blank.png" alt="&nbsp;" width=16 height=22 />');
	} else {
		push (@out,  "？？");
	}
}

sub put_directory {
	my ($node) = @_;
#
# print "'$node'";
	if ($node =~ /^\*(.+)/) { # if node string starts from '*' then
		$node = $1;
		push (@out,  '<img src="ftv2doc.png" alt="*" width=24 height=22 />');
	} else {
		push (@out,  '<img src="ftv2directory.png" alt="*" width=24 height=22 />');
	}
	push (@out,  "$node");
}

sub prt_comment {
	my ($i);
	my ($tmp);
	for ($i=0; $i<$nspan;$i++) {
		push (@out,  "<tr> <td > <div class=\"directory\"> <p>");
		$tmp = $titbl[$i];
		if ($tmp =~ /^\s*$/) {
			$tmp = "&nbsp;";
		} elsif ($tmp eq "") { # title string is NULL ?
			$tmp = "&nbsp;";
		}
		push (@out,  $tmp);
		push (@out,  "</p></div></td>");
	}
}


