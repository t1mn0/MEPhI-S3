#pragma once

namespace tmn_strategy {
    
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Hash strategies :

    // АЛЛОЦИРОВАТЬ ПАМЯТЬ В СТРАТЕГИЯХ НЕОБЯЗАТЕЛЬНО. В СЛУЧАЕ CHAINS STRATEGY ИНОГДА ПРИДЕТСЯ. В СЛУЧАЕ OA НЕ ПРИДЕТСЯ НИКОГДА.
    // Работа через collision_strategy_traits:
    // 1) ПОЛУЧИТЬ УКАЗАТЕЛЬ, НА МЕСТО КОТОРОГО БУДЕМ КОНСТРУИРОВАТЬ ОБЪЕКТ
    // 2) СООБЩИТЬ, КАКУЮ FORWARD-ПРОШИВКУ ИСПОЛЬЗОВАТЬ: горизонтальную ИЛИ вертикальную
    //    2.1) ЕСЛИ горизонтальную, ТО ПОИСК БЛИЖ. ЛЕВОГО
    //    2.2) ЕСЛИ вертикальную ТО БЕРЕМ ВЕРХ КОРЗИНЫ (считаем, что hash_index неизменен (свойство детерменированности))



template <class StorageType>
class SeparateChaining {
public:
    // StorageType* get_memplace(StorageType** storage, std::size_t storage_size, std::size_t hash_index){}

};

template <class StorageType>
class OpenAddressing {
public:
    StorageType* get_memplace(StorageType** storage, std::size_t storage_size, std::size_t hash_index){
        std::size_t count = 0;

        // Linear probing (линейное зондирование)
        while (storage[hash_index] != nullptr && count < storage_size) {
            hash_index = (hash_index + 1) % storage_size;
            count++;
        }

        if (count == storage_size) {
            return nullptr; 
        }

        return storage[hash_index];
    }

    static bool is_vertical() {
        return false;
    }

    static bool is_horizontally() {
        return true;
    }
};



template <class Strategy, class StorageType>
class collision_strategy_traits {
public:
    static StorageType* get_memplace(Strategy& strategy, StorageType** storage, std::size_t storage_size, std::size_t hash_index) {
        return strategy.get_memplace(storage, storage_size, hash_index);
    }

    static bool is_vertical() {
        return Strategy::is_vertical();
    }

    static bool is_horizontally() {
        return Strategy::is_horizontally();
    }
};

}