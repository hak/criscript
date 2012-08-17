#/*JP
#\page hhc2toc �R���e���c�t�@�C���ϊ��X�N���v�g toc2hhc.pl
#
#\section hhc2toc_synopsis hhc2toc.pl�T�v
#
#	- NAME
#		hhc2toc.pl : hhc to toc converter
#
#	- SYNOPSIS
#		perl hhc2toc2.pl HHC-FILE > TOC-FILE
#
#	- DESCRIPTION
#		HTML Help Compiler�p�̃R���e���c�t�@�C���i�g���q.hhc�j��
#		PreDox�`���̖ڎ��t�@�C���ɕϊ�����B
#		
#	- INPUT
#		- HHC-FILE	HTML HELP Compiler�p�̃R���e���c�t�@�C��(�g���q.hhc)
#		�ڎ��\�����`����e�L�X�g�t�@�C���B
#		HTML�`����<UL>�`</UL>�ɂ��K�w�͈͂������A
#		<LI><OBJECT>�`</OBJECT>�Ŋe�G���g����\������B�i���L����Q�Ɓj
#		�y���Ӂz�󔒕����Ƃ��ă^�u�R�[�h�͋����Ȃ��炵���B
#				�^�u�R�[�h�������HTML HELP Compiler���ǂݍ��܂Ȃ��B
#
#	��j
#
#   <!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML//EN">
#   <HTML><HEAD></HEAD>
#   <BODY>
#   <OBJECT type="text/site properties"><param name="FrameName" value="right"></OBJECT>
#   <UL>
#     <LI>
#       <OBJECT type="text/sitemap">
#         <param name="Name" value="��1�� �͂��߂�">
#         <param name="Local" value="main.html">
#         <param name="ImageNumber" value="2">
#       </OBJECT>
#     <UL>
#       <LI>
#         <OBJECT type="text/sitemap">
#           <param name="Name" value="1.1 �����̊T�v">
#           <param name="Local" value="chap11.html">
#           <param name="ImageNumber" value="11">
#         </OBJECT>
#       <LI>
#         <OBJECT type="text/sitemap">
#           <param name="Name" value="1.2 �����̃C���X�g�[��">
#           <param name="Local" value="chap12.html">
#           <param name="ImageNumber" value="1">
#         </OBJECT>
#       <UL>
#         <LI>
#           <OBJECT type="text/sitemap">
#             <param name="Name" value="1.2.1 �����̃_�E�����[�h">
#             <param name="Local" value="page121.html">
#             <param name="ImageNumber" value="11">
#           </OBJECT>
#       </UL>
#       <LI>
#         <OBJECT type="text/sitemap">
#           <param name="Name" value="1.3 �����̓o�^">
#           <param name="Local" value="page13.html">
#           <param name="ImageNumber" value="1">
#         </OBJECT>
#          �F
#          �F
#          �F
#
#	- OUTPUT
#		- TOC-FILE	�ڎ��t�@�C��(�g���q.toc)
#		�ڎ��\�����`����e�L�X�g�t�@�C���B
#		�P�s�łP�G���g���������A�e�s�Ɉȉ���3�v�f���_�u���N�H�[�g��
#		����󔒕�������؂�Ƃ��ĕ��ׂ�B
#			- �^�C�g��������	�FHTML�y�[�W�̃^�C�g���ɕϊ������
#			- �y�[�W��������	�FHTML�t�@�C�����̃x�[�X���ƂȂ�
#			- �A�C�R�����ʔԍ�
#				�e�m�[�h�̏ꍇ��"1"
#				�q�m�[�h�i���[�j�̏ꍇ��"11"
#
#		�s���̃^�u�R�[�h(����)�̐��ŊK�w��\���B
#	��j
#	  �s��
#	  ��
#	  �b"��1�� �͂��߂�" "main" "1"					�^�u���i�́j
#	  �b	"1.1 �����̊T�v" "page11" "11"			�^�u�P�i�߁j
#	  �b	"1.2 �����̃C���X�g�[��" "page12" "1"	�^�u�P�i�߁j
#	  �b		"1.2.1 �����̃_�E�����[�h" "page121" "11"	�^�u�Q�i���j
#	  �b	"1.3 �����̓o�^" "page13" "11"						�F
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

# �P�j�y�[�W���z�� @pgtbl
# �@*.hhc�t�@�C���ŏo������"<OBJECT>�`</OBJECT>"���L������B
# 	$pgtbl[0] : �擪��<OBJECT>�`</OBJECT> ������͎��̓y�[�W�G���g���ł͂Ȃ��B
# 	$pgtbl[1] : ��1��<OBJECT>�`</OBJECT>
# 	$pgtbl[2] : ��2��<OBJECT>�`</OBJECT>
# 		:
# 		:
# �@�������A�擪��<OBJECT>�`</OBJECT>��Frame����̂��߂Ɏg�p����Ă���̂�
# �@�P�Ԗڈȍ~���y�[�W���ł���B
# �Q�j�^�C�g��������z�� @ttbl
# �@$ttbl[i]�ɂ�$pgtbl[i]�̃^�C�g�������񂪊i�[�����
# �@������i�͏o�������ԍ��ŁA���y�[�W�ŗL�̎��ʔԍ��ł�����B
# 
# �R�jHTML�t�@�C�����z�� @ftbl
# �@$ftbl[i]�ɂ�$pgtbl[i]��HTML�t�@�C���������񂪊i�[�����
# �@
# �@�֘A�y�[�W�ɂ��āAHTML�t�@�C������\page �R�}���h�̎��ʖ�<name>��
# �@�x�[�X���Ƃ��Ďg�p�����B
# �@
# �@�Q�l�F�@\page <name> (title)
# 
# 
# �S�j�K�w�\���\���z�� @children�A@parent
# 	$children[i]��"3;5;8"�̏ꍇ�Ai�Ԗڂ̃y�[�W�̉���3,5,8�̃y�[�W�Q��
# 	�q���Ƃ��đ����Ă��Ƃ�\���B
# 	$parent[i]��i�Ԗڂ��q���Ƃ��Ď��e�̎��ʔԍ���ێ�����B
# 
# �Ⴆ�΁A$k�Ԗڂ̃y�[�W���폜���A���̎q�����i�グ����ɂ�
# 
# 	@bros = split(/;/, $children[$parent[$k]]); # $k-th page�̌Z��
# 	for ($i=0; $i<$#bros; $i++) {
# 		if ($bros[$i] == $k ) {			#�Z��̂���$k��
# 			$bros[$i] = $children[$k];	#$k�̎q���Œu��������
# 		}
# 	}
# 	$children[$parent[$k]] = join(/;/,@bros); #���ɂ��ǂ�
# 
# 	@kids = split(/;/, $children[$k]); # $k-th�̎q��
# 	for ($i=0; $i<$#kids; $i++) {
# 		$parent[$kids[$i]] = $parent[$k]; # k�̎q���̐e��k�̐e�Ƃ���
# 	}
# 

# ===== �Ǘ��e�[�u�� 
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
