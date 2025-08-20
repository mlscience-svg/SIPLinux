cmake_minimum_required(VERSION 3.10.2)
project(sip_example)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_C_STANDARD 11)
## --- C++11 build flags ---
add_definitions(-D_GLIBCXX_USE_C99=1)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Release) #Release Debug
set(CM_BUILD_OPTION "mips") #x86_64 arm arm_ez mips mips_gcc540

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -s")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -s")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os -Wall -s")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os -Wall -s")   
endif ()

if (${CM_BUILD_OPTION} STREQUAL "x86_64")
        
elseif (${CM_BUILD_OPTION} STREQUAL "arm")
        set(CMAKE_BUILD_RPATH "/data/libs")
        #配置编译工具链
        set(CMAKE_SYSTEM_PROCESSOR arm)
        set(tools /home/ubuntu/project/sip_linux_sdk/resources/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf)
        set(CMAKE_C_COMPILER ${tools}/bin/arm-linux-gnueabihf-gcc)
        set(CMAKE_CXX_COMPILER ${tools}/bin/arm-linux-gnueabihf-g++)

        
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                ${CMAKE_CURRENT_SOURCE_DIR}/main/src
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/arm/include
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/curl_license/arm/include
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/openssl/arm/include
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/alsa/arm/include
        )

        set(LIB_FILES 
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/curl/arm/lib/libcurl.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/arm/lib/libsipsdk.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/openssl/arm/lib/libcrypto.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/openssl/arm/lib/libssl.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/curl_license/arm/lib/libcurl_license.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/alsa/arm/lib/libasound.so
        )

        set(lib_l "-lpthread -lm -ldl -lstdc++ -lrt")
elseif (${CM_BUILD_OPTION} STREQUAL "arm_ez")
        set(CMAKE_BUILD_RPATH "/data/libs")
        #配置编译工具链
        set(CMAKE_SYSTEM_PROCESSOR arm)
        set(tools /home/ubuntu/project/sip_linux_sdk/resources/gcc-7.4.0-x86_64_arm-ez-linux-uclibcgnueabihf)
        set(CMAKE_C_COMPILER ${tools}/bin/arm-ez-linux-uclibcgnueabihf-gcc)
        set(CMAKE_CXX_COMPILER ${tools}/bin/arm-ez-linux-uclibcgnueabihf-g++)

        
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                ${CMAKE_CURRENT_SOURCE_DIR}/main/src
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/arm_ez/include
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/curl_license/arm_ez/include
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/openssl/arm_ez/include
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/alsa/arm_ez/include
        )

        set(LIB_FILES 
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/curl/arm_ez/lib/libcurl.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/arm_ez/lib/libsipsdk.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/openssl/arm_ez/lib/libcrypto.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/openssl/arm_ez/lib/libssl.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/curl_license/arm_ez/lib/libcurl_license.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/alsa/arm_ez/lib/libasound.so
        )

elseif (${CM_BUILD_OPTION} STREQUAL "mips")
         #配置编译工具链
        set(CMAKE_SYSTEM_PROCESSOR mips)
        set(tools /home/ubuntu/project/linux/tools/toolchains/mips-gcc720-glibc229)
        set(CMAKE_C_COMPILER ${tools}/bin/mips-linux-gnu-gcc)
        set(CMAKE_CXX_COMPILER ${tools}/bin/mips-linux-gnu-g++)

        
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                ${CMAKE_CURRENT_SOURCE_DIR}/main/src
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/mips/sdk/include
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/curl_license/mips/include
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/openssl/mips/include
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/alsa/mips/include
        )

        set(LIB_FILES 
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/mips/pjsip/lib/libpj-mips-unknown-linux-gnu.a
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/mips/pjsip/lib/libpjlib-util-mips-unknown-linux-gnu.a
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/mips/pjsip/lib/libpjmedia-audiodev-mips-unknown-linux-gnu.a
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/mips/pjsip/lib/libpjmedia-codec-mips-unknown-linux-gnu.a
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/mips/pjsip/lib/libpjmedia-mips-unknown-linux-gnu.a
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/mips/pjsip/lib/libpjmedia-videodev-mips-unknown-linux-gnu.a
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/mips/pjsip/lib/libpjnath-mips-unknown-linux-gnu.a
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/mips/pjsip/lib/libpjsip-mips-unknown-linux-gnu.a
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/mips/pjsip/lib/libpjsip-simple-mips-unknown-linux-gnu.a
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/mips/pjsip/lib/libpjsip-ua-mips-unknown-linux-gnu.a
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/mips/pjsip/lib/libpjsua-mips-unknown-linux-gnu.a
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/mips/pjsip/lib/libpjsua2-mips-unknown-linux-gnu.a
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/mips/pjsip/lib/libresample-mips-unknown-linux-gnu.a
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/mips/pjsip/lib/libsrtp-mips-unknown-linux-gnu.a
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/mips/pjsip/lib/libyuv-mips-unknown-linux-gnu.a
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/mips/opus/lib/libopus.a
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/mips/aecm/lib/libaecm.a
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/mips/sdk/lib/libsipsdk.a
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/mips/license/lib/liblicense.a
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/curl_license/mips/lib/libcurl_license.a
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/mips/speexdsp/lib/libspeexdsp.a
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/curl/mips/lib/libcurl.a
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/alsa/mips/lib/libasound.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/openssl/mips/lib/libcrypto.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/openssl/mips/lib/libssl.so
        )
        set(lib_l "-lpthread -lm -ldl -lstdc++ -lrt")
endif ()

file(GLOB_RECURSE SOURCES 
        ${CMAKE_CURRENT_SOURCE_DIR}/main/src/*.cpp
)

add_executable(sip_example
        ${SOURCES}
)

# 链接第三方静态库
target_link_libraries(sip_example 
        ${lib_l}
        -Wl,--start-group ${LIB_FILES} -Wl,--end-group
)
