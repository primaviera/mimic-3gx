#!/bin/sh
set -x

TITLE_ID="$1"
CITRA_PATH="$2"
[ "$1" ] || TITLE_ID="00040000001B4E00"
[ "$2" ] || CITRA_PATH="$HOME/.local/share/citra-emu"

ROMFS_PATH="$CITRA_PATH/load/mods/$TITLE_ID/"
PLUGIN_PATH="$CITRA_PATH/sdmc/luma/plugins/$TITLE_ID/"
CONFIG_PATH="$CITRA_PATH/sdmc/config/mimic-3gx/"

[ -f *.3gx ] || { make || exit 1; }
[ -d romfs ] && { mkdir -p $ROMFS_PATH && cp -r romfs $ROMFS_PATH; }
[ -f data/config.toml ] && { mkdir -p $CONFIG_PATH && cp data/config.toml $CONFIG_PATH; }

mkdir -p $PLUGIN_PATH
cp *.3gx $PLUGIN_PATH
