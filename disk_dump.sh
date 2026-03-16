#!/usr/bin/env bash
# dump_hd_1k.sh
# Dump a disk image: first 512 bytes as MBR, then 1KB blocks from offset 512 onward.
#
# Usage: ./dump_hd_1k.sh <image_file> <output_dir>

set -euo pipefail

# ── Argument validation ────────────────────────────────────────────────────────
if [[ $# -ne 2 ]]; then
    echo "[ERROR] Usage: $0 <image_file> <output_dir>" >&2
    exit 1
fi

IMG="$1"
OUTDIR="$2"
MBR_SIZE=512
BLOCK_SIZE=1024

if [[ ! -f "$IMG" ]]; then
    echo "[ERROR] Image file not found: $IMG" >&2
    exit 1
fi

if [[ ! -d "$OUTDIR" ]]; then
    echo "[ERROR] Output directory does not exist: $OUTDIR" >&2
    echo "[ERROR] Please create it manually before running this script." >&2
    exit 1
fi

# ── Image info ─────────────────────────────────────────────────────────────────
FILE_SIZE=$(stat -c%s "$IMG")

if [[ $FILE_SIZE -le $MBR_SIZE ]]; then
    echo "[ERROR] Image too small (${FILE_SIZE} bytes). Must be larger than ${MBR_SIZE} bytes." >&2
    exit 1
fi

DATA_SIZE=$(( FILE_SIZE - MBR_SIZE ))
TOTAL_BLOCKS=$(( (DATA_SIZE + BLOCK_SIZE - 1) / BLOCK_SIZE ))

echo "=================================================="
echo " Image      : $IMG"
echo " Size       : $FILE_SIZE bytes  ($(( FILE_SIZE / 1024 / 1024 )) MB)"
echo " MBR        : offset 0x00000000, 512 bytes"
echo " Data start : offset 0x00000200"
echo " Blocks     : $TOTAL_BLOCKS x 1KB"
echo " Output dir : $OUTDIR"
echo "=================================================="

# ── MBR dump ──────────────────────────────────────────────────────────────────
MBR_FILE="$OUTDIR/mbr_0x00000000.xxd"
xxd -s 0 -l "$MBR_SIZE" "$IMG" > "$MBR_FILE"
echo "[MBR]  Saved -> $(basename "$MBR_FILE")"

# ── 1KB block dump ────────────────────────────────────────────────────────────
START_TIME=$(date +%s)

for (( i=0; i<TOTAL_BLOCKS; i++ )); do
    OFFSET=$(( MBR_SIZE + i * BLOCK_SIZE ))
    OUTFILE=$(printf "%s/block_%05d_0x%08X.xxd" "$OUTDIR" "$i" "$OFFSET")
    xxd -s "$OFFSET" -l "$BLOCK_SIZE" "$IMG" > "$OUTFILE"

    if (( (i + 1) % 100 == 0 || i + 1 == TOTAL_BLOCKS )); then
        PCT=$(( (i + 1) * 100 / TOTAL_BLOCKS ))
        printf "\r  Progress: [%-40s] %d%% (%d/%d)" \
            "$(printf '#%.0s' $(seq 1 $((PCT * 40 / 100))))" \
            "$PCT" "$((i+1))" "$TOTAL_BLOCKS"
    fi
done

echo ""

END_TIME=$(date +%s)
ELAPSED=$(( END_TIME - START_TIME ))

echo ""
echo "=================================================="
echo " Done!"
echo " MBR file   : 1"
echo " Block files: $TOTAL_BLOCKS"
echo " Elapsed    : ${ELAPSED}s"
echo "=================================================="

# ── Index file ────────────────────────────────────────────────────────────────
INDEX="$OUTDIR/INDEX.txt"
{
    echo "# disk image xxd dump index"
    echo "# Generated : $(date)"
    echo "# Image     : $IMG ($FILE_SIZE bytes)"
    echo "# MBR       : 512 bytes at offset 0x00000000"
    echo "# Blocks    : $TOTAL_BLOCKS x ${BLOCK_SIZE}B from offset 0x00000200"
    echo "#"
    printf "%-6s  %-12s  %-12s  %s\n" "ENTRY" "OFFSET(dec)" "OFFSET(hex)" "FILE"
    echo "------  ------------  ------------  -------------------------"
    printf "%-6s  %-12d  0x%08X    %s\n" "MBR" 0 0 "$(basename "$MBR_FILE")"
    for (( i=0; i<TOTAL_BLOCKS; i++ )); do
        OFFSET=$(( MBR_SIZE + i * BLOCK_SIZE ))
        FNAME=$(printf "block_%05d_0x%08X.xxd" "$i" "$OFFSET")
        printf "%-6d  %-12d  0x%08X    %s\n" "$i" "$OFFSET" "$OFFSET" "$FNAME"
    done
} > "$INDEX"

echo " Index file : $INDEX"
echo "=================================================="
