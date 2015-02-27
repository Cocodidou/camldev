#!/bin/bash 

DESTDIR="/etc/lemoncaml"

echo "Building the Caml Light toplevel"
./setupcl.sh
if [ $? == 0 ]; then
   echo "The Caml Light toplevel has been successfully compiled."
   echo "Now compiling LemonCaml."
   qmake
   make >> ./log.txt 2>&1
   if [ $? == 0 ]; then
      echo "LemonCaml has been successfully built."
      echo "Now installing all the files."
      sudo make install
      if ! [ $? == 0 ]; then
         echo "Installing failed, make sure you have the necessary rights."
         exit
      fi
      sudo mkdir $DESTDIR
      sudo cp -rf ./caml $DESTDIR
      sudo cp ./keywords $DESTDIR
      sudo cp ./lemoncaml_*.qm $DESTDIR
      sudo cp -rf ./gentree $DESTDIR
      sudo mkdir /etc/xdg/Cocodidou
      echo "[%General]" > ./LemonCaml.conf
      echo "camlPath=$DESTDIR/caml/CamlLightToplevel" >> ./LemonCaml.conf
      echo "keywordspath=$DESTDIR/keywords" >> ./LemonCaml.conf
      echo "camlArgs= -stdlib \"$DESTDIR/caml/lib\"" >> ./LemonCaml.conf
      echo "treeModelsPath=$DESTDIR/gentree" >> ./LemonCaml.conf
      echo "setupPath=$DESTDIR/" >> ./LemonCaml.conf
      sudo cp ./LemonCaml.conf /etc/xdg/Cocodidou
      echo "A little bit of cleaning..."
      make clean >> ./log.txt 2>&1
      echo "Configuration done!"
   else
      echo "LemonCaml building has failed. Exiting there."
      exit 1
   fi
else
   echo "The Caml Light system building has failed. Exiting there."
   exit 1
fi