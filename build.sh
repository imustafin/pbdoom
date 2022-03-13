SDK_PATH=$(cat sdk_path)
rm -rf build && mkdir build && cd build && cmake -DCMAKE_TOOLCHAIN_FILE=$SDK_PATH/share/cmake/arm_conf.cmake .. && cmake --build .
