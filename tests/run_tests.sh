#!/usr/bin/env bash
# run_tests.sh - compiles and runs the ft_malloc test suite (mini-valgrind).

set -u

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
SRC="$ROOT/src"
TST="$ROOT/tests"
OUT="/tmp/opencode/ft_malloc"

CC="cc"
CFLAGS="-Wall -Wextra -Werror -Wno-use-after-free -Wno-free-nonheap-object \
	-Wno-alloc-size-larger-than -g -pthread -I$ROOT -I$SRC -I$SRC/libft -I$TST"

ALLOC="ft_malloc.c ft_free.c ft_realloc.c ft_calloc.c ft_show_alloc_mem.c \
	$SRC/alloc_core.c $SRC/arena.c $SRC/arena_ops.c $SRC/chunk.c \
	$SRC/coalesce.c $SRC/debug.c $SRC/free_utils.c $SRC/large.c $SRC/tcache.c \
	$SRC/utils.c"
LIBFT="$SRC/libft/ft_bzero.c $SRC/libft/ft_strlen.c $SRC/libft/ft_memcpy.c \
	$SRC/libft/ft_memset.c"
ENGINE="$TST/test_engine.c"

TESTS="t_basic t_calloc t_large t_realloc t_stress t_threads t_robust \
	t_bounds t_leaks"

all_pass=1
for t in $TESTS; do
	bin="${OUT}_$t"
	if ! $CC $CFLAGS "$TST/$t.c" $ENGINE $ALLOC $LIBFT -o "$bin" 2>"${OUT}_$t.err"; then
		echo "[$t] COMPILE FAIL"
		cat "${OUT}_$t.err"
		all_pass=0
		continue
	fi
	if "$bin"; then
		echo "[$t] PASS"
	else
		echo "[$t] FAIL"
		all_pass=0
	fi
done

if [ "$all_pass" = "1" ]; then
	echo "ALL TESTS PASSED"
	exit 0
fi
echo "SOME TESTS FAILED"
exit 1