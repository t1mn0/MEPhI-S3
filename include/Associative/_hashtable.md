## Реализация HashTable
`HashTable` состоит из массива узлов (указателей на узлы).
___Поля___:
- `Node** _storage = nullptr`
- `std::size_t _size = 0`
- `std::size_t _buffer_size = 256`
- `float _max_load_factor = 0.5`. Классический `current_load_factor` рассчитывается по следующей формуле: `current_load_factor = (_size / _buffer_size)`.

___Шаблонные аргументы___:
- `Key`. ~~На данный тип вешается концепт "тип, объекты которого можно проверять на равенство"~~.
- `Value`

Узел (внутренняя структура `Node`) хранит в себе:
- Пару ключ-значение: `tmn::Pair<const Key, Value> pair`
- Указатели: на следующий (`next`) и предыдущий (`prev`) узлы.
- Посчитанный хэш (кэширован) - `cache`

Процесс вычисления индекса:
1) Прогнать аргумент через `Hash`
2) Взять остаток от полученного результата при делении на `_buffer_size`

### Аллокация объектов
Для аллокации будут использоваться:
```cpp
std::allocator<Node> _alloc_node;
```
Выбор аллокатора для хеш-таблиц не предусмотрен.

### Итерация по контейнеру
>![StrategyChains](../../img/L3%20StrategyChains.png)
>Упрощенная иллюстрация внутреннего устройства `HashTable` (!в массиве не сами узлы!), построенной на _стратегии цепочек_. Все узлы между собой связаны в одну сторону, чтобы обеспечить итерацию по контейнеру, в обратную - чтобы делать удаление за среднее O(1). Для итерации возможна реализация простого `list`-подобного итератора (_forward iterator_). Образ итерируемого объекта будет простым односвязным списком, тип значения узлов которого будет `tmn::Pair<const Key, Value>`: ![LinkedList](../../img/L3%20LinkedList.png)

### Грубая оценка
Такая хэш-таблица требует памяти __O(K + V)__, где __K + V__ - множество пар _ключ+запись_. Обертки в виде: `Pair<Key, Value>`, `Node` в данном случае не учитываются (рассматриваются как достаточно легковесные обертки).

При этом временная сложность поиска остается __О(1)__ (аморт.): сначала подсчёт хэш-значения, а потом проход по бакету, __если требуется__.

На данный момент используется детерменированная хеш-функция из STL (просто функтор обернут в функцию для сомнительного удобоства).