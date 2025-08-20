current_dir=/home/ubuntu/project/lvgl/sip_sdk_example/main/libs/alsa/mips

rm -rf $current_dir/*

cd /home/ubuntu/project/third_party/alsa-lib-1.2.11

make clean
find ./ -name "*.o" -exec rm -rf {} \;
./configure CC=/home/ubuntu/project/linux/tools/toolchains/mips-gcc720-glibc229/bin/mips-linux-gnu-gcc\
        CXX=/home/ubuntu/project/linux/tools/toolchains/mips-gcc720-glibc229/bin/mips-linux-gnu-g++ \
	CFLAGS="-fPIC" \
	LDFLAGS="-fPIC" \
        --enable-shared --disable-static --host=mips-linux --with-configdir=/usr/data/mips-alsa --prefix=$current_dir
make -j16
make install
