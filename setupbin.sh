#!/bin/bash 

DESTDIR="/etc/lemoncaml"

echo "Installing the binary files..."
sudo cp ./lemoncaml /usr/bin
if [ $? == 0 ]; then
   sudo mkdir $DESTDIR
   sudo cp -rf ./caml $DESTDIR
   sudo cp ./keywords $DESTDIR
   sudo cp ./lemoncaml_*.qm $DESTDIR
   sudo mkdir /etc/xdg/Cocodidou
   echo "[%General]" > ./LemonCaml.conf
   echo "camlPath=$DESTDIR/caml/CamlLightToplevel" >> ./LemonCaml.conf
   echo "keywordspath=$DESTDIR/keywords" >> ./LemonCaml.conf
   echo "camlArgs= -stdlib \"$DESTDIR/caml/lib\"" >> ./LemonCaml.conf
   echo "treeModelsPath=$DESTDIR/gentree" >> ./LemonCaml.conf
   echo "setupPath=$DESTDIR/" >> ./LemonCaml.conf
   sudo cp ./LemonCaml.conf /etc/xdg/Cocodidou
   echo "Configuration and installation done!"
else
   echo "Unable to install the LemonCaml binary files (make sure you have the right to write to /etc, /usr/bin). Exiting there."
   exit 1
fi

exit 0