#!/bin/bash

# NAME: copy-boost-files.sh
# AUTHOR: Barbara Frewen
# DATE: February 18, 2010
# DESCRIPTION: Copy the minimum number of files from the boost
#   distribution for building the program_options library into
#   the current directory.

usage="USAGE: copy-boost-files.sh <path to boost package>"
usage2="NOTE: Moves files into cwd.  Run from BiblioSpec/src/extern/program_options/boost_<version>"

if [ $# != 1 ]
then
    echo $usage
    echo $usage2
    exit;
fi

bpath=$1

echo "Moving files from $bpath"

cp $bpath/bootstrap.sh .
cp $bpath/Jamroot .
cp $bpath/boost-build.jam .

mkdir tools
cp -r $bpath/tools/jam tools
cp -r $bpath/tools/build tools

mkdir boost
cp -r $bpath/boost/program_options boost
cp -r $bpath/boost/function boost
cp -r $bpath/boost/config boost
cp -r $bpath/boost/type_traits boost
cp -r $bpath/boost/detail boost
cp -r $bpath/boost/mpl boost
cp -r $bpath/boost/utility boost
cp -r $bpath/boost/bind boost
cp -r $bpath/boost/preprocessor boost
cp -r $bpath/boost/smart_ptr boost
cp -r $bpath/boost/iterator boost
cp -r $bpath/boost/exception boost
cp $bpath/boost/*hpp boost

mkdir libs
cp -r $bpath/libs/program_options libs
cp -r $bpath/libs/detail libs
