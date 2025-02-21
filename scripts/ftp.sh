#!/bin/sh
set -x

_3DS_IP=$1
TITLE_ID=$2
[ "$1" ] || _3DS_IP="192.168.15.151:5000"
[ "$2" ] || TITLE_ID="00040000001B4E00"

[ -f mimic-3gx.3gx ] || { make || exit 1; }
[ -d romfs ] && { find romfs -type f -exec curl --ftp-create-dirs -T {} ftp://${_3DS_IP}/luma/titles/${TITLE_ID}/{} \; ; }
[ -f data/config.toml ] && { curl --ftp-create-dirs -T data/config.toml ftp://${_3DS_IP}/config/mimic-3gx/config.toml; }

curl --ftp-create-dirs -T mimic-3gx.3gx ftp://${_3DS_IP}/luma/plugins/${TITLE_ID}/mimic-3gx.3gx

