#!/bin/bash

echo ---------------------------------------------------------
echo Autoformatting all CMake files
echo ---------------------------------------------------------

while IFS= read -r -d '' file
do
  if (! cmake-format --check "$file" 2>/dev/null); then
    echo "Reformatting $file"
    cmake-format -i "$file"
  fi
done < <(find . -name 'CMakeLists.txt' -or -name '*.cmake' -print0)

echo Auto-format complete.