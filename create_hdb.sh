#!/bin/bash
set -e

IMG="hdb.img"
MOUNT_POINT="/mnt/hdb"
PARTITION_START=20
PARTITION_SIZE=81900
PARTITION_OFFSET=$((PARTITION_START * 512))  # 10240

# Check if image already exists
if [ -f "$IMG" ]; then
    echo "[!] $IMG already exists. Overwrite? (y/n)"
    read -r answer
    if [ "$answer" != "y" ]; then
        echo "[-] Aborted."
        exit 1
    fi
fi

# 1. Create blank image
echo "[*] Creating 40MB blank image..."
dd if=/dev/zero of="$IMG" bs=1M count=40 status=progress
echo "[+] Image created"

# 2. Create partition table
echo "[*] Writing MBR partition table..."
echo "start=$PARTITION_START, size=$PARTITION_SIZE, type=83" | sfdisk -X dos "$IMG"
echo "[+] Partition created"

# 3. Attach to available loop device
echo "[*] Setting up loop device..."
LOOP_DEV=$(losetup -f)
losetup -o "$PARTITION_OFFSET" "$LOOP_DEV" "$IMG"
echo "[+] Attached to $LOOP_DEV (offset=$PARTITION_OFFSET)"

# 4. Create Minix v1 filesystem
echo "[*] Creating Minix v1 filesystem..."
mkfs.minix -n 14 "$LOOP_DEV"
echo "[+] Minix filesystem created"

# 5. Mount
echo "[*] Mounting..."
mkdir -p "$MOUNT_POINT"
mount -t minix "$LOOP_DEV" "$MOUNT_POINT"
echo "[+] Mounted at $MOUNT_POINT"

echo ""
echo "=================================="
echo " Done. Copy your files to $MOUNT_POINT"
echo " Loop device: $LOOP_DEV"
echo ""
echo " When finished:"
echo "   sudo umount $MOUNT_POINT"
echo "   sudo losetup -d $LOOP_DEV"
echo "=================================="