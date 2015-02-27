#!/bin/bash

# Locations
PATCHJM=PatchCl75_2014_11_24

if ! [ -a "/lib/cpp" ]; then
   echo -e "/lib/cpp does not exist, but is needed to compile Caml Light.\n\
   If gcc is not installed on your system, you are strongly advised to\n\
   install it before running this script again.\n\
   If gcc is installed on your system and you still see this error \n\
   (eg. on Arch Linux), then the C preprocessor (cpp) is usually found\n\
   elsewhere in the PATH (usually /usr/lib or /usr/bin).\n\
   Try to look for it in the following locations, then symlink it to\n\
   /lib/cpp : \n" $PATH
   exit 1
fi

echo "Current working dir: " $(pwd)

# Parse command line arguments
while :
do
    case "$1" in
    -n|--no-delete)
    NODELETE="true"
    shift
    ;;
    -d|--no-download)  
    NODOWNLOAD="true"
    shift
    ;;
    -s|--initial-config)  
    INITIALCONFIG="true"
    shift
    ;;
    *)
    break
    ;;
   esac
done

if [ ! "$NODOWNLOAD" = "true" ]; then
   echo "Downloading the source and the patch for Caml Light..."
   # Get the archives - source from the INRIA and patch from Jean Mouric
   wget http://caml.inria.fr/pub/distrib/caml-light-0.75//cl75unix.tar.gz >>./log.txt 2>&1
   wget http://jean.mouric.pagesperso-orange.fr/archives/$PATCHJM.zip >>./log.txt 2>&1
fi

# Extract them and apply the patch
tar -xf ./cl75unix.tar.gz >> ./log.txt 2>&1
unzip ./$PATCHJM.zip >> ./log.txt 2>&1
cd $PATCHJM
tar -xf ./dif.tar.gz >> ../log.txt 2>&1
cd ..
patch -s -p0  < ./$PATCHJM/dif.txt || exit 1
cd cl75
cd src

# One useful variable
LIBDIR=$(pwd)

# Workaround to avoid "make install"-ing
sed -i "s/LIBDIR=\/usr\/local\/lib\/caml-light/LIBDIR=$(pwd | sed "s/\//\\\\\//g")\/lib/g" ./Makefile

# Build Caml Light
echo "Building the core Caml system"
make configure >> ../../log.txt  2>&1
make world >>../../log.txt 2>&1
if [ $? == 0 ] ; then
   echo "Building succeeded"
else
   echo "Building failed!"
   exit 1
fi

# Another workaround to avoid "make install"-ing
PATH=$PATH:$(pwd):$(pwd)/launch:$(pwd)/tools
export PATH

# Make these two scripts executable, as would a "make install" do
chmod +x ./launch/camlc
chmod +x ./launch/camlmktop

# Copy some binaries to the lib dir, where they will be used to compile the next modules
cp ./camlcomp ./lib/ >>../../log.txt 2>&1
cp ./camllex ./lib/ >>../../log.txt 2>&1
cp ./camllibr ./lib/ >>../../log.txt 2>&1
cp ./camllink ./lib/ >>../../log.txt 2>&1
cp ./compiler/* ./lib/ >>../../log.txt 2>&1
cp ./runtime/* ./lib/ >>../../log.txt 2>&1
cp ./toplevel/* ./lib/ >>../../log.txt 2>&1
cd ..

# Optional packages are in the "contrib" subdir
cd contrib

# Optional packages built
APPEND=""
CCOPT=""
SUMMARY=""

# Build the "unix" package, required for the "graphics" package below
echo "Building libunix"
cd libunix
make >> ../../log.txt 2>&1
if [ $? == 0 ] && [ -a "./libunix.a" ]; then
   cp ./unix.zi ../../src/lib/
   cp ./unix.zo ../../src/lib/
   cp ./*.a ../../src/lib/
   UNIXBUILT="true"
   APPEND=$APPEND" unix.zo libunix.a"
   CCOPT=$CCOPT"-ccopt -L/usr/lib -lpthread"
   echo "Built the Unix package"
else
   echo "Building failed!"
   UNIXBUILT="false"
   SUMMARY=$SUMMARY"Building the Unix package failed - could not build the graphics package either.\n"
fi
cd ..

# Build the "graphics" package
if [ $UNIXBUILT = "true" ]; then
   echo "Building the graphics library"
   cd libgraph-unix
   make >> ../../log.txt 2>&1
   if [ $? == 0 ] && [ -a "./libgraph.a" ]; then
      cp ./*.zi ../../src/lib/
      cp ./*.zo ../../src/lib/
      cp ./*.a ../../src/lib/
      APPEND=$APPEND" graphics.zo libgraph.a"
      CCOPT=$CCOPT" -lX11"
      echo "Built the graphics package"
   else
      echo "Building failed!"
      SUMMARY=$SUMMARY"Building the graphics package failed, yet the unix library has been successfully built.\n"
   fi
   cd ..
fi

# Build the "num" package
echo "Building the num package"
cd libnum
sed -i "s/CAMLLIBR=camllibr/CAMLLIBR=camlrun $(echo $LIBDIR | sed "s/\//\\\\\//g")\/camllibr/g" ./Makefile
make >> ../../log.txt 2>&1
if [ $? == 0 ] && [ -a "./libnums.a" ]; then
   cp ./*.zi ../../src/lib/
   cp ./*.zo ../../src/lib/
   cp ./*.a ../../src/lib/
   APPEND=$APPEND" int_misc.zo fnat.zo nat.zo big_int.zo arith_flags.zo ratio.zo num.zo arith_status.zo numprint.zo libnums.a"
   echo "Built the num package"
else
   echo "Building failed!"
   SUMMARY=$SUMMARY"Building the num package failed.\n"
fi
cd ..


# Build the "str" package
echo "Building the str package"
cd libstr
make >> ../../log.txt 2>&1
if [ $? == 0 ] && [ -a "./libstr.a" ]; then
   cp ./*.zi ../../src/lib/
   cp ./*.zo ../../src/lib/
   cp ./*.a ../../src/lib/
   APPEND=$APPEND" str.zo libstr.a"
   echo "Built the str package"
else
   echo "Building failed!"
   SUMMARY=$SUMMARY"Building the str package failed.\n"
fi
cd ..

# Build the debugger
echo "Build the debugger"
cd debugger
sed -i "s/CAMLLEX=camllex/CAMLLEX=camlrun $(echo $LIBDIR | sed "s/\//\\\\\//g")\/camllex/g" ./Makefile
make >> ../../log.txt 2>&1
if [ $? == 0 ]; then
   cp ./*.zi ../../src/lib/
   cp ./*.zo ../../src/lib/
   echo "Built the debugger"
else
   echo "Building failed!"
   SUMMARY=$SUMMARY"Building the debugger failed.\n"
fi
cd ..


cd ..

# Build a toplevel with the compiled libs
echo "Making a toplevel with the compiled libs"
cd src
cd lib
camlmktop -o CamlLightToplevel -custom $APPEND $CCOPT >> ../../../log.txt 2>&1
if [ $? == 0 ]; then
   echo "Toplevel created"
else
   echo "Failed creating the toplevel; there may be missing files. We will stop there."
   cd ..
   cd ..
   cd ..
   if [ ! "$NODELETE" = "true" ]; then
      echo "Deleting the source files"
      # Delete the source archives and the dirs
      rm cl75unix.tar.gz
      rm -rf ./cl75
      rm $PATCHJM.zip
      rm -rf $PATCHJM
      rm archi.txt
   fi
   exit 1
fi
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

if [ ! "$NODELETE" = "true" ]; then
   echo "Deleting the source files"
   # Delete the source archives and the dirs
   rm cl75unix.tar.gz
   rm -rf ./cl75
   rm $PATCHJM.zip
   rm -rf $PATCHJM
   rm archi.txt
fi

if [ "$INITIALCONFIG" = "true" ]; then
   echo "Doing initial configuration for LemonCaml"
   # Configure LemonCaml
   if [ ! -e  ~/.config/Cocodidou ]; then
      mkdir ~/.config/Cocodidou
   fi
   echo "[%General]" > ~/.config/Cocodidou/LemonCaml.conf
   echo "camlPath=$(pwd)/caml/CamlLightToplevel" >> ~/.config/Cocodidou/LemonCaml.conf
   echo "keywordspath=$(pwd)/keywords" >> ~/.config/Cocodidou/LemonCaml.conf
   echo "stdlibPath=$(pwd)/caml/lib" >> ~/.config/Cocodidou/LemonCaml.conf
   echo "treeModelsPath=$(pwd)/gentree" >> ~/.config/Cocodidou/LemonCaml.conf
   echo "Configured LemonCaml."
fi

if [ "$SUMMARY" = "" ]; then
   echo "Everything went right, Caml should be up and running."
else
   echo -e $SUMMARY
fi

exit 0