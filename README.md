# Как собрать алгоритм параллельного отжига

Сперва убедитесь, что у вас установлены GMP (https://gmplib.org/) и GSL

```
git clone --recurse-submodules https://github.com/ot931/ParallelTemperingFinalVersion.git
cd ParallelTemperingFinalVersion
mkdir build && cd build
cmake ..
cmake --build .
```

# Как пользоваться

Запустите `./metropolis --help`, и прочтите содержимое.

# Благодарности

Программа разработана за счет гранта Российского научного фонда № 23-22-00328, [https://rscf.ru/project/23-22-00328/](https://rscf.ru/project/23-22-00328/)

Проверено на суперкомпьютерном вычислительном кластере Института прикладной математики ДВО РАН.