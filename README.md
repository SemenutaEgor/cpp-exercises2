Небольшое консольное приложение-калькулятор на C++, использующее внешнюю библиотеку математических функций `mathlib`

## Сборка
cmake -S . -B build
cmake --build build

## Установка
sudo cmake --build build --target install
sudo cmake --build build --target uninstall
