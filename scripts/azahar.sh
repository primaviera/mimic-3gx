#!/bin/sh
set -e

TITLE_ID="$1"
[ "$1" ] || TITLE_ID="00040000001B4E00"

AZAHAR_PATH="$HOME/.var/app/org.azahar_emu.Azahar/data/azahar-emu"
$(dirname "$0")/citra.sh $TITLE_ID $AZAHAR_PATH
