#!/bin/sh

case "$(uname -s)" in
    *inux*)
        CLANG_FORMAT="tools/linux/clang-format-20_linux-amd64"
        ;;
    *arwin*)
        CLANG_FORMAT="tools/macos/clang-format-20_macosx-amd64"
        ;;
    *INGW*|*SYS*|*CYGWIN*)
        CLANG_FORMAT="tools/windows/clang-format-20_windows-amd64.exe"
        ;;
    *)
        echo "Unsupported OS: $(uname -s)" >&2
        exit 1
        ;;
esac

# Directories to exclude
EXCLUDE_DIRS="build external libs/sourcepawn out"

FILES=$(git diff --cached --name-only --diff-filter=ACM | grep -E '\.(cpp|hpp|c|h|scp)$')
has_files=0

for f in $FILES; do
    skip=0

    for d in $EXCLUDE_DIRS; do
        case "$f" in
            "$d"/*)
                skip=1
                break
                ;;
        esac
    done

    if [ "$skip" -eq 1 ]; then
        continue
    fi

    if [ "$has_files" -eq 0 ]; then
        echo "Running clang-format on staged files..."
        has_files=1
    fi

    echo "Formatting: $f"
    "$CLANG_FORMAT" -i "$f"
    git add "$f"
done
