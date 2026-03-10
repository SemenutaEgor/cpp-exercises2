Небольшое консольное приложение-калькулятор на C++, использующее внешнюю библиотеку математических функций `mathlib`

## Сборка
`cmake -S . -B build`\
`cmake --build build`

## Установка
`sudo cmake --build build --target install`\
`sudo cmake --build build --target uninstall`

## TSan
`cmake -S . -B build-tsan -G Ninja \` \
  `-DCMAKE_BUILD_TYPE=Debug \` \
  `-DCMAKE_CXX_COMPILER=clang++ \` \
  `-DCMAKE_C_COMPILER=clang \` \
  `-DCMAKE_CXX_FLAGS="-O1 -g -fno-omit-frame-pointer -fsanitize=thread -fno-pie" \` \
  `-DCMAKE_EXE_LINKER_FLAGS="-fsanitize=thread -fuse-ld=lld -no-pie"`\
`cmake --build build-tsan`\
`ctest --test-dir build-tsan -V`
