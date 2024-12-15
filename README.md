# GYD_manager
Агрегатор облачных хранилищ [Yandex Disk](https://disk.yandex.by/client/disk), [Google Drive](https://www.google.com/intl/ru/drive/), [Dropbox](https://www.dropbox.com/ru/) и тп. Использование всех этих сервисов в одном месте.

### Реализовано:
- Добавление аккаунтов облачных сервисов(Google drive, Yandex disk, DropBox).
- Скачивания файлов
- Загрузка файлов (выбор файлов/drag&drop)
- удаление файлов
- создание папок
- реализована вся файловая система
- поиск
- разные мелочи
- обновление списка
- т. п.

### Структура проекта
```
+-- src/
|  +-- about/
|  |  +-- aboutwindow.cpp
|  |  +-- aboutwindow.h
|  +-- api/
|  |  +-- http/
|  |  |  +-- client.cpp
|  |  |  +-- client.hpp
|  |  |  +-- server.cpp
|  |  |  +-- server.hpp
|  |  +-- base_api.hpp
|  |  +-- base_api.cpp
|  |  +-- google_api.hpp
|  |  +-- google_api.cpp
|  |  +-- dropbox_api.hpp
|  |  +-- dropbox_api.cpp
|  +-- forms/
|  |  +-- about.ui
|  |  +-- mainwindow.ui
|  |  +-- settings.ui
|  +-- main/
|  |  +-- account.cpp
|  |  +-- account.hpp
|  |  +-- files_table_view.hpp
|  |  +-- files_table_view.cpp
|  |  +-- item.hpp
|  |  +-- item.cpp
|  |  +-- itemlist.hpp
|  |  +-- itemlist.cpp
|  |  +-- mainwindow.h
|  |  +-- mainwindow.cpp
|  +-- settings/
|  |  +-- account_table_view.cpp
|  |  +-- account_table_view.hpp
|  |  +-- settingswindow.h
|  |  +-- settingswindow.cpp
|  +-- static/
|  +-- ...
|  +-- ui/
|  |  +-- ui_about.h
|  |  +-- ui_mainwindow.h
|  |  +-- ui_settings.h
|  +-- utils/
|  |  +-- datetime.hpp
|  |  +-- focut_delegate.hpp
|  |  +-- tools.hpp
```

### Зависимости
- Qt 6.7.2
- Curl 8.6.0
- Boost 1.83.0
- nlohmann_json 3.11.3
- OpenSSL

### Сборка
```bash
cmake --build .
```

### Ссылка на Sonar - [клац]()
