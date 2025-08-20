current_dir=/home/ubuntu/project/lvgl/sip_sdk_example/main/libs/openssl/mips

rm -rf $current_dir/*

cd /home/ubuntu/project/third_party/openssl-3.2.2

find ./ -name "*.o" -exec rm -rf {} \;
./Configure linux-mips32 no-asm \
	    --cross-compile-prefix=/home/ubuntu/project/linux/tools/toolchains/mips-gcc720-glibc229/bin/mips-linux-gnu- \
        CFLAGS="-fPIC -O3" \
        LDFLAGS="-fPIC -O3" \
        --prefix=$current_dir

sed -i 's/-m64/ /g' Makefile

make clean
make -j20
make install
