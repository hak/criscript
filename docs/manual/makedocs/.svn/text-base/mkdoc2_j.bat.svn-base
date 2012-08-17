md ..\output

call get_date %DATE%
set PROJECT_NAME=CRI Audio マニュアル
set PROJECT_NUMBER=%PROJECT_DATE% 更新
set CHM_FILE=CRI_Audio_Manual_j.chm

rem ドキュメントの作成
doxygen Doxyfile_j

perl hhc2toc.pl  ..\output\jpn\html\index.hhc >..\docs\index_by_doxygen.txt
rem ヘルプファイルの作成
csconv ..\output\jpn\html\index.hhc -F%%SOURCE%% -Auto -ROFF
csconv ..\output\jpn\html\index.hhk -F%SOURCE% -Auto -ROFF
csconv ..\output\jpn\html\index.hhp -F%SOURCE% -Auto -ROFF
xcopy .\gfx\*.* ..\output\jpn\html /y
xcopy ..\images\*.png ..\output\jpn\html /y
xcopy ..\images\*.jpg ..\output\jpn\html /y

rem コンパイルオプションの追加
perl kwgen\hhpopt.pl ..\output\jpn\html\index.hhp > index.hhp
copy index.hhp ..\output\jpn\html\index.hhp
del index.hhp

rem 目次ファイルの変換
perl toc2hhc.pl ..\docs\index_j.toc >..\output\jpn\html\index.hhc
perl toc2tree.pl ..\docs\index_j.toc >..\output\jpn\html\tree.html

rem キーワードの追加
perl kwgen\hhk2kwl.pl ..\output\jpn\html\index.hhk > index_kwl.txt
copy ..\docs\kw_list_j.txt+index_kwl.txt kwl.txt
perl kwgen\kwl2hhk.pl kwl.txt > index.hhk
copy index.hhk ..\output\jpn\html\index.hhk
del kwl.txt index_kwl.txt index.hhk

"C:\Program Files\HTML Help Workshop\hhc" ..\output\jpn\html\index.hhp

rem ヘルプファイルの表示
start ..\output\jpn\html\%CHM_FILE%
start ..\output\jpn\html\index.html
