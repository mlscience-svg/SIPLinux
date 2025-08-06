current_dir=/home/ubuntu/project/lvgl/sip_sdk_example/main/libs/openssl/mips_gcc540

rm -rf $current_dir/*

cd /home/ubuntu/project/third_party/openssl-3.2.2

find ./ -name "*.o" -exec rm -rf {} \;
./Configure linux-mips32 \
	--cross-compile-prefix=/home/ubuntu/project/sip_linux_sdk/resources/mips-gcc540-uclibc0.9.33.2-64bit-r3.3.0.smaller/bin/mips-linux-uclibc- \
        CFLAGS="-fPIC" \
        LDFLAGS="-fPIC" \
        no-async \
        --prefix=$current_dir

make clean
make -j20
make install
