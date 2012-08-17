#/*JP
#\page hhc2toc コンテンツファイル変換スクリプト toc2hhc.pl
#
#\section hhc2toc_synopsis hhc2toc.pl概要
#
#	- NAME
#		hhc2toc.pl : hhc to toc converter
#
#	- SYNOPSIS
#		perl hhc2toc2.pl HHC-FILE > TOC-FILE
#
#	- DESCRIPTION
#		HTML Help Compiler用のコンテンツファイル（拡張子.hhc）を
#		PreDox形式の目次ファイルに変換する。
#		
#	- INPUT
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
#	- OUTPUT
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
#*/
#
# ===== PROGRAM START
#
undef $/; # undef NEWLINE, then all lines are read at one time
#
if ($#ARGV == -1) {	# if there is no args, then use STDIN
	$_ =<STDIN>;
} else {
	open SOURCEFILE, "< $ARGV[$#ARGV]" or die "Can't open sourcefile $ARGV[$#ARGV]\n";
	$_ = <SOURCEFILE>;
	close SOURCEFILE;
}

# １）ページ情報配列 @pgtbl
# 　*.hhcファイルで出現順に"<OBJECT>～</OBJECT>"を記憶する。
# 	$pgtbl[0] : 先頭の<OBJECT>～</OBJECT> ←これは実はページエントリではない。
# 	$pgtbl[1] : 第1の<OBJECT>～</OBJECT>
# 	$pgtbl[2] : 第2の<OBJECT>～</OBJECT>
# 		:
# 		:
# 　ただし、先頭の<OBJECT>～</OBJECT>はFrame制御のために使用されているので
# 　１番目以降がページ情報である。
# ２）タイトル文字列配列 @ttbl
# 　$ttbl[i]には$pgtbl[i]のタイトル文字列が格納される
# 　ここでiは出現順序番号で、かつページ固有の識別番号でもある。
# 
# ３）HTMLファイル名配列 @ftbl
# 　$ftbl[i]には$pgtbl[i]のHTMLファイル名文字列が格納される
# 　
# 　関連ページについて、HTMLファイル名は\page コマンドの識別名<name>が
# 　ベース名として使用される。
# 　
# 　参考：　\page <name> (title)
# 
# 
# ４）階層構造表現配列 @children、@parent
# 	$children[i]が"3;5;8"の場合、i番目のページの下に3,5,8のページ群が
# 	子供として属してことを表す。
# 	$parent[i]はi番目を子供として持つ親の識別番号を保持する。
# 
# 例えば、$k番目のページを削除し、その子供を格上げするには
# 
# 	@bros = split(/;/, $children[$parent[$k]]); # $k-th pageの兄弟
# 	for ($i=0; $i<$#bros; $i++) {
# 		if ($bros[$i] == $k ) {			#兄弟のうち$kを
# 			$bros[$i] = $children[$k];	#$kの子供で置き換える
# 		}
# 	}
# 	$children[$parent[$k]] = join(/;/,@bros); #元にもどす
# 
# 	@kids = split(/;/, $children[$k]); # $k-thの子供
# 	for ($i=0; $i<$#kids; $i++) {
# 		$parent[$kids[$i]] = $parent[$k]; # kの子供の親はkの親とする
# 	}
# 

# ===== 管理テーブル 
# 
@pgtbl = (); # page table
@titbl = (); # title table
@fntbl = (); # file name table
@intbl = (); # image number table
@children = ();
@parent = ();
#
# ===== Regular Expressions
#
$ulopn = "\<ul\>";							# for <UL>
$ulcls = "\</ul\>";							# for </UL>
$object = "\<object(.|\n)+?\<\/object\>";	# for <OBJECT> ... </OBJECT>
											#   ; (.|\n) is necessary to match across lines
											#   ; '?' after '+' forces to match shortest pattern.
$level = 0;
$pgidx = 0;
$title="";
$fname="";
@curparent=();
$curparent[$level] = 0;
$curchildren[$level]= "";

while ( /($ulopn)|($ulcls)|($object)/ig ) { # <UL> or </UL> or <OBJECT>...</OBJECT>
	if ($1 ne "") {							# <ul> found
		$level++;
		$curchildren[$level]= "";			# initialize current children list.
		$curparent[$level] = $pgidx-1;		# save currect level parent idx.
	} elsif ($2 ne "") {					# </ul> found, let us close current parent process.
		$tmp = $curchildren[$level];		# get children id list
		@tmplist = split(/;/, $tmp);		# discard the first NUL element
		$tmp = shift(@tmplist);
		$tmp = join(";", @tmplist);
		$children[$curparent[$level]] = $tmp; # set children list to the parent
		$level--;
	} else {								# <object> ... </object>
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
		$parent[$pgidx] = $curparent[$level];	# set current parent idx
		$children[$pgidx] = "";					# init children list
		$curchildren[$level] = $curchildren[$level] . ";" . $pgidx;
												# append this to current children
		$pgidx++;
	}
}

$i=0;
$level=0;

@kids = split(/;/, $children[0]);	# Here, @kids is an array of children idxs for idx=0;
foreach $kid (@kids) {				# Let's do all children of top level.
	out_child($kid);
}

exit;


#
# ===== out_child : recursive procedure for children
#
sub out_child{
	my ($idx) = @_;
	my (@kids);
	my ($kid);
	my ($lvl);
	my ($title, $fname, $imgnum);

	for ($lvl=0; $lvl<$level; $lvl++) {	# put appropriate TAB codes
		print "\t";
	}
	$title = $titbl[$idx];
	$fname = $fntbl[$idx];
	$imgnum = $intbl[$idx];
	# 2007-02-27 T.Ishikawa
	if ( $fname =~ /^(.+?)\..*(\#.*)/ ) {	# if fname has any extention,
		$fname = $1.$2;
	}
	elsif ( $fname =~ /^(.+?)\../ ) {
		$fname = $1;
	}
	print "\"$title\"\t\"$fname\"\t\"$imgnum\"\n"; # put strings...

	@kids = split(/;/, $children[$idx]);	# get array of children into @kids.
	foreach $kid (@kids) {					# and for each child
		$level++;
		out_child($kid);					# do this process recursively
		$level--;
	}
}
