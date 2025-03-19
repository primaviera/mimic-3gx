#!/bin/sh

TITLE_ID="$1"
[ "$1" ] || TITLE_ID="00040000001B4E00"

AZAHAR_PATH="$HOME/.local/share/azahar-emu"
$(dirname "$0")/citra.sh $TITLE_ID $AZAHAR_PATH
