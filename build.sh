dir=$(pwd)
rm -rf BUILD

rm -rf build_rel
python tools/build.py -r -d -m KL26Z,KL27Z -t GCC_ARM --profile tools/profiles/release.json
mv BUILD build_rel

rm -rf build_debug
python tools/build.py -r -d -m KL26Z,KL27Z -t GCC_ARM --profile tools/profiles/debug.json
mv BUILD build_debug

ln -s $dir/build_rel $dir/BUILD
