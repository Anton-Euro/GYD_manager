cmake_minimum_required(VERSION 3.25)

set(PROJECT_N testgui)

project(${PROJECT_N} VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt/6.7.2_1/lib/cmake")
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Concurrent Network)
find_package(CURL 8.6.0 REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})
find_package(Boost 1.83.0 REQUIRED COMPONENTS)
include_directories(${Boost_INCLUDE_DIRS})
find_package(nlohmann_json 3.11.3 REQUIRED)
find_package(OpenSSL REQUIRED)

set(SOURCES
    src/main.cpp
    src/main/mainwindow.cpp
    src/main/files_table_view.cpp
    src/main/item.cpp
    src/main/itemlist.cpp
    src/main/account.cpp
    src/settings/settingswindow.cpp
    src/settings/account_table_view.cpp
    src/about/aboutwindow.cpp
    src/api/http/client.cpp
    src/api/http/server.cpp
    src/api/google_api.cpp
    src/api/yandex_api.cpp
    src/api/base_api.cpp
)

set(HEADERS
    src/main/mainwindow.h
    src/main/files_table_view.hpp
    src/main/item.hpp
    src/main/itemlist.hpp
    src/main/account.hpp
    src/settings/settingswindow.h
    src/settings/account_table_view.h
    src/about/aboutwindow.h
    src/api/http/server.hpp
    src/api/http/client.hpp
    src/api/http/root_certificates.hpp
    src/api/google_api.hpp
    src/api/yandex_api.hpp
    src/api/base_api.hpp
    src/utils/datetime.hpp
    src/utils/tools.hpp
    src/utils/focus_delegate.hpp
)

set(UI_HEADERS
    src/ui/ui_mainwindow.h
    src/ui/ui_settings.h
    src/ui/ui_about.h
)

set(RESOURCE_FILES
    src/resources.qrc
)

qt6_wrap_cpp(MOC_SOURCES
    src/settings/account_table_view.h
    src/main/mainwindow.h
    src/settings/settingswindow.h
    src/about/aboutwindow.h
)

qt_add_resources(RSS_SOURCES ${RESOURCE_FILES})

add_executable(${PROJECT_N}
    ${SOURCES}
    ${HEADERS}
    ${UI_HEADERS}
    ${RSS_SOURCES}
    ${MOC_SOURCES}
)

target_link_libraries(${PROJECT_N}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Concurrent
    Qt6::Network
    ${CURL_LIBRARIES}
    ${Boost_LIBRARIES}
    nlohmann_json::nlohmann_json
    OpenSSL::SSL
    OpenSSL::Crypto
)


# set(UIS
#     mainwindow.ui
# )
# qt6_wrap_ui(UI_HEADERS ${UIS})