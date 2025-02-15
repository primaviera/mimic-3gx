#!/bin/sh
set -x

title_id="$1"
[ "$1" ] || title_id="00040000001B4E00"

make re || exit 1

mkdir -p ~/.local/share/citra-emu/sdmc/luma/plugins/$title_id
cp *.3gx ~/.local/share/citra-emu/sdmc/luma/plugins/$title_id
