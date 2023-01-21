#!/bin/sh

cd /mnt/ext1/applications/pbdoom || exit

./game.app 2>&1 | tee game.log