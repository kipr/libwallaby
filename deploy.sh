#!/bin/bash
set -e

# --- Configuration ---
PI_IP="192.168.125.1"
DEB_FILE="build/libkipr-1.1.0-Linux.deb"
PI_USER="pi"

# --- Build ---
docker buildx build --platform linux/arm64 -t wallaby-build --load .

# --- Deployment ---
echo "Deploying to Raspberry Pi at $PI_IP..."

# Copy the .deb file to the Raspberry Pi
scp "$DEB_FILE" "$PI_USER@$PI_IP:/tmp/"

# Install the .deb file on the Raspberry Pi
ssh "$PI_USER@$PI_IP" "sudo dpkg -i /tmp/$(basename $DEB_FILE)"

echo "Deployment complete!"
