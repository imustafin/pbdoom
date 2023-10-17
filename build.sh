export LC_ALL=C
SDK_PATH=$(cat sdk_path)
mkdir -p build && cd build && cmake -DCMAKE_TOOLCHAIN_FILE=$SDK_PATH/share/cmake/arm_conf.cmake .. && cmake --build .
