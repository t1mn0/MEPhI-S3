import re
import matplotlib.pyplot as plt



def analyze_results(filename):
    data = {
        "stl_uniqueptr_creation": [],
        "stringptr_creation": [],
        "uniqueptr_creation": []
    }

    with open(filename, "r") as f:
        for line in f:
            match_shared = re.search(r"(\d+) nanoseconds \(time of creation \d+ stl_uniqueptr\) \[TEST_UNIQUE]", line)
            match_string = re.search(r"(\d+) nanoseconds \(time of creation \d+ string\*\) \[TEST_UNIQUE]", line)
            match_uniqueptr = re.search(r"(\d+) nanoseconds \(time of creation \d+ uniqueptr\) \[TEST_UNIQUE]", line)

            if match_shared:
                data["stl_uniqueptr_creation"].append(int(match_shared.group(1)))
            elif match_string:
                data["stringptr_creation"].append(int(match_string.group(1)))
            elif match_uniqueptr:
                data["uniqueptr_creation"].append(int(match_uniqueptr.group(1)))

    return data



def plot_data(data):
    x_max = max(len(data["stl_uniqueptr_creation"]), len(data["stringptr_creation"]), len(data["uniqueptr_creation"]))
    y_min = min(min(data["stl_uniqueptr_creation"]), min(data["stringptr_creation"]), min(data["uniqueptr_creation"])) if data["stl_uniqueptr_creation"] and data["stringptr_creation"] and data["uniqueptr_creation"] else 0
    y_max = max(max(data["stl_uniqueptr_creation"]), max(data["stringptr_creation"]), max(data["uniqueptr_creation"])) if data["stl_uniqueptr_creation"] and data["stringptr_creation"] and data["uniqueptr_creation"] else 0


    plt.figure(figsize=(12, 6))

    plt.subplot(131)
    plt.plot(data["stl_uniqueptr_creation"], linestyle='-')
    plt.title("Время создания по 1000 stl uniqueptr")
    plt.xlabel("Номер теста")
    plt.ylabel("Время (наносекунды)")
    plt.xlim(0, x_max)
    plt.ylim(y_min, y_max) if y_min != y_max else plt.ylim(0,1)


    plt.subplot(132)
    plt.plot(data["stringptr_creation"], linestyle='-')
    plt.title("Время создания по 1000 string*")
    plt.xlabel("Номер теста")
    plt.ylabel("Время (наносекунды)")
    plt.xlim(0, x_max)
    plt.ylim(y_min, y_max) if y_min != y_max else plt.ylim(0,1)

    plt.subplot(133)
    plt.plot(data["uniqueptr_creation"], linestyle='-')
    plt.title("Время создания по 1000 uniqueptr")
    plt.xlabel("Номер теста")
    plt.ylabel("Время (наносекунды)")
    plt.xlim(0, x_max)
    plt.ylim(y_min, y_max) if y_min != y_max else plt.ylim(0,1)

    plt.tight_layout()
    plt.show()



if __name__ == "__main__":
    filename = "../build/Testing/Temporary/LastTest.log"
    
    data = analyze_results(filename)
    plot_data(data)
