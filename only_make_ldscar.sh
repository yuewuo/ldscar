#!/bin/sh

make package/ldscar/compile V=s

if [ $? -eq 0 ];then
    echo "compile done"
else 
    echo "compile failed"
    exit 1
fi

