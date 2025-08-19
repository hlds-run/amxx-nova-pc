#!/bin/sh

if uname -s | grep -qi "mingw" || uname -s | grep -qi "msys"; then
    CLANG_FORMAT="tools/windows/clang-format-20_windows-amd64.exe"
elif [ "$(uname -s)" = "Darwin" ]; then
    CLANG_FORMAT="tools/macos/clang-format-20_macosx-amd64"
else
    CLANG_FORMAT="tools/linux/clang-format-20_linux-amd64"
fi

FILES=$(git diff --cached --name-only --diff-filter=ACM | grep -E '\.(cpp|hpp|c|h|scp)$')

if [ -z "$FILES" ]; then
    exit 0
fi

echo "Running clang-format on staged files..."

for f in $FILES; do
    "$CLANG_FORMAT" -i "$f"
    git add "$f"
done
