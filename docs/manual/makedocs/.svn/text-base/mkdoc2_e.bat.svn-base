md ..\output

call get_date "%DATE%"
set PROJECT_NAME=CRI Script Manual
rem set PROJECT_NUMBER=Updated on %PROJECT_DATE%
set CHM_FILE=CRI_Script_Manual_e.chm

rem set version number string
set PROJECT_NUMBER1=CriScript Version
perl ..\..\..\tools\bin\get_build_number.pl ..\..\..\inc\versions.h > tmp.txt
set PROJECT_NUMBER2=
set /P PROJECT_NUMBER2="Setting Project Number" < tmp.txt

set PROJECT_NUMBER3= Bult on %DATE%
set PROJECT_NUMBER=%PROJECT_NUMBER1% %PROJECT_NUMBER2% %PROJECT_NUMBER3%
rem date /T > tmp.txt
rem set /P PROJECT_NUMBER="Setting Project Number" < tmp.txt

del tmp.txt

rem ドキュメントの作成
doxygen Doxyfile_e

perl hhc2toc.pl  ..\output\eng\html\index.hhc >..\docs\index_by_doxygen.txt
rem ヘルプファイルの作成
csconv ..\output\eng\html\index.hhc -F%%SOURCE%% -Auto -ROFF
csconv ..\output\eng\html\index.hhk -F%SOURCE% -Auto -ROFF
csconv ..\output\eng\html\index.hhp -F%SOURCE% -Auto -ROFF
xcopy .\gfx\*.* ..\output\eng\html /y
xcopy ..\images\*.png ..\output\eng\html /y
xcopy ..\images\*.jpg ..\output\eng\html /y

rem コンパイルオプションの追加
perl kwgen\hhpopt.pl ..\output\eng\html\index.hhp > index.hhp
copy index.hhp ..\output\eng\html\index.hhp
del index.hhp

rem 目次ファイルの変換
perl toc2hhc.pl ..\docs\index_e.toc >..\output\eng\html\index.hhc
perl toc2tree.pl ..\docs\index_e.toc >..\output\eng\html\tree.html

rem キーワードの追加
perl kwgen\hhk2kwl.pl ..\output\eng\html\index.hhk > index_kwl.txt
copy ..\docs\kw_list_e.txt+index_kwl.txt kwl.txt
perl kwgen\kwl2hhk.pl kwl.txt > index.hhk
copy index.hhk ..\output\eng\html\index.hhk
del kwl.txt index_kwl.txt index.hhk

"C:\Program Files\HTML Help Workshop\hhc" ..\output\eng\html\index.hhp

rem ヘルプファイルの表示
start ..\output\eng\html\%CHM_FILE%
start ..\output\eng\html\index.html

