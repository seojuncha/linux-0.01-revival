#!/bin/bash

set -e

LOOP_DEV=""

cleanup() {
    echo "[!] Cleaning up..."
    umount "$MOUNT_POINT" 2>/dev/null || true
    losetup -d "$LOOP_DEV" 2>/dev/null || true
}
trap cleanup ERR EXIT

if [ "$(id -u)" -ne 0 ]; then
    echo "[-] This script must be run as root."
    exit 1
fi

IMG="hdb.img"
MOUNT_POINT="/mnt/hdb"
ROOT_MOUNT_POINT=$MOUNT_POINT/root
BIN_MOUNT_POINT=$MOUNT_POINT/bin
DEV_MOUNT_POINT=$MOUNT_POINT/dev
TTY=$DEV_MOUNT_POINT/tty
TTY0=$DEV_MOUNT_POINT/tty0
PARTITION_START=20
PARTITION_SIZE=81900   # sector size = 512B
PARTITION_OFFSET=$((PARTITION_START * 512))  # 10240

if [ -f "$IMG" ]; then
    echo "[!] $IMG already exists. Overwrite? (y/n)"
    read -r answer
    if [ "$answer" != "y" ]; then
        echo "[-] Aborted."
        exit 1
    fi
fi

if mountpoint -q "$MOUNT_POINT"; then
    echo "[-] $MOUNT_POINT is already mounted. Unmount first."
    exit 1
fi

echo ""
echo "[*] Creating 40MB blank image..."
dd if=/dev/zero of="$IMG" bs=1M count=40 status=progress
echo ""
echo "[+] Image created"

echo ""
echo "[*] Writing MBR partition table..."
echo "start=$PARTITION_START, size=$PARTITION_SIZE, type=83" | sfdisk -X dos "$IMG"
echo ""
echo "[+] Partition created"
echo "[+]    Layout: "
sfdisk -l "$IMG"

echo ""
echo "[*] Setting up loop device..."
LOOP_DEV=$(losetup -f)
losetup -o "$PARTITION_OFFSET" "$LOOP_DEV" "$IMG"
echo ""
echo "[+] Attached to $LOOP_DEV (offset=$PARTITION_OFFSET)"
losetup -l "$LOOP_DEV"

echo ""
echo "[*] Creating Minix v1 filesystem..."
mkfs.minix -n 14 "$LOOP_DEV"
echo ""
echo "[+] Minix filesystem created"

echo ""
echo "[*] Mounting..."
echo ""
mkdir -p "$MOUNT_POINT"
mount -t minix "$LOOP_DEV" "$MOUNT_POINT"
echo ""
echo "[+] Mounted at $MOUNT_POINT"
echo "[+] Mount status:"
findmnt "$MOUNT_POINT"

echo ""
echo "[*] Creating directories..."
echo ""
mkdir -p "$ROOT_MOUNT_POINT"
mkdir -p "$DEV_MOUNT_POINT"
mkdir -p "$BIN_MOUNT_POINT"
echo ""
echo "[+] Directory structure:"
ls -lh "$MOUNT_POINT"

echo ""
echo "[*] Extracting shell and utilities..."
echo "[*]  Extracting sh.tar.gz..."
tar -xzf sh.tar.gz -C "$BIN_MOUNT_POINT"
echo ""
echo "[*]  Extracting commands.tar.gz..."
tar -xzf commands.tar.gz -C "$BIN_MOUNT_POINT"
echo ""
echo "[+] Contents of $BIN_MOUNT_POINT:"
ls -lh "$BIN_MOUNT_POINT"

echo ""
echo "[*] Creating device files..."
echo ""
mknod "$TTY" c 5 0  # Character Device, Major=5, Minor=0
chmod 755 "$TTY"
mknod "$TTY0" c 4 0 # Character Device, Major=4, Minor=0
chmod 755 "$TTY0"
echo ""
echo "[+] Device files in $DEV_MOUNT_POINT"
ls -la "$DEV_MOUNT_POINT"

CURRENT_USER=$(logname 2>/dev/null || echo "${SUDO_USER:-$USER}")
chown "$CURRENT_USER":"$CURRENT_USER" "$IMG"
echo "[+] Ownership of $IMG transferred to $CURRENT_USER"

echo ""
echo "══════════════════════════════════════════"
echo "  Build Complete"
echo "══════════════════════════════════════════"
echo "  Image       : $IMG  ($(du -h "$IMG" | cut -f1))"
echo "  Loop device : $LOOP_DEV"
echo "  Offset      : $PARTITION_OFFSET bytes  (sector $PARTITION_START)"
echo "  Mount point : $MOUNT_POINT"
echo ""
echo "  Disk usage:"
df -h "$MOUNT_POINT"
echo ""
echo "  When finished:"
echo "    sudo umount $MOUNT_POINT"
echo "    sudo losetup -d $LOOP_DEV"
echo "══════════════════════════════════════════"

trap - EXIT
