import re
import matplotlib.pyplot as plt
import numpy as np

def parse_log_line(line):
    match = re.match(r'(\d+)\)\s*([\d.]+)\s*nanoseconds.*\[(.*)\]', line)
    if match:
        try:
            number = int(match.group(1))
            time = float(match.group(2))
            type = match.group(3)
            return number, time, type
        except ValueError:
            return None
    else:
        return None

def process_log_file(filename):
    test_hashtable_data = []
    test_unorderedtable_data = []

    with open(filename, 'r') as f:
        for line in f:
            result = parse_log_line(line)
            if result:
                number, time, type = result
                if type == 'TEST_HASHSET':
                    test_hashtable_data.append((number, time))
                elif type == 'TEST_UNORDEREDSET':
                    test_unorderedtable_data.append((number, time))

    return test_hashtable_data, test_unorderedtable_data


def plot_results(test_hashtable_data, test_unorderedtable_data):
    if not test_hashtable_data or not test_unorderedtable_data:
        print("No data to plot.")
        return

    hashtable_x, hashtable_y = zip(*test_hashtable_data)
    unorderedtable_x, unorderedtable_y = zip(*test_unorderedtable_data)

    fig, ax = plt.subplots()

    ax.plot(hashtable_x, hashtable_y, label='TEST_HASHSET')
    ax.plot(unorderedtable_x, unorderedtable_y, label='TEST_UNORDEREDSET')

    max_y = max(max(hashtable_y), max(unorderedtable_y))
    ax.set_ylim(0, 800000)
    
    ax.set_xlabel('Номер замера (замер = создание + занесение в структуру данных 1000 записей)')
    ax.set_ylabel('Время (наносекунды)')
    ax.set_title('Время выполнения вставки r-value пар')
    ax.legend()
    plt.grid(True)
    plt.show()


if __name__ == "__main__":
    log_filename = "../build/Testing/Temporary/LastTest.log" 
    hashtable_data, unorderedtable_data = process_log_file(log_filename)
    plot_results(hashtable_data, unorderedtable_data)