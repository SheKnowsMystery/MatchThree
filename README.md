# Тестовое задание

## Инструкция по сборке

### Необходимо

- CMake
- vcpkg
- система сборки, поддерживаемая генератором CMake
- компилятор, поддерживающий стандарт языка C++20

### Шаги

1. Открыть консоль в папке проекта
2. Создать папку __`/build`__ и перейти в неё

    ```cmd
    mkdir build
    cd build
    ```

3. Сконфигурировать проектные файлы системы сборки используя CMake (при необходимости указав предпочитаемые систему сборки и компилятор, а так же тип сборки `Release` для `Single-config` генераторов: `-DCMAKE_BUILD_TYPE=Release`)

    [!ВАЖНО!]

    - либо предварительно добавить переменную среды с именем __`VCPKG_ROOT`__, значением которой будет __путь к корню vcpkg__ (далее как `<vcpkg-root>`)

      ```cmd
      cmake ..
      ```

    - либо передать в качестве аргумента к CMake путь к __toolchain__ vcpkg

      ```cmd
      cmake .. -DCMAKE_TOOLCHAIN_FILE="<vcpkg-root>/scripts/buildsystems/vcpkg.cmake"
      ```

      что эквивалентно

      ```cmd
      cmake .. --toolchain="<vcpkg-root>/scripts/buildsystems/vcpkg.cmake"
      ```

    [!ВАЖНО!]

4. Собрать проект в конфигурации __`Release`__ и установить нужные файлы в папку __`/bin`__

    ```cmd
    cmake --build . --config Release --target install --
    ```

5. Перейти в папку __`/bin`__ и запустить приложение

    ```cmd
    cd ../bin
    ./MatchThree.exe
    ```

## Пост Скриптум

Возникли проблемы с зависимостями __`SDL2-image`__ из-за того,
что (насколько я понял, изучив __issues__ на __GitHub__)
библиотека загружает свои зависимости в __runtime__,
что приводит к тому, что эти зависимости почти невозможно отследить.
В __`CMakeLists.txt`__ немного костыльный способ скопировать бинарники библиотек.
Не уверен, насколько он кросс-платформенный.

---

Какая-то часть кода взята из предыдущего тестового задания, какая-то из моих наработок до этого.