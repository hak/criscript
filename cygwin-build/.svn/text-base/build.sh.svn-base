#!/bin/sh

#
# CriScript nightly build:
#
# * Fetch code from version control
# * Do full build
# * Run test suite(s)
# * Build archives for source, binaries, etc.
# * Anything else necessary
# * Initiate copy-to-server process

#
# Configuration; default options
#
# Change to this directory before starting build
BUILD_DIR=/criscript

# Directory (relative to BUILD_DIR) where source is put
SRC_PATH='CriScript/'

# Other directories we need for the build/release process
LIBS_DIR="libs"
DOCS_DIR="docs"
TOOLS_DIR="tools"

# This is based off the top-level build directory
HTML_DOC_DIR="$SRC_PATH/$DOCS_DIR/manual/output/eng/html"

# Where the archives &c. we build go
ARCHIVE_DIR="$BUILD_DIR/dist"

# Server where CriScript source lives; Subversion
SVN_SERVER='192.168.10.150'

# Path to root of CriScript source, used in svn:// URL
SVN_REPOS_PATH="$SVN_SERVER/trunk"

# Server where additional code lives; Perforce
P4_SERVER='192.168.2.10'

# Server where publicly downloadable files live
CRISCRIPT_SERVER='192.168.10.150'

# Who to run the remote commands as
CRISCRIPT_USER='root'

# Remote path where uploads go
CRISCRIPT_REMOTE_PATH='criscript'
CRISCRIPT_UL_PATH="$CRISCRIPT_REMOTE_PATH/upload"

# Remote command to put newly-copied files in the right place
CRISCRIPT_UL_CMD="$CRISCRIPT_REMOTE_PATH/update-criscript-file.sh"

# Files containing list of files for SDK, source archives
SDK_ARCHIVE_LIST="$SRC_PATH/SDK-archive.txt"
WIN32_SRC_ARCHIVE_LIST="$SRC_PATH/source-archive-win32.txt"

# Symlinks we create on the server, for consistent downloadable filenames
SDK_ZIP_ARCHIVE_LINKNAME="CriScript-SDK.zip"
SDK_TGZ_ARCHIVE_LINKNAME="CriScript-SDK.tgz"
WIN32_SRC_ZIP_ARCHIVE_LINKNAME="CriScript-src-Win32.zip"
WIN32_SRC_TGZ_ARCHIVE_LINKNAME="CriScript-src-Win32.tgz"

# Options to Visual Studio
VS_OPTS="/verbosity:quiet /nologo"

# Commands with security implications we want to run
SCP='/bin/scp'
SSH='/bin/ssh'

#
# Functions
#
#
# Set the major.minor.revision.build number, once we have the source pulled down
# Used as base name of archive files we generate
# Expects a string of the form "M.m.r.b" (major.minor.revision.build)
# Generates filenames of the form "CriScript-M.m.r.b.XXX"
# set_archive_filenames(version_str)
set_archive_filenames()
{
    CRI_SCRIPT_VERSION="$1"

    # Source archive filename(s)
    ARCHIVE_BASE_FILENAME="CriScript-$CRI_SCRIPT_VERSION"
    SDK_ARCHIVE_BASE_FILENAME="$ARCHIVE_BASE_FILENAME.SDK"
    WIN32_SRC_ARCHIVE_BASE_FILENAME="$ARCHIVE_BASE_FILENAME.src.Win32"
    HTML_MANUAL_ARCHIVE_BASE_FILENAME="$ARCHIVE_BASE_FILENAME.html-doc"

    # .zip, .tgz versions
    SDK_ZIP_ARCHIVE_FILENAME="$ARCHIVE_DIR/$SDK_ARCHIVE_BASE_FILENAME.zip"
    SDK_TGZ_ARCHIVE_FILENAME="$ARCHIVE_DIR/$SDK_ARCHIVE_BASE_FILENAME.tgz"
    WIN32_SRC_ZIP_ARCHIVE_FILENAME="$ARCHIVE_DIR/$WIN32_SRC_ARCHIVE_BASE_FILENAME.zip"
    WIN32_SRC_TGZ_ARCHIVE_FILENAME="$ARCHIVE_DIR/$WIN32_SRC_ARCHIVE_BASE_FILENAME.tgz"

    # Just .tgz version of the HTML doc for now - just using it to transfer to server
    HTML_MANUAL_TGZ_ARCHIVE_FILENAME="$ARCHIVE_DIR/$HTML_MANUAL_ARCHIVE_BASE_FILENAME.tgz"
}

# Track build step; echo to output
# BUILD_STEP is referenced in build_error()
# build_step(step description)
build_step()
{
    BUILD_STEP=$1
    echo "[$BUILD_STEP]"
}

# Handle build errors of any kind (by telling someone, then bailing)
# build_error(message, error code, ...)
build_error()
{
    echo "* Build error encountered at [$BUILD_STEP]!"
    echo "* $1"
    echo "* Command returns [$2]"
    
    exit $2
}

# Called after each step to check exit code
# If not 0, will call build_error() (exiting the script)
# bail_on_error(exit code, message)
bail_on_error()
{
    if [ $1 -ne 0 ]; then

        # This will exit
        build_error "$2" $1

    fi
}

# Useful utility routine
die()
{
    echo "$*"
    exit;
}

#
# Entry
#


#
# Get and deal with any options
#

#
# Make sure we're in the right place
#
[ -d "$BUILD_DIR" ]	|| die "Build directory '$BUILD_DIR' does not exist!"
cd "$BUILD_DIR"		|| die "Cannot change to build directory '$BUILD_DIR'!"

#
# Clean out any old build files from before
#
build_step 'Cleaning build directories'

if [ -d "$SRC_PATH" ]; then
    rm -rf "$SRC_PATH"
    bail_on_error $? 'Removal of previous build directories failed'
fi

#
# Fetch source:
#
# Most of the tree will live in Subversion; some (mainly internal-use-only)
# things may live in Perforce.
#
build_step 'Getting source'

# Get source from subversion
svn co -q "svn://$SVN_REPOS_PATH" "$SRC_PATH" --username "$SVN_USER" --password "$SVN_PASSWORD" --no-auth-cache

# This will exit if $? != 0
bail_on_error $? 'Retrieval from Subversion failed'

# Get other stuff from Perforce

# Set the version properly; make appropriate archive filenames
version=`perl "$SRC_PATH/tools/bin/get_build_number.pl" "$SRC_PATH/inc/versions.h"`
echo "* Current CriScript version is [$version]"
set_archive_filenames $version

# Files seem to lose their executable bits
build_step 'Fixing executable permissions'

find . -name '*.exe' -exec chmod +x '{}' \;
find . -name '*.bat' -exec chmod +x '{}' \;

#
# Build, build
#

build_step 'Building libraries'
pushd "$SRC_PATH/build/win32/CRIScript"

MSBuild.exe CRIScript.sln /t:Rebuild /p:Configuration=Release $VS_OPTS
bail_on_error $? 'RELEASE compilation failed'

MSBuild.exe CRIScript.sln /t:Rebuild /p:Configuration=Debug $VS_OPTS
bail_on_error $? 'DEBUG compilation failed'

popd

build_step 'Building script compiler'
pushd "$SRC_PATH/tools/cscl"

# Full rebuild not needed - project depends on CriScript libs, which have 
# already been built - no need to build them again
MSBuild.exe cscl.sln /p:Configuration=Release $VS_OPTS
bail_on_error $? 'Script compiler build failed'

popd

build_step 'Building helpfile'
pushd "$SRC_PATH/docs/manual/makedocs"

./mkdoc2_e.sh
echo "OK - skipping DOC BUILD check..."

bail_on_error $? 'Documentation build failed'

popd

#
# Run test suite(s)
#
build_step 'Running Javascript test suite'
pushd "$SRC_PATH/test/jscript"

./test.bat release
bail_on_error $? 'Javascript test suite failed'

popd

#
# Copy files to proper places
#
build_step 'Organizing files'
pushd "$SRC_PATH"

[ -d "$LIBS_DIR" ]	|| mkdir -p "$LIBS_DIR"
[ -d "$DOCS_DIR" ]	|| mkdir -p "$DOCS_DIR"
[ -d "$TOOLS_DIR" ]	|| mkdir -p "$TOOLS_DIR"
cp build/win32/CRIScript/debug/CriScriptd.lib "$LIBS_DIR/CriScriptd.lib" && \
  cp build/win32/CRIScript/release/CriScript.lib "$LIBS_DIR/CriScript.lib" && \
  cp "$DOCS_DIR/manual/output/eng/html/CRI_Script_Manual_e.chm" "$DOCS_DIR/CRI_Script_Manual_e.chm" && \
  cp "$TOOLS_DIR/cscl/Release/cscl.exe" "$TOOLS_DIR/cscl.exe"
bail_on_error $? 'Copying of files failed'

popd

#
# Build distribution archives
#
build_step 'Building distribution archives'

# If we have to create the diretory, make sure others can write to it
[ -d "$ARCHIVE_DIR" ]	|| ( mkdir -p "$ARCHIVE_DIR" && chmod 777 "$ARCHIVE_DIR" )

# These files contains lists of files to add to the SDK and source distribution archives
# Comments start with a '/', section titles (which we can ignore) are in []
# Original file(s) came from Windows, so lose \r in line endings
grep -v '^[/[]' "$SDK_ARCHIVE_LIST" | tr '\\' '/' | tr -d '\r' > "$SDK_ARCHIVE_LIST.~tmp"
grep -v '^[/[]' "$WIN32_SRC_ARCHIVE_LIST" | tr '\\' '/' | tr -d '\r' > "$WIN32_SRC_ARCHIVE_LIST.~tmp"

cat "$SDK_ARCHIVE_LIST.~tmp" | zip -q "$SDK_ZIP_ARCHIVE_FILENAME" -@
bail_on_error $? 'Building ZIP SDK archive failed'

tar zcf "$SDK_TGZ_ARCHIVE_FILENAME" -T "$SDK_ARCHIVE_LIST.~tmp"
bail_on_error $? 'Building TGZ SDK archive failed'

cat "$WIN32_SRC_ARCHIVE_LIST.~tmp" | zip -q "$WIN32_SRC_ZIP_ARCHIVE_FILENAME" -@
bail_on_error $? 'Building ZIP Win32 source archive failed'

tar zcf "$WIN32_SRC_TGZ_ARCHIVE_FILENAME" -T "$WIN32_SRC_ARCHIVE_LIST.~tmp"
bail_on_error $? 'Building TGZ Win32 source archive failed'

# The HTML documentation
tar zcf "$HTML_MANUAL_TGZ_ARCHIVE_FILENAME" -C "$HTML_DOC_DIR" .
bail_on_error $? 'Building ZIP documentation archive failed'

#
# Increment build number (for next time through)
#
build_step 'Incrementing build number'
pushd "$SRC_PATH/tools/bin"
./countup_svn.sh
popd

#
# Copy archives to download server
# The remote side will backup the previous versions, make symlinks to 
# the new files (as 'current version' or similar), and any other
# housekeeping that may be necessary.
#

build_step 'Copying files to download server'

# Do the copy
$SCP "$SDK_ZIP_ARCHIVE_FILENAME" "$SDK_TGZ_ARCHIVE_FILENAME" "$WIN32_SRC_ZIP_ARCHIVE_FILENAME" "$WIN32_SRC_TGZ_ARCHIVE_FILENAME" "$HTML_MANUAL_TGZ_ARCHIVE_FILENAME" $CRISCRIPT_USER@$CRISCRIPT_SERVER:$CRISCRIPT_UL_PATH

# For each file, get it (without path) & put in the right place, with the proper alias
fn=`basename $SDK_ZIP_ARCHIVE_FILENAME`
$SSH $CRISCRIPT_USER@$CRISCRIPT_SERVER $CRISCRIPT_UL_CMD "$fn" $SDK_ZIP_ARCHIVE_LINKNAME

fn=`basename $SDK_TGZ_ARCHIVE_FILENAME`
$SSH $CRISCRIPT_USER@$CRISCRIPT_SERVER $CRISCRIPT_UL_CMD "$fn" $SDK_TGZ_ARCHIVE_LINKNAME

fn=`basename $WIN32_SRC_ZIP_ARCHIVE_FILENAME`
$SSH $CRISCRIPT_USER@$CRISCRIPT_SERVER $CRISCRIPT_UL_CMD "$fn" $WIN32_SRC_ZIP_ARCHIVE_LINKNAME

fn=`basename $WIN32_SRC_TGZ_ARCHIVE_FILENAME`
$SSH $CRISCRIPT_USER@$CRISCRIPT_SERVER $CRISCRIPT_UL_CMD "$fn" $WIN32_SRC_TGZ_ARCHIVE_LINKNAME

# Copy documentation; tell other side to unzip it in the proper place
fn=`basename $HTML_MANUAL_TGZ_ARCHIVE_FILENAME`
$SSH $CRISCRIPT_USER@$CRISCRIPT_SERVER $CRISCRIPT_UL_CMD -x "$fn" 'html-manual'

#
# Cleanup
#

build_step 'All done - cleaning up'
rm "$SDK_ARCHIVE_LIST.~tmp"
rm "$WIN32_SRC_ARCHIVE_LIST.~tmp"

