#!/bin/sh

# upload files to remote

LOCAL_FILES=./*
REMOTE_DIR=/root/.
REMOTE_IP=192.168.0.104
REMOTE_USERNAME=root
REMOTE_PASSWORD=admin

echo "uploading files..."

sshpass -p $REMOTE_PASSWORD scp -r * $REMOTE_USERNAME@$REMOTE_IP:$REMOTE_DIR

if [ $? -eq 0 ];then
    echo "upload done"
else
    echo "upload failed"
    exit 1
fi

sshpass -p admin ssh root@192.168.0.104 df -h

