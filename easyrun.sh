#!/bin/bash
if [[ $# != 1 ]] ; then
    echo 'ERROR: You need to give exactly 1 parameter to this script.
       The parameter should be a filename of a parameter file in /PARAM_FILES

       Please try again

          '

    exit 0
fi

rm Parameters.hpp
cp $1 PARAM_FILES/Parameters.hpp
mv -i PARAM_FILES/Parameters.hpp .
make clean
make && clear && ./Simulator
exit 0
