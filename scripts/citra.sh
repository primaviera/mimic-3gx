#!/bin/sh
set -e

cd $(dirname "$0") && cd ..

TITLE_ID="$1"
CITRA_PATH="$2"
[ "$1" ] || TITLE_ID="00040000001B4E00"
[ "$2" ] || CITRA_PATH="$HOME/.local/share/citra-emu"

CONFIG_PATH="$CITRA_PATH/sdmc/config/mimic-3gx/"
PLUGIN_PATH="$CITRA_PATH/sdmc/luma/plugins/$TITLE_ID/"
ROMFS_PATH="$CITRA_PATH/load/mods/$TITLE_ID/"

[ -f *.3gx ] || { make || exit 1; }
[ -f config.ini ] && { mkdir -p $CONFIG_PATH && cp config.ini $CONFIG_PATH; }
[ -d romfs ] && { mkdir -p $ROMFS_PATH && cp -r romfs $ROMFS_PATH; }

mkdir -p $PLUGIN_PATH
cp *.3gx $PLUGIN_PATH

echo "Copied files to emulator, exiting..."
