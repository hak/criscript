#*************************************************************************
#	Appli	：HTML Help Workshop補助ツール
#	Module	：キーワードリスト生成
#	File	：kwlgen.pl
#	Date	：2007-03-08
#	Author	：石川達也
#*************************************************************************
#=========================================================================
#	機能
#=========================================================================
#	■概要
#	　マニュアル(chm)のソース(txt)を解析し、キーワードを以下の形式で列挙
#	します。
#
#	│"キーワード1" "リンク先1"
#	│	"キーワード11" "リンク先11"
#	│"キーワード2" "リンク先2"
#	│	"キーワード21" "リンク先21"
#	│	"キーワード21" "リンク先21"
#
#	■書式
#	　perl kwlgen.pl [ファイルリスト] [言語モード] > [出力ファイル]
#
#	　[ファイルリスト]……キーワードを抽出するファイルのリスト
#	　[言語モード]　　……列挙するキーワードの言語
#	　　　　　　　　　　（※指定がない場合は日本語になります。）
#
#	（例）
#	 > perl kwlgen.pl ../../docs/index_j.toc JP > kw_list_j.txt
#
#	■入力ファイルの形式
#	　入力する[ファイルリスト]には以下のものが使用できます。
#	(a)　tocファイル
#	(b)　ディレクトリパス
#	 　　※ソースのあるディレクトリを指定するとそのディレクトリ以下にある全て
#	 　　のtxtファイルからキーワードを抽出します。
#
#	■キーワードリストのフォーマット
#	・１行で１エントリ
#	・各行に以下の2要素を"で括り半角スペースで区切る
#	　キーワード：表示されるキーワード。
#	　リンク先　：キーワードのリンク先。(指定のない場合はリンクが付きません)
#	行頭のタブでグループ分けをします。
#
#	（例）
#	│"データの作成手順" "usr_ssenc_procedure.html"
#	│	"新規プロジェクトの作成" "usr_ssenc_procedure.html#criss_project"
#	│	"素材ファイルの登録" "usr_ssenc_procedure.html#criss_registration_of_encode_file"
#	│"データ作成の基本操作" "usr_ssenc_intro.html"
#	│"エンコードウィンドウ"
#	│	"エンコードウィンドウ" "usr_ssenc_dialog.html#usr_ssenc_encode_window"
#	│	"オプションダイアログ" "usr_ssenc_dialog.html#usr_ssenc_option_dialog"
#
#-------------------------------------------------------------------------
#	変更履歴
#-------------------------------------------------------------------------
#	2006-12-05	新規作成
#	2007-03-08	tocから生成する場合の処理を変更
#-------------------------------------------------------------------------
#=========================================================================
#	メイン
#=========================================================================
#	引数取得
$listfile = $ARGV[0];	# ファイルリスト
$lang     = $ARGV[1];	# 言語(JP/EN)
@filelist = ();			# ファイルリスト配列

if ($lang eq "") {
	$lang = "JP";
}

# ソースファイル(txt)のフォルダへのパス
if ($lang eq "JP") {
	$srcpath  = "../../docs/jpn/";
}
else {
	$srcpath  = "../../docs/eng/";
}

if ($listfile eq "") {
	die("リストファイルを指定してください。\n");
}

# ファイルの拡張子取得
$tmp = $listfile;
if ($tmp =~ /(.*)\.(.*)/) {
	$ext = $2;
	if ($ext eq "toc") {
		@pagelist = crate_file_table_toc($listfile);
		@filelist = GetSourcefileList($srcpath);
		$mode = "toc";
	}
	else {
		@pagelist = 1;
		@filelist = GetSourcefileList($srcpath);
		$mode = "dir";
	}
}

@fname = ();		# name配列初期化
@title = ();		# title配列初期化
$count = 0;			# ファイル内要素数カウント
$blangflg = 0;		# 言語フラグ
$bmainpageflg = 0;	# メインページフラグ
$findpageflg  = 0;	# 検索するページ発見フラグ

for ($p = 0; $p < @pagelist; $p++) {

	for ($f = 0; $f < @filelist; $f++) {

		if (@filelist[$f] =~ /\.$/) {
			next;
		}
		# ファイルオープンに失敗
		if (!open(SOURCE, @filelist[$f])) {
			print "@filelist[$f]のオープンに失敗しました。スキップします。\n";
			next;
		}

		# ページ内検索
		while (<SOURCE>) {
			# 言語チェック
			if ($lang eq "EN") {
				if ($_ =~ m|^\/\*JP|) {
					$blangflg = 1;
				}
				if ($_ =~ m|^\*\/|) {
					$blangflg = 0;
				}
				if ($blangflg == 1) {
					next;
				}
			}
			elsif ($lang eq "JP") {
				if ($_ =~ m|^\/\*EN|) {
					$blangflg = 1;
				}
				if ($_ =~ m|^\*\/|) {
					$blangflg = 0;
				}
				if ($blangflg == 1) {
					next;
				}
			}

			# \pageの行
			if ($_ =~ /\\page/) {
				if ($_ =~ /\\page\s(.*?)\s(.*)/) {
					# tocから検索した場合
					if ($mode eq "toc") {
						if (@pagelist[$p] eq $1) {
							$findpageflg = 1;
						}
						else {
							$findpageflg = 0;
							next;
						}
					}
					# docsフォルダ以下全部検索する場合
					elsif ($mode eq "dir") {
						$findpageflg = 1;
					}

					@fname[$count] = $1.".html";	# ファイル名
					@title[$count] = $2;			# 項目名
					$count++;
				}
			}
			elsif ($findpageflg == 0) {
				next;
			}

			# \sectionの行
			elsif ($_ =~ /\\section/) {
				if ($_ =~ /\\section\s(.*?)\s(.*)/) {
					if ($2 eq "目次") {
						next;
					}
					elsif ($2 eq "Table of contents") {
						next;
					}
					elsif ($2 eq "Table of Contents") {
						next;
					}
					@fname[$count] = $1;	# <a name
					@title[$count] = $2;	# 項目名
					$count++;
				}
			}
		}
	}

	close(SOURCE);
}

# 検索結果を出力
for ($n = 0; $n < $count; $n++) {
	if (@fname[$n] =~ ".html") {
		print "\"@title[$n]\" \"@fname[$n]\"\n";
		$tmpfname = @fname[$n];	# htmlファイル名を一時保存
		$tmptitle = @title[$n];	# htmlファイル名を一時保存
	}
	else {
		if ($tmptitle eq @title[$n]) {
			next;
		}
		print "\t\"@title[$n]\" \"$tmpfname#@fname[$n]\"\n";
	}
}

exit();

#=========================================================================
#	サブルーチン
#=========================================================================
#-------------------------------------------------------------------------
#	tocファイルからファイルリストの配列生成
#-------------------------------------------------------------------------
sub crate_file_table_toc {
	my ($lines)    = "";	#	tocファイルの中身を格納する配列
	my (@filelist) = ();	#	ファイルパスを格納する配列
	my ($fcount)   = 0;		#	ファイル数

	$file = @_[0];			#	tocファイル名取得

	if (!open(LIST, $file)) {
		die("tocファイルのオープンに失敗しました。\n");
	}

	while(<LIST>) {
		$lines = $_;
		$lines =~ s/\n//;	#	改行コード削除
		$lines =~ s/^\t*//;	#	行頭のタブ削除
		if ($lines =~ /\"(.+?)\"\s*\"(.+?)\"\s*\"(.+?)\"/) {	# 要素抜き出し
			@filelist[$fcount] = "$2";
		}

		$fcount++;
	}

	close(LIST);

	return @filelist;
}

#-------------------------------------------------------------------------
#	ディレクトリ以下のファイルパスを配列に入れる
#-------------------------------------------------------------------------
sub GetSourcefileList {
	my (@filelist) = ();	# ソーステキストファイルリストの配列
	my ($nCount)   = 0;		# 配列カウント数
	$dirpath = @_[0];		# ディレクトリパス

	if (!opendir(DIR, $dirpath)) {
		die("$dirpath:ディレクトリオープンに失敗しました。");
	}

	@filelist = readdir(DIR);
	closedir(DIR);

	for ($nCount = 0; $nCount < @filelist; $nCount++) {
		@filelist[$nCount] = $dirpath.@filelist[$nCount];
	}

	return @filelist;
}

