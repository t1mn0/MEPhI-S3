import re
import matplotlib.pyplot as plt

def analyze_results(filename):
    data = {
        "shared_creation": [],
        "string_creation": [],
        "uniqueptr_creation": []
    }

    with open(filename, "r") as f:
        for line in f:
            match_shared = re.search(r"(\d+) nanoseconds \(time of creation \d+ sharedptr by 1 rawptr\) \[TEST_SHARED]", line)
            match_string = re.search(r"(\d+) nanoseconds \(time of creation \d+ string\*\) \[TEST_UNIQUE]", line)
            match_uniqueptr = re.search(r"(\d+) nanoseconds \(time of creation \d+ uniqueptr\) \[TEST_UNIQUE]", line)

            if match_shared:
                data["shared_creation"].append(int(match_shared.group(1)))
            elif match_string:
                data["string_creation"].append(int(match_string.group(1)))
            elif match_uniqueptr:
                data["uniqueptr_creation"].append(int(match_uniqueptr.group(1)))

    return data


def plot_data(data):
    plt.figure(figsize=(12, 6))

    plt.subplot(131)
    plt.plot(data["shared_creation"], linestyle='-')
    plt.title("Время создания по 100 shared_ptr")
    plt.xlabel("Номер теста")
    plt.ylabel("Время (наносекунды)")

    plt.subplot(132)
    plt.plot(data["string_creation"], linestyle='-')
    plt.title("Время создания по 1000 string*")
    plt.xlabel("Номер теста")
    plt.ylabel("Время (наносекунды)")

    plt.subplot(133)
    plt.plot(data["uniqueptr_creation"], linestyle='-')
    plt.title("Время создания по 1000 uniqueptr")
    plt.xlabel("Номер теста")
    plt.ylabel("Время (наносекунды)")

    plt.tight_layout()
    plt.show()


def main():
    filename = "../build/Testing/Temporary/LastTest.log"
    
    data = analyze_results(filename)
    plot_data(data)


if __name__ == "__main__":
    main()
