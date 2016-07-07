#!/bin/sh                                                                                                                                                                                    

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

test_and_add_bash()
{
    if [ -e $1 ] ; then
        echo "" >> $1
        echo "export LD_LIBRARY_PATH=\"$DIR/libs\"" >> $1
    fi
}

cd dependancies
cd FreeImage
echo "Compiling FreeImage..."
make
cd ..
cd OIS
echo "Compiling OIS ..."
./configure
make
cp -rf src/.libs/libOIS* ../../libs/.
cd ..
echo "Compiling Ogre ..."
cd Ogre
mkdir Dependencies/
mkdir Dependencies/include/
mkdir Dependencies/Debug/
mkdir Dependenies/Release/
cp ../FreeImage/Source/FreeImage.h Dependencies/include/.
cp ../FreeImage/libfreeimage* Dependencies/Debug/.
cp ../FreeImage/libfreeimage* Dependencies/Debug/.
cmake .
make -j 2
cp -rf lib/* ../../libs/.
echo "Compiling CEGUI ..."
cd ../CEGUI/
cmake .
make -j 2
cp -rf lib/* ../../libs/.
cd ..
echo "Compiling jsoncpp ..."
cd jsoncpp
cmake .
make
cp src/lib_json/libjsoncpp.a ../../libs/.
cd ../../

export LD_LIBRARY_PATH=./libs/:$LD_LIBRARY_PATH

ln -fs $DIR/libs/libsfml-audio.so $DIR/libs/libsfml-audio.so.2.3 &&\
ln -fs $DIR/libs/libsfml-graphics.so $DIR/libs/libsfml-graphics.so.2.3 &&\
ln -fs $DIR/libs/libsfml-network.so $DIR/libs/libsfml-network.so.2.3 && \
ln -fs $DIR/libs/libsfml-system.so $DIR/libs/libsfml-system.so.2.3 &&\
ln -fs $DIR/libs/libsfml-window.so $DIR/libs/libsfml-window.so.2.3

test_and_add_bash ~/.bashrc
