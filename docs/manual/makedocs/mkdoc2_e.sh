#!/bin/sh

[ -d ../output ] || mkdir ../output

DATE=`./get_date.sh`
PROJECT_DATE=$DATE
PROJECT_NAME="\"CRI Script Manual\""
CHM_FILE="CRI_Script_Manual_e.chm"

# set version number string
PROJECT_NUMBER1="CriScript Version"

perl ../../../tools/bin/get_build_number.pl ../../../inc/versions.h > tmp.txt
PROJECT_NUMBER2=`cat tmp.txt`

PROJECT_NUMBER3="Built on $PROJECT_DATE"
PROJECT_NUMBER="\"$PROJECT_NUMBER1 $PROJECT_NUMBER2 $PROJECT_NUMBER3\""

echo $PROJECT_NUMBER

export DATE PROJECT_DATE PROJECT_NAME PROJECT_NUMBER CHM_FILE
# Probably not necessary...
# export PROJECT_NUMBER1 PROJECT_NUMBER2 PROJECT_NUMBER3
rm tmp.txt

# ドキュメントの作成
# Need to convert newlines and file separators
# This will change all '\' to '/' except at EOLN (for continuation chars)
sed -e 's|\\\([^$]\)|/\1|g' Doxyfile_e | tr -d '\r' > Doxyfile_e.tmp

#
# Apparently, under Cygwin, expansion of environment vars in the doxyfile doesn't work;
# hence this workaround
#
# doxygen Doxyfile_e.tmp
#
( cat Doxyfile_e.tmp 
  # echo "$PROJECT_NAME=$PROJECT_NAME"
  echo "PROJECT_NUMBER=$PROJECT_NUMBER"
  echo "CHM_FILE=$CHM_FILE")| doxygen -

rm Doxyfile_e.tmp

perl hhc2toc.pl  ../output/eng/html/index.hhc >../docs/index_by_doxygen.txt
# ヘルプファイルの作成
./csconv ../output/eng/html/index.hhc -F. -Auto -ROFF
./csconv ../output/eng/html/index.hhk -F. -Auto -ROFF
./csconv ../output/eng/html/index.hhp -F. -Auto -ROFF
cp -r ./gfx/*.* ../output/eng/html
cp -r ../images/*.png ../output/eng/html
cp -r ../images/*.jpg ../output/eng/html

# コンパイルオプションの追加
perl kwgen/hhpopt.pl ../output/eng/html/index.hhp > index.hhp
mv index.hhp ../output/eng/html/index.hhp

# 目次ファイルの変換
perl toc2hhc.pl ../docs/index_e.toc > ../output/eng/html/index.hhc
perl toc2tree.pl ../docs/index_e.toc > ../output/eng/html/tree.html

# キーワードの追加
perl kwgen/hhk2kwl.pl ../output/eng/html/index.hhk > index_kwl.txt
cat ../docs/kw_list_e.txt index_kwl.txt > kwl.txt
perl kwgen/kwl2hhk.pl kwl.txt > index.hhk
cp index.hhk ../output/eng/html/index.hhk
rm kwl.txt index_kwl.txt index.hhk

"/c/Program Files/HTML Help Workshop/hhc" ../output/eng/html/index.hhp

