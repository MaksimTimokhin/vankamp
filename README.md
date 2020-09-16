# vankamp
JB project test task

Утилита строит случайную диаграмму Ван-Кампена для заданной группы.

# Установка

```git clone https://github.com/MaksimTimokhin/vankamp.git
 cd vankamp
 mkdir build
 cd build
 cmake ..
 make vankamp
```
После этого бинарник находится в текущей директории.

# Использование

`./vankamp <output.png>`. Далее программа читает с клавиатуры задание группы в виде <x_1, ... , x_i | r1_, ... r_j>, где все r_j --- строки над x_i и x_i, выводит
сгенерированное слово и сохраняет диаграмму в указанный файл.
