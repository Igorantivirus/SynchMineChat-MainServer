# SynchMineChat-MainServer
## (Синхронный майнкрафт чат — главный сервер) 

[![English version](https://img.shields.io/badge/English%20version-blue)](README.md)

## Описание

Телеграмм бот для работы с синхронным майнкрафт чатом.
Отправляет сообщения из тг чата в майнкрафт парсер.

Способен фиксировать:
1) Текстовые сообщения
2) Аудио сообщения (в разработке)
3) Медиа файлы

Сервер с майнкрафтом можно найти здесь: [Ссылка на основной сервер](https://github.com/Igorantivirus/SynchMineChat-GameServer)

## Зависимости

Язык программирования: **С++20**

Библиотеки:
* [tgbot-cpp](https://github.com/reo7sp/tgbot-cpp) - Тегелрамм бот C++
* [CROW]([https://www.boost.org/](https://github.com/CrowCpp/Crow)) - Сервер
* [nlohmann-json](https://github.com/nlohmann/json) - библиотека для работы с JSON

## Особенности реализации

* Работает в многопоточном режима
  * Главный поток
  * Отдельные потоки для CROW и TgBot
  * Дополнительные потоки для CROW из его внутренней реализации
* Поддержка конфигурации через JSON-файлы

## Установка и запуск

### Предварительные требования
* Компилятор с поддержкой C++20 (GCC 10+, Clang 10+, MSVC 19.29+)
* CMake 3.15+
* Установленные зависимости (см. выше)

### Инструкция по сборке

1. **Клонируйте репозиторий**
   ```sh
   git clone https://github.com/Igorantivirus/SynchMineChat-MainServer
   cd SynchMineChat-MainServer 
   ```
2. **Соберите CMake**
   ```sh
   cmake -B build -DCMAKE_BUILD_TYPE=Release
   ```
3. **Соберите проект** 
   ```sh
   cmake --build build --config Release
   ```

### Запуск 

1. Создайте конфиг файлы конфигурации (структура конфига описана в дирректории *src/Services/Configs*):
  * CrowConfig.json
  * ServerConfig.json
  * TgBotConfig.json
2. Перейдите в директорию build и запустите программу
   ```sh
   ./main
   ``` 

## Лицензия 
MIT Лицензия
