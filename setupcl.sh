#!/bin/bash

# Get the archives - source from the INRIA and patch from Jean Mouric
wget http://caml.inria.fr/pub/distrib/caml-light-0.75//cl75unix.tar.gz
wget http://jean.mouric.pagesperso-orange.fr/archives/PatchCl75_2014_04_30.zip

# Extract them and apply the patch
tar -xf ./cl75unix.tar.gz
unzip ./PatchCl75_2014_04_30.zip
cd PatchCl75_2014_04_30
tar -xf ./dif.tar.gz
cd ..
patch -s -p0  < ./PatchCl75_2014_04_30/dif.txt || exit 1
cd cl75
cd src

# One useful variable
LIBDIR=$(pwd)

# Workaround to avoid "make install"-ing
sed -i "s/LIBDIR=\/usr\/local\/lib\/caml-light/LIBDIR=$(pwd | sed "s/\//\\\\\//g")\/lib/g" ./Makefile

# Build Caml Light
make configure
make world

# Another workaround to avoid "make install"-ing
PATH=$PATH:$(pwd):$(pwd)/launch:$(pwd)/tools
export PATH

# Make these two scripts executable, as would a "make install" do
chmod +x ./launch/camlc
chmod +x ./launch/camlmktop

# Copy some binaries to the lib dir, where they will be used to compile the next modules
cp ./camlcomp ./lib/
cp ./camllex ./lib/
cp ./camllibr ./lib/
cp ./camllink ./lib/
cp ./runtime/* ./lib/
cp ./toplevel/* ./lib/
cd ..

# Optional packages are in the "contrib" subdir
cd contrib

# Optional packages built
APPEND=""
CCOPT=""
SUMMARY=""

# Build the "unix" package, required for the "graphics" package below
cd libunix
make
if [ -a "./libunix.a" ]; then
   cp ./unix.zi ../../src/lib/
   cp ./unix.zo ../../src/lib/
   cp ./*.a ../../src/lib/
   UNIXBUILT="true"
   APPEND=$APPEND" unix.zo libunix.a"
   CCOPT=$CCOPT"-ccopt -L/usr/lib -lX11 -lpthread"
   echo "Built the Unix package"
else
   SUMMARY=$SUMMARY"Building the Unix package failed - could not build the graphics package either\n"
fi
cd ..

# Build the "graphics" package
if [ $UNIXBUILT = "true" ]; then
   cd libgraph-unix
   make
   if [ -a "./libgraph.a" ]; then
      cp ./*.zi ../../src/lib/
      cp ./*.zo ../../src/lib/
      cp ./*.a ../../src/lib/
      APPEND=$APPEND" graphics.zo libgraph.a"
      echo "Built the graphics package"
   else
      SUMMARY=$SUMMARY"Building the graphics package failed, yet the unix library has been successfully built\n"
   fi
   cd ..
fi

# Build the "num" package
cd libnum
sed -i "s/CAMLLIBR=camllibr/CAMLLIBR=camlrun $(echo $LIBDIR | sed "s/\//\\\\\//g")\/camllibr/g" ./Makefile
make
if [ -a "./libnums.a" ]; then
   cp ./*.zi ../../src/lib/
   cp ./*.zo ../../src/lib/
   cp ./*.a ../../src/lib/
   APPEND=$APPEND" int_misc.zo fnat.zo nat.zo big_int.zo arith_flags.zo ratio.zo num.zo arith_status.zo numprint.zo libnums.a"
   echo "Built the num package"
else
   SUMMARY=$SUMMARY"Building the num package failed\n"
fi
cd ..


# Build the "str" package
cd libstr
make
if [ -a "./libstr.a" ]; then
   cp ./*.zi ../../src/lib/
   cp ./*.zo ../../src/lib/
   cp ./*.a ../../src/lib/
   APPEND=$APPEND" str.zo libstr.a"
   echo "Built the str package"
else
   SUMMARY=$SUMMARY"Building the str package failed\n"
fi
cd ..

cd ..

# Build a toplevel with the compiled libs
cd src
cd lib
camlmktop -o CamlLightToplevel -custom $APPEND $CCOPT
cd ..
cd ..

cd ..

# Copy the built files and the toplevel
mkdir caml
cd caml
mkdir lib
cd ..
cp ./cl75/src/lib/*.zi ./caml/lib/
cp ./cl75/src/lib/*.zo ./caml/lib/
cp ./cl75/src/lib/CamlLightToplevel ./caml/
cd caml
chmod +x ./CamlLightToplevel
cd ..

# Delete the source archives and the dirs
rm cl75unix.tar.gz
rm -rf ./cl75
rm PatchCl75_2014_04_30.zip
rm -rf rm PatchCl75_2014_04_30
rm archi.txt

# Configure LemonCaml
if [ ! -e  ~/.config/Cocodidou ]; then
   mkdir ~/.config/Cocodidou
fi
echo "[%General]" > ~/.config/Cocodidou/LemonCaml.conf
echo "camlPath=$(pwd)/caml/CamlLightToplevel" >> ~/.config/Cocodidou/LemonCaml.conf
echo "keywordspath=$(pwd)/keywords" >> ~/.config/Cocodidou/LemonCaml.conf
echo "stdlibPath=$(pwd)/caml/lib" >> ~/.config/Cocodidou/LemonCaml.conf

if [ "$SUMMARY" = "" ]; then
   echo "Everything went right, Caml should be up and running."
else
   echo $SUMMARY
fi