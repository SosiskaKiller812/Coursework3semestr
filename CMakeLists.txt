cmake_minimum_required(VERSION 3.16)
project(BusStationInfoSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(BusStationInfoSystem
    # Основные файлы
    src/main.cpp
    src/mainwindow.cpp
    src/mainmenu.cpp
    src/filedatabase.cpp
    src/route.cpp
    src/company.cpp
    src/addstopdialog.cpp
    src/trip.cpp
    src/editroutedialog.cpp
    src/routedialog.cpp
    src/routedetailsdialog.cpp
    
    # Новые классы - базовые

    
    # Система скидок
    
    # Контейнерные классы
    src/schedule.cpp
    
    # Утилитные классы
    src/pricecalculator.cpp
    src/routefinder.cpp
    src/reportgenerator.cpp
    
    # Служебные классы
    src/logger.cpp
    src/configmanager.cpp
    
    # Заголовочные файлы
    include/mainwindow.h
    include/filedatabase.h
    include/route.h
    include/company.h
    include/addstopdialog.h
    include/mainmenu.h
    include/trip.h
    include/editroutedialog.h
    include/stop.h
    include/routedialog.h
    include/routedetailsdialog.h
    include/iserializable.h
    include/DatabaseException.h
    include/RouteException.h 
    include/ValidationException.h
    
    # Новые заголовочные файлы


    include/schedule.h
    include/routemanager.h
    include/pricecalculator.h
    include/routefinder.h

    include/reportgenerator.h
    include/logger.h
    include/configmanager.h
)

target_link_libraries(BusStationInfoSystem PRIVATE Qt6::Widgets)

# Copy data folder into build dir
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})
