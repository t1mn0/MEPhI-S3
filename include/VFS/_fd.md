## Реализация FileDescriptor
1. `enum Permission`
Перечислимый тип `Permission` определяет права доступа к файлу или каталогу:

| Значение | Целочисленное значение | Описание | Строковое представление |
| ----------------------- | ------------------------ | ------------------------- | ------------------------|
| NONE | 0 | Отсутствие прав доступа | -- | 
| READ | 1 | Право на чтение | r- | 
| WRITE | 2 | Право на запись | -w | 
| READWRITE | 3 |Права на чтение и запись | rw |

2. `FilePermissions`

Структура FilePermissions соединяет в себе права доступа к файлу или каталогу  для владельца, группы-владельца и других пользователей.

3. `FileDescriptor`

Структура `FileDescriptor` хранит полные метаданные для файла или каталога в виртуальной файловой системе:

Методы:
- Геттеры (`IsDirectory`, `PhysicalPath`, `VirtualPath`, `Size`, `OwnerUser`, `OwnerGroup`, `WhenCreated`, `WhenModified`, `InnerFiles`) для доступа к закрытым переменным-членам.
- Модификаторы (`AddInnerFile` добавляет имя файла в множество `inner_files` (поле) для каталогов).
- `nlohmann::json to_json() const noexcept`: преобразует объект `FileDescriptor` в объект `JSON` для простого хранения и передачи данных.
- `friend class VirtualFileSystem;`: предоставаление классу `VirtualFileSystem` доступ к закрытым членам `FileDescriptor`.

