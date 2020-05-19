# PNG Редактор

## Зависимости

* [Qt5](https://www.qt.io/download) Разработка производилась на версии 5.14.2, работоспособность на более низких версиях не гарантируется
* libpng. На Ubuntu/Debian можно установить командой ```sudo apt-get install libpng-dev```

## Сборка

* Открыть проект в QtCreator
* Альтернативный способ.
```
mkdir build && cd build
qmake ../source
make
```

## Примеры работы

### Исходные картинки

1. ![test][Images/test.png]
2. ![rgb][Images/rgb.png]
3. ![github][Images/github.png]
4. ![jetbrains][Images/jetbrains.png

### Работа программы

1. Запуск ![start][for_readme/pngeditor0.png]
2. Открытие картинки ![start][for_readme/pngeditor1.png]
3. Рисвание треугольника ![start][for_readme/pngeditor2.png]
4. Рисование линий ![start][for_readme/pngeditor3.png]
5. Замена наибольшего прямоугольника(4 раза) ![start][for_readme/pngeditor4.png]
6. Создание колллажа ![start][for_readme/pngeditor5.png]
Результаты находятся в папке **Image**, их названия начинаются с _res__
