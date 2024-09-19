import re
import matplotlib.pyplot as plt



def extract_nanoseconds(filename):
    nanoseconds = []
    pattern = re.compile(r'\d+\)\s+(\d+)\s+nanoseconds\s+\(time of creation 100 sharedptr by 1 rawptr\)')
    with open(filename, 'r') as file:
        for line in file:
            match = pattern.search(line)
            if match:
                ns_value = int(match.group(1))
                nanoseconds.append(ns_value)
    return nanoseconds



def plot_nanoseconds(ns_values):
    plt.figure(figsize=(15, 10))
    plt.plot(ns_values, marker='o', linestyle='-', color='b')
    plt.title('Results in Nanoseconds')
    plt.xlabel('Hundred number')
    plt.ylabel('Time (nanoseconds)')
    plt.grid(True)
    plt.show()



if __name__ == "__main__":
    filename = "../build/Testing/Temporary/LastTest.log"
    
    nanoseconds = extract_nanoseconds(filename)
    
    plot_nanoseconds(nanoseconds)
