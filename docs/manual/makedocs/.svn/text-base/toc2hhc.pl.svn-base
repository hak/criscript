#/*JP
#\page toc2hhc 目次ファイル変換スクリプト toc2hhc.pl
#
#\section toc2hhc_synopsis toc2hhc.pl概要
#
#	- NAME
#		toc2hhc.pl : toc to hhc converter
#
#	- SYNOPSIS
#		perl toc2hhc.pl TOC-FILE > HHC-FILE
#
#	- DESCRIPTION
#		PreDox形式の目次ファイルをHTML Help Compiler用のコンテンツ
#		ファイル（拡張子.hhc）に変換する。
#		
#	- INPUT
#		- TOC-FILE	目次ファイル(拡張子.toc)
#		目次構成を定義するテキストファイル。
#		１行で１エントリを示し、各行に以下の3要素をダブルクォートで
#		括り空白文字を区切りとして並べる。
#			- タイトル文字列	：HTMLページのタイトルに変換される
#			- ページ名文字列	：HTMLファイル名のベース名となる
#			- アイコン識別番号
#				親ノードの場合は"1"
#				子ノード（末端）の場合は"11"
#
#		行頭のタブコード(￥ｔ)の数で階層を表す。
#	例）
#	  行頭
#	  ↓
#	  ｜"第1章 はじめに" "main" "1"					タブ無（章）
#	  ｜	"1.1 ○○の概要" "page11" "11"			タブ１（節）
#	  ｜	"1.2 ○○のインストール" "page12" "1"	タブ１（節）
#	  ｜		"1.2.1 ○○のダウンロード" "page121" "11"	タブ２（項）
#	  ｜	"1.3 ○○の登録" "page13" "11"						：
#
#	- OUTPUT
#		- HHC-FILE	HTML HELP Compiler用のコンテンツファイル(拡張子.hhc)
#		目次構成を定義するテキストファイル。
#		HTML形式で<UL>～</UL>により階層範囲を示し、
#		<LI><OBJECT>～</OBJECT>で各エントリを表現する。（下記例を参照）
#		【注意】空白文字としてタブコードは許さないらしい。
#				タブコードが入るとHTML HELP Compilerが読み込まない。
#
#	例）
#
#   <!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML//EN">
#   <HTML><HEAD></HEAD>
#   <BODY>
#   <OBJECT type="text/site properties"><param name="FrameName" value="right"></OBJECT>
#   <UL>
#     <LI>
#       <OBJECT type="text/sitemap">
#         <param name="Name" value="第1章 はじめに">
#         <param name="Local" value="main.html">
#         <param name="ImageNumber" value="2">
#       </OBJECT>
#     <UL>
#       <LI>
#         <OBJECT type="text/sitemap">
#           <param name="Name" value="1.1 ○○の概要">
#           <param name="Local" value="chap11.html">
#           <param name="ImageNumber" value="11">
#         </OBJECT>
#       <LI>
#         <OBJECT type="text/sitemap">
#           <param name="Name" value="1.2 ○○のインストール">
#           <param name="Local" value="chap12.html">
#           <param name="ImageNumber" value="1">
#         </OBJECT>
#       <UL>
#         <LI>
#           <OBJECT type="text/sitemap">
#             <param name="Name" value="1.2.1 ○○のダウンロード">
#             <param name="Local" value="page121.html">
#             <param name="ImageNumber" value="11">
#           </OBJECT>
#       </UL>
#       <LI>
#         <OBJECT type="text/sitemap">
#           <param name="Name" value="1.3 ○○の登録">
#           <param name="Local" value="page13.html">
#           <param name="ImageNumber" value="1">
#         </OBJECT>
#          ：
#          ：
#          ：
#
#*/
#
# ===== PROGRAM START
#
undef $/;			# Undefine the record separator to read all data at once ignoring NEWLINE.

open SOURCEFILE, "< $ARGV[$#ARGV]" or die "Can't open sourcefile $ARGV[$#ARGV]\n";
$_ = <SOURCEFILE>;	# Now $_ has all data.
close SOURCEFILE;

@lines=split(/\n/, $_);	# Array @lines includes all lines.

#
# reg-expr for n-TABs at the beginning of a line.
# These reg-expr match [TAB...]"title-string" "file-name-string" "image-number"
# 
@ntabexpr = (
	"^\"(.+?)\"\\s+?\"(.+?)\"\\s+?\"(.+?)\"",								# 0-TAB,
	"^\\t\"(.+?)\"\\s+?\"(.+?)\"\\s+?\"(.+?)\"",							# 1-TAB
	"^\\t\\t\"(.+?)\"\\s+?\"(.+?)\"\\s+?\"(.+?)\"",							# 2
	"^\\t\\t\\t\"(.+?)\"\\s+?\"(.+?)\"\\s+?\"(.+?)\"",						# 3
	"^\\t\\t\\t\\t\"(.+?)\"\\s+?\"(.+?)\"\\s+?\"(.+?)\"",					# 4
	"^\\t\\t\\t\\t\\t\"(.+?)\"\\s+?\"(.+?)\"\\s+?\"(.+?)\"",				# 5
	"^\\t\\t\\t\\t\\t\\t\"(.+?)\"\\s+?\"(.+?)\"\\s+?\"(.+?)\"",				# 6
	"^\\t\\t\\t\\t\\t\\t\\t\"(.+?)\"\\s+?\"(.+?)\"\\s+?\"(.+?)\"",			# 7
	"^\\t\\t\\t\\t\\t\\t\\t\\t\"(.+?)\"\\s+?\"(.+?)\"\\s+?\"(.+?)\"",		# 8
	"^\\t\\t\\t\\t\\t\\t\\t\\t\\t\"(.+?)\"\\s+?\"(.+?)\"\\s+?\"(.+?)\"");	# 9
#
#

put_header();	# output boring header lines.

$level=0;

foreach $ln (@lines) {
	$rank= 0;	# rank is ntab+1, means hierarchy, that is, top-level is 1.
	for ($ntab=0; $ntab < $#ntabexpr; $ntab++) {
		if ($ln =~ /$ntabexpr[$ntab]/) {	#if the line has some TAB ?
			$title = $1;					# then keep title,file-name and icon-id.
			$fname = $2;
			$imgnum = $3;
			$rank = $ntab+1;				# and rank may be 1,2,3 ...
			break;
		}
	}
	if ($rank == 0) {						# if rank == 0 then no-match for toc record.
		die "Illegal entry in $ARGV[$#ARGV] : $ln\n";
	}

	if ( $level > $rank) { 					# if a lower rank entry appears
		for (; $level > $rank; $level--) {	# then close higher level brackets
			put_tabs($level-1);
			print "</UL>\n";
		}
	} elsif ($level < $rank) {				#if a higher rank entry appears
		for (; $level< $rank; $level++) {	# then open new brackets
			put_tabs($level);				# but, actually, the entry should be
			print "<UL>\n";					# only one level higher
		}
	}
	if ($rank > 0) { #if rank <= 0, then the line does NOT specify contents.
#
#		put_tabs($level);
#		print "<LI>\n";
#		put_tabs($level);
#		print "<OBJECT type=\"text/sitemap\">\n";
#		put_tabs($level+1);
#		print "<param name=\"Name\" value=\"$title\">\n";
#		put_tabs($level+1);
#		print "<param name=\"Local\" value=\"$fname.html\">\n";
#		put_tabs($level+1);
#		print "<param name=\"ImageNumber\" value=\"$imgnum\">\n";
#		put_tabs($level);
#		print "</OBJECT>"
#
# Unfortunately, HTML Help Compiler seems to hate TABs or
# some strange indentation.
#
		put_tabs($level);
		print "<LI>";
		print "<OBJECT type=\"text/sitemap\">";
		print "<param name=\"Name\" value=\"$title\">";
		$fname = make_linkpath($fname);
		print "<param name=\"Local\" value=\"$fname\">";
		print "<param name=\"ImageNumber\" value=\"$imgnum\">";
		print "</OBJECT>\n"
	}
}

for (; $level > 0; $level--) {
	put_tabs($level-1);
	print "</UL>\n";
}

put_footer();

exit;
#
# ===== put_tabs : output ntabs tabs, but I changed TAB code to
#                  two space code ("  ") because HHC hates TAB code.
#
sub put_tabs{
	my ($ntabs) = @_;
	my ($i);

	for ($i=0; $i<$ntabs; $i++) {
		print "  "; # HHC says "DON'T USE TAB!"
	}
}

#
# ===== put_header : output boring fixed header lines.
#
sub put_header{
print <<'EOT'
<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML//EN">
<HTML>
<HEAD>
</HEAD>
<BODY>
<OBJECT type="text/site properties"><param name="FrameName" value="right"></OBJECT>
EOT
	;
}

#
# ===== put_footer : output boring fixed footer lines.
#
sub put_footer{
print <<'EOT'
</BODY></HTML>
EOT
	;
}

sub make_linkpath{
	my ($fname) = @_;

	if ($fname =~ /\#/) {
		$fname =~ s/\#/\.html\#/;
	}
	else {
		$fname .= ".html";
	}

	return $fname;
}
