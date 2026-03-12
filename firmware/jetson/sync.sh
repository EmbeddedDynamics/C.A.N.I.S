#!/usr/bin/env bash
# Sync Jetson scripts and sounds to the Jetson Orin Nano Super.
# Usage: bash sync.sh
# Run from anywhere — uses the script's own directory as source.

read -rp "Jetson IP: " IP
REMOTE="perijn@$IP"
DEST="/home/perijn"
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "Syncing Jetson files to $REMOTE:$DEST ..."
rsync -avz --exclude='sync.sh' --exclude='.git' "$DIR/" "$REMOTE:$DEST/"
echo "Done."
