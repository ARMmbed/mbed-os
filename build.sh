dir=$(pwd)
rm -rf BUILD

rm -rf build_debug
python tools/build.py -r -d -m HEADSUP_AERO -t GCC_ARM --profile tools/profiles/debug.json
mv BUILD build_debug

ln -s $dir/build_rel $dir/BUILD
