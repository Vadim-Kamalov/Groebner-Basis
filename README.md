# Groebner-Basis

Библиотека для работы с базисами Грёбнера. 

## Требования

- Компилятор с поддержкой C++20
- CMake >= 3.28

## Сборка

```bash
git clone https://github.com/Vadim-Kamalov/Groebner-Basis.git
cd Groebner-Basis
mkdimkdir build && cd build
cmake ..
make -j$(nproc)
```

Тесты запускаются с помощью команды
```bash
./tests/run_tests
```
Библиотека собирается как статическая в `/lib/libgroebner_basis_lib.a`