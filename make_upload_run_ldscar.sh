#!/bin/sh

REMOTE_IP=192.168.0.104
LOCAL_FILE=bin/ramips/packages/base/ldscar_*
REMOTE_USERNAME=root
REMOTE_PASSWORD=admin
REMOTE_DIR=/root/.
INSTALL_FILE=/root/ldscar_*

make package/ldscar/compile V=s

if [ $? -eq 0 ];then
    echo "compile done"
else 
    echo "compile failed"
    exit 1
fi

echo "uploading ipk..."

sshpass -p $REMOTE_PASSWORD scp $LOCAL_FILE $REMOTE_USERNAME@$REMOTE_IP:$REMOTE_DIR

if [ $? -eq 0 ];then
    echo "upload done"
else
    echo "upload failed"
    exit 2
fi

sshpass -p admin ssh root@$REMOTE_IP opkg install $INSTALL_FILE

sshpass -p admin ssh root@$REMOTE_IP df -h

echo ""
echo "run ldscar..."
echo ""

sshpass -p admin ssh root@$REMOTE_IP ldscar

