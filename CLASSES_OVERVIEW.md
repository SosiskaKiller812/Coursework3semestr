# Обзор классов проекта "Автовокзал"

## Всего классов: 34

### Базовые классы (5)
1. **Company** - компания-перевозчик
2. **Route** - маршрут
3. **Stop** - остановка (структура)
4. **Trip** - поездка/рейс
5. **Passenger** - пассажир (наследуется от ISerializable)

### Классы билетной системы (4)
6. **Ticket** - билет (наследуется от ISerializable)
7. **Booking** - бронирование
8. **Payment** - платеж
9. **TicketOffice** - касса (контейнерный класс с итераторами)

### Классы транспорта (3)
10. **Bus** - автобус (наследуется от ISerializable)
11. **Driver** - водитель (наследуется от ISerializable)
12. **Seat** - место в автобусе

### Система скидок (4)
13. **Discount** - абстрактный класс для скидок (виртуальные функции)
14. **StudentDiscount** - скидка для студентов (наследуется от Discount)
15. **SeniorDiscount** - скидка для пенсионеров (наследуется от Discount)
16. **ChildDiscount** - скидка для детей (наследуется от Discount)

### Контейнерные и утилитные классы (6)
17. **Schedule** - расписание (контейнерный класс с итераторами)
18. **RouteManager** - менеджер маршрутов (шаблонный класс)
19. **PriceCalculator** - калькулятор цен
20. **RouteFinder** - поиск маршрутов
21. **TicketValidator** - валидатор билетов
22. **ReportGenerator** - генератор отчетов

### Служебные классы (2)
23. **Logger** - логгер (singleton, работа с файлами)
24. **ConfigManager** - менеджер конфигурации (singleton, работа с файлами)

### Базовые интерфейсы и исключения (4)
25. **ISerializable** - абстрактный интерфейс для сериализации
26. **DatabaseException** - исключение базы данных
27. **RouteException** - исключение маршрута
28. **ValidationException** - исключение валидации

### Классы работы с данными (1)
29. **FileDatabase** - база данных файлов (работа с файлами)

### GUI классы (5)
30. **MainWindow** - главное окно
31. **MainMenu** - главное меню
32. **AddStopDialog** - диалог добавления остановки
33. **EditRouteDialog** - диалог редактирования маршрута
34. **RouteDialog** - диалог маршрута
35. **RouteDetailsDialog** - диалог деталей маршрута

## Используемые технологии

### ✅ Классы и объекты
Все 34 класса используют объектно-ориентированный подход

### ✅ Дружественные функции и перегрузка операций
- **Trip**: `operator==`, `operator!=`, `operator<`, `operator>`, дружественная функция `operator<<`
- **Passenger**: `operator==`, `operator!=`
- **Ticket**: `operator==`, `operator!=`, `operator<`, дружественная функция `formatTicketInfo`
- **Bus**: `operator==`, `operator!=`, дружественная функция `getBusInfo`
- **Driver**: `operator==`, `operator!=`, `operator<`, дружественная функция `getDriverInfo`
- **Seat**: `operator==`, `operator!=`, `operator<`, дружественная функция `getSeatInfo`
- **Booking**: `operator==`, `operator!=`, `operator<`, дружественная функция `getBookingInfo`
- **Payment**: `operator==`, `operator!=`, `operator<`, дружественная функция `getPaymentInfo`
- **Schedule**: `operator+=`, `operator+`, дружественная функция `getScheduleInfo`
- **Discount**: `operator==`, `operator!=`
- **PriceCalculator**: `operator()`
- **Logger**: `operator<<`
- **ConfigManager**: `operator[]`, `operator+=`

### ✅ Наследование
- **ISerializable** → Passenger, Ticket, Bus, Driver
- **Discount** → StudentDiscount, SeniorDiscount, ChildDiscount
- **std::runtime_error** → DatabaseException, RouteException, ValidationException
- **QObject** → FileDatabase
- **QMainWindow** → MainWindow, MainMenu
- **QDialog** → AddStopDialog, EditRouteDialog, RouteDialog, RouteDetailsDialog

### ✅ Виртуальные функции и абстрактные классы
- **ISerializable**: абстрактный класс с виртуальными функциями `serialize()`, `deserialize()`
- **Discount**: абстрактный класс с виртуальными функциями:
  - `calculateDiscount()` - виртуальная чистая функция
  - `name()` - виртуальная чистая функция
  - `percentage()` - виртуальная чистая функция
  - `isApplicable()` - виртуальная чистая функция

### ✅ Шаблоны функций и классов
- **RouteManager<T>** - шаблонный класс для управления коллекциями маршрутов
- Использование шаблонов STL: `std::shared_ptr`, `std::optional`, `std::function`, `std::map`

### ✅ Генерация и обработка исключительных ситуаций
- **DatabaseException** - для ошибок базы данных
- **RouteException** - для ошибок маршрутов
- **ValidationException** - для ошибок валидации
- Обработка исключений в `TicketValidator::validateTicketThrow()`
- Обработка исключений в `Route::insertStop()`, `Route::removeStop()`

### ✅ Потоки ввода/вывода и работа с файлами
- **FileDatabase**: работа с файлами через QFile и QTextStream
- **ReportGenerator**: 
  - `exportToFile()` - экспорт в файл
  - `exportToCSV()` - экспорт в CSV
  - `saveReportToStream()` - работа с std::ofstream
  - `loadReportFromStream()` - работа с std::ifstream
- **Logger**: запись логов в файл через QTextStream и std::ofstream
- **ConfigManager**: 
  - `loadFromFile()` / `saveToFile()` - работа с QFile
  - `loadFromStream()` / `saveToStream()` - работа с std::ifstream/std::ofstream

### ✅ Контейнерные классы, итераторы и стандартные контейнеры STL
- **TicketOffice**: контейнерный класс с итераторами (TicketIterator, ConstTicketIterator)
- **Schedule**: контейнерный класс с итераторами (ScheduleIterator, ConstScheduleIterator)
- **RouteManager<T>**: шаблонный контейнерный класс с итераторами
- Использование STL контейнеров:
  - `QVector` - во многих классах
  - `std::map` - в Statistics (удален), Cache (удален)
  - `QHash` - в ConfigManager, Cache (удален)
  - `std::shared_ptr` - умные указатели
  - `std::optional` - в Cache (удален)
- Итераторы STL: используются в RouteManager, Schedule, TicketOffice
- Алгоритмы STL: `std::find_if`, `std::copy_if`, `std::remove_if`, `std::sort`, `std::accumulate`, `std::max_element`, `std::min_element`

## Итого: 34 класса с использованием всех требуемых технологий


