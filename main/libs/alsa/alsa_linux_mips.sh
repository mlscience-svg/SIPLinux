current_dir=/home/ubuntu/project/lvgl/sip_sdk_example/main/libs/alsa/mips_gcc540

rm -rf $current_dir/*

cd /home/ubuntu/project/third_party/alsa-lib-1.2.11

make clean
find ./ -name "*.o" -exec rm -rf {} \;
./configure CC=/home/ubuntu/project/sip_linux_sdk/resources/mips-gcc540-uclibc0.9.33.2-64bit-r3.3.0.smaller/bin/mips-linux-uclibc-gcc \
        CXX=/home/ubuntu/project/sip_linux_sdk/resources/mips-gcc540-uclibc0.9.33.2-64bit-r3.3.0.smaller/bin/mips-linux-uclibc-g++ \
	CFLAGS="-fPIC" \
	LDFLAGS="-fPIC" \
        --enable-shared --disable-static --host=mips-linux --with-configdir=/usr/data/mips_gcc540-alsa --prefix=$current_dir
make -j16
make install
