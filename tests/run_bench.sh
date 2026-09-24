#!/usr/bin/env bash
# Compare libft_malloc vs glibc malloc on the same workload.
# Usage: ./run_bench.sh [iterations of whole battery]

set -e
DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(dirname "$DIR")"
BENCH="$DIR/bench"
SOS="libft_malloc.so"

ITER="${1:-1}"

make -s -C "$ROOT" re >/dev/null

cc -Wall -Wextra -O2 "$DIR/bench.c" -o "$BENCH" -lpthread

echo ">> glibc malloc"
N="$ITER" "$BENCH"
echo
echo ">> libft_malloc (LD_PRELOAD=$ROOT/$SOS)"
N="$ITER" env LD_PRELOAD="$ROOT/$SOS" "$BENCH"

rm -f "$BENCH"