# Подключение библиотеки PartsEngine к работающему проекту

Для подключения библиотеки нужно добавить в файл проекта (.pro) следующие строки:
```
CONFIG(release,debug|release){
    LIBS += -L$$PWD/../partsEngine/ -lPartsEngine
}
CONFIG(debug,debug|release){
    LIBS += -L$$PWD/../partsEngine/ -ldPartsEngine
}
INCLUDEPATH += $$PWD/../partsEngine
DEPENDPATH += $$PWD/../partsEngine
PRE_TARGETDEPS += $$PWD/../partsEngine/libPartsEngine.a
```