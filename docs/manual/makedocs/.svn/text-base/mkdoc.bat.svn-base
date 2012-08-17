md ..\output

rem ドキュメントの作成
doxygen Doxyfile_j

rem ヘルプファイルの作成
csconv ..\output\jpn\html\index.hhc -F%%SOURCE%% -Auto -ROFF
csconv ..\output\jpn\html\index.hhk -F%SOURCE% -Auto -ROFF
csconv ..\output\jpn\html\index.hhp -F%SOURCE% -Auto -ROFF
xcopy .\gfx\*.* ..\output\jpn\html /y
xcopy ..\images\*.png ..\output\jpn\html /y

rem 目次ファイルの変換
perl hhc2toc.pl ..\output\jpn\html\index.hhc | perl delpages.pl > tmp.txt
copy ..\docs\index_j.toc+tmp.txt tmp2.txt
perl toc2hhc.pl tmp2.txt >..\output\jpn\html\index.hhc
perl toc2tree.pl tmp2.txt >..\output\jpn\html\tree.html
del tmp.txt tmp2.txt
"C:\Program Files\HTML Help Workshop\hhc" ..\output\jpn\html\index.hhp

rem ヘルプファイルの表示
start ..\output\jpn\html\.chm
start ..\output\jpn\html\index.html
