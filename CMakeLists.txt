cmake_minimum_required(VERSION 3.16)
project(BusStationInfoSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
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
    src/passenger.cpp
    src/ticket.cpp
    src/bus.cpp
    src/driver.cpp
    src/seat.cpp
    src/booking.cpp
    src/payment.cpp
    
    # Система скидок
    src/discount.cpp
    src/studentdiscount.cpp
    src/seniordiscount.cpp
    src/childdiscount.cpp
    
    # Контейнерные классы
    src/ticketoffice.cpp
    src/schedule.cpp
    
    # Утилитные классы
    src/pricecalculator.cpp
    src/routefinder.cpp
    src/ticketvalidator.cpp
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
    include/passenger.h
    include/ticket.h
    include/bus.h
    include/driver.h
    include/seat.h
    include/booking.h
    include/payment.h
    include/discount.h
    include/studentdiscount.h
    include/seniordiscount.h
    include/childdiscount.h
    include/ticketoffice.h
    include/schedule.h
    include/routemanager.h
    include/pricecalculator.h
    include/routefinder.h
    include/ticketvalidator.h
    include/reportgenerator.h
    include/logger.h
    include/configmanager.h
)

target_link_libraries(BusStationInfoSystem PRIVATE Qt6::Widgets)

# Copy data folder into build dir
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})
