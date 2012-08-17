#####################################################
#
#
#
undef $/;

open SOURCEFILE, "< $ARGV[$#ARGV]" or die "Can't open sourcefile $ARGV[$#ARGV]\n";
$_ = <SOURCEFILE>;
close SOURCEFILE;

#open DEBUG, ">debug.txt" or die "Can't open debug.txt\n";


@lines=split(/\n/, $_);

	tree_process(); # input @lines

	put_html();

exit;

#
#####################################################
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
#
}
#
#####################################################
#
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
	@buf=();
#

$level=0;
		push( @buf, "<OBJECT type=\"text/sitemap\">");
		push( @buf, "<param name=\"Name\" value=\"Root\">");
		push( @buf, "<param name=\"Local\" value=\"Root.html\">");
		push( @buf, "<param name=\"ImageNumber\" value=\"1\">");
		push( @buf, "</OBJECT>");

foreach $ln (@lines) {
	$rank= 0;	# rank is ntab+1, means hierarchy, that is, top-level is 1.
	for ($ntab=0; $ntab < $#ntabexpr; $ntab++) {
		if ($ln =~ /$ntabexpr[$ntab]/) {
			$title = $1;
			$fname = $2;
			$imgnum = $3;
			$rank = $ntab+1;
			break;
		}
	}
	if ($rank == 0) {
#		print "Illegal entry in $ARGV[$#ARGV] : $ln\n";
		next;
	}
#	print "$rank: $fname $title $imgnum\n";

	if ( $level > $rank) { 					# if a lower rank entry appears
		for (; $level > $rank; $level--) {	# then close higher level brackets
#			put_tabs($level-1);
			push(@buf, "</UL>");
		}
	} elsif ($level < $rank) {				#if a higher rank entry appears
		for (; $level< $rank; $level++) {	# then open new brackets
#			put_tabs($level);				# but, actually, the entry should be
			push( @buf, "<UL>");					# only one level higher
		}
	}
	if ($rank > 0) { #if rank <= 0, then the line does NOT specify contents.
#
		push( @buf, "<OBJECT type=\"text/sitemap\">");
		push( @buf, "<param name=\"Name\" value=\"$title\">");
		$fname = make_linkpath($fname);
		push( @buf, "<param name=\"Local\" value=\"$fname\">");
		push( @buf, "<param name=\"ImageNumber\" value=\"$imgnum\">");
		push( @buf, "</OBJECT>");
	}
}

for (; $level > 0; $level--) {
	push( @buf, "</UL>");
}

}
#
##################################################################
#
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
#foreach $ln (@buf) {
#	print DEBUG "$ln\n";
#}


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
			$i = $curparent[$level];
#			print (DEBUG "$i : $fntbl[$i] P=$parent[$i] C=$children[$i]\n");
			
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
			$i = $pgidx;
#			print (DEBUG "$i : $fntbl[$i] P=$parent[$i] C=$children[$i]\n");
			$pgidx++;
		}
	}
	
#	for($i=0; $i<$pgidx; $i++) {
#		print (DEBUG "$i : $fntbl[$i] P=$parent[$i] C=$children[$i]\n");
#	}
	
	$nspan = $pgidx+1;

}


sub put_html{

	$level=0;
	push(@levelbar, "　　");
	put_header();
	put_body("Table of Contents");

}

sub put_body {
	my ($title) = @_;
	print <<'EOT';
  <body>
EOT
	print '<div class="directory">', "\n";
	print "<h3>$title</h3>\n";
#
	print '<div style="display: block;">', "\n";
#
	prt_child(0);

	print <<'EOT';
    </div>
  </body>
</html>
EOT
}



sub prt_child {
	my ($idx) = @_;
	my (@kids, $nkids);
	my ($kid);
	my ($lvl, $i, $fn);
	my ($spacer_flag);
	my ($lastkid);
	my (@gkids, $ngkids); # no. of grand children

	$spacer_mark = "\\\*";
#	$spacer_mark = "[\\\-\\;\\@\\_\\%\\*\\#\\\$]"; # for '-','$','@','_','%' ...
	$spacer_rexpr = "^" . $spacer_mark . "(.*)";

	$title = $titbl[$idx];
	$fname = $fntbl[$idx];
	$imgnum = $intbl[$idx];

#	print (DEBUG "$idx : $fname \"$title\"\n");
	@kids = split(/;/, $children[$idx]);

	$nkids = @kids;
#	print (DEBUG "nkids=$nkids, lastkid=$lastkid \n");
	
	for ($i=0; $i<$nkids; $i++) {
		@gkids = split(/;/, $children[$kids[$i]]);
		$ngkids = @gkids;
		$level++;
		$levelbar[$level] = "┃　";
#
		print "<p>";

		for ($lvl=0; $lvl<$level; $lvl++) {
			put_branch($levelbar[$lvl]);
		}
		if ($i == $nkids-1) {
			if ($ngkids == 0) {
				put_branch("┗━");	# node (non-last kids)
				put_node($kids[$i]);
				$levelbar[$level] = "　　";
			} else {
				$folder_idx++;
				put_branch("＋━"); # Folder
				put_node($kids[$i]);
				$levelbar[$level] = "　　";
			}
		} else {
			if ($ngkids == 0) {
				put_branch("┣━");	# node (non-last kids)
				put_node($kids[$i]);
			} else {
				$folder_idx++;
				put_branch("田━"); # Folder
				put_node($kids[$i]);
			}
		}
		print "</p>\n";
		if ($ngkids == 0) {
			prt_child($kids[$i]);
		} else {
			print "<div id =\"folder$folder_idx\">\n";
			$folder_idx++;
			prt_child($kids[$i]);
			print "</div>\n";
		}
		$level--;
	}
}


sub put_branch {
	my ($branch) = @_;

	if ($branch eq "┣━") {
#		print "┣━";
		print '<img src="ftv2node.png" alt="o" width=16 height=22 />';
		print '<img src="ftv2doc.png" alt="*" width=24 height=22 />';
	} elsif ($branch eq "┗━") {
#		print "┗━";
		print '<img src="ftv2lastnode.png" alt="\" width=16 height=22 />';
		print '<img src="ftv2doc.png" alt="*" width=24 height=22 />';
	} elsif ($branch eq "田━") {
#		print "田━";
		print '<img src="ftv2pnode.png" alt="o" width=16 height=22 onclick="toggleFolder(';
		print "'folder$folder_idx', this)";
		print '"/>';
		print '<img src="ftv2folderclosed.png" alt="+" width=24 height=22 onclick="toggleFolder(';
		print "'folder$folder_idx', this)";
		print '"/>';
	} elsif ($branch eq "＋━") {
#		print "＋━";
		print '<img src="ftv2plastnode.png" alt="\" width=16 height=22 onclick="toggleFolder(';
		print "'folder$folder_idx', this)";
		print '"/>';
		print '<img src="ftv2folderclosed.png" alt="+" width=24 height=22 onclick="toggleFolder(';
		print "'folder$folder_idx', this)";
		print '"/>';
	} elsif ($branch eq "┃　") {
#		print "┃　";
		print '<img src="ftv2vertline.png" alt="|" width=16 height=22 />';
	} elsif ($branch eq "　　") {
#		print "　　";
		print '<img src="ftv2blank.png" alt="&nbsp;" width=16 height=22 />';
	} else {
		print "？？";
	}
}

sub put_node {
	my ($node) = @_;
#
	print '<a class="el" href="';
	print "$fntbl[$node]";
	print '" target="basefrm">';
	print "$titbl[$node]";
	print "</a>";
}

sub put_header{

print <<'EOT';
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
  <head>
    <meta http-equiv="Content-Type" content="text/xhtml;charset="Shift_JIS" />
    <meta http-equiv="Content-Style-Type" content="text/css" />
    <meta http-equiv="Content-Language" content="en" />
    <link rel="stylesheet" href="cri_doc_style_j.css">
    <title>TreeView</title>
    <style type="text/css">
    <!--
    .directory { font-size: 10pt; font-weight: bold; }
    .directory h3 { margin: 0px; margin-top: 1em; font-size: 11pt; }
    .directory p { margin: 0px; white-space: nowrap; }
    .directory div { display: none; margin: 0px; }
    .directory img { vertical-align: middle; }
    -->
    </style>
    <script type="text/javascript">
    <!-- // Hide script from old browsers
    
    function findChildNode(node, name) 
    {
      var temp;
      if (node == null) 
      {
        return null;
      } 
      node = node.firstChild;
      while (node != null) 
      {
        if (node.nodeName == name) 
        {
          return node;
        }
        temp = findChildNode(node, name);
        if (temp != null) 
        {
          return temp;
        }
        node = node.nextSibling;
      }
      return null;
    }

    function toggleFolder(id, imageNode) 
    {
      var folder = document.getElementById(id);
      var l = 0;
      var vl = "ftv2vertline.png";
      if (imageNode != null && imageNode.nodeName != "IMG") 
      {
        imageNode = findChildNode(imageNode, "IMG");
        if (imageNode!=null) l = imageNode.src.length;
      }
      if (folder == null) 
      {
      } 
      else if (folder.style.display == "block") 
      {
        while (imageNode != null && 
               imageNode.src.substring(l-vl.length,l) == vl)
        {
          imageNode = imageNode.nextSibling;
          l = imageNode.src.length;
        }
        if (imageNode != null) 
        {
          l = imageNode.src.length;
          imageNode.nextSibling.src = "ftv2folderclosed.png";
          if (imageNode.src.substring(l-13,l) == "ftv2mnode.png")
          {
            imageNode.src = "ftv2pnode.png";
          }
          else if (imageNode.src.substring(l-17,l) == "ftv2mlastnode.png")
          {
            imageNode.src = "ftv2plastnode.png";
          }
        }
        folder.style.display = "none";
      } 
      else 
      {
        while (imageNode != null && 
               imageNode.src.substring(l-vl.length,l) == vl)
        {
          imageNode = imageNode.nextSibling;
          l = imageNode.src.length;
        }
        if (imageNode != null) 
        {
          l = imageNode.src.length;
          imageNode.nextSibling.src = "ftv2folderopen.png";
          if (imageNode.src.substring(l-13,l) == "ftv2pnode.png")
          {
            imageNode.src = "ftv2mnode.png";
          }
          else if (imageNode.src.substring(l-17,l) == "ftv2plastnode.png")
          {
            imageNode.src = "ftv2mlastnode.png";
          }
        }
        folder.style.display = "block";
      }
    }

    // End script hiding -->        
    </script>
  </head>
EOT
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
