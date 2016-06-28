rm -rf .build

rm -rf build_rel
python workspace_tools/build.py -r -d -m KL26Z -t GCC_ARM
mv .build build_rel

rm -rf build_debug
python workspace_tools/build.py -r -d -m KL26Z -t GCC_ARM -o debug-info
mv .build build_debug
