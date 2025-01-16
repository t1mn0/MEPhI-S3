#include <gtest/gtest.h>

#include <random>
#include <ctime>
#include <type_traits>
#include <chrono>
#include <fstream>

#include "../../include/Sort/Sort.hpp"
#include "../../include/Sort/SmartSequence.hpp"
#include "../../include/Sequence/ArraySequence.hpp"
#include "../../include/Sequence/Array.hpp"

void fill_sequence_random(tmn::sequence::Sequence<int>& seq, size_t n) {
  std::mt19937 generator(std::time(0));
  std::uniform_int_distribution<int> distribution(0, 200000);

  for (size_t i = 0; i < n; ++i) {
    seq.push_back(distribution(generator));
  }
}

bool is_sorted(const tmn::sequence::Sequence<int>& seq, tmn::Function<int(const int&, const int&)>& cmp) {
  size_t size = seq.size();
  if (size <= 1) {
    return true;
  }

  for (size_t i = 0; i < size - 1; ++i) {
    if (cmp(seq.get(i), seq.get(i + 1)) > 0) {
      std::cout << seq.get(i) << "  " << seq.get(i + 1) << std::endl;
      return false;
    }
  }

  return true;
}

template <typename SorterType, typename SequenceType>
void test_sorting(std::string&& filename) {
  const std::string filepath = std::string(EXECUTABLE_PATH) + "/TestResults/" + filename;

  std::ofstream outfile(filepath);
  if (!outfile.is_open()) {
    std::cerr << "Failed to open log file: " << filepath << std::endl;
    return;
  }

  tmn::Function<int(const int&, const int&)> default_cmp(tmn::sort::default_comparator<int>);

  for (size_t size_factor = 1; size_factor <= 50; ++size_factor) {
    size_t size = size_factor * 10000;
    SequenceType seq;
    fill_sequence_random(seq, size);

    SorterType sorter;

    auto start = std::chrono::high_resolution_clock::now();
    sorter(seq);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    outfile <<  duration << std::endl;

    ASSERT_TRUE(is_sorted(seq, default_cmp));
  }
  outfile.close();
}

TEST(SortingTest, HeapSortArraySequence) {
  test_sorting<tmn::sort::HeapSort<int>, tmn::sequence::ArraySequence<int>>("heapsort_arrayseq.txt");
}

TEST(SortingTest, HeapSortSmartSequence) {
    test_sorting<tmn::sort::HeapSort<int>, tmn::sequence::SmartSequence<int>>("heapsort_smartseq.txt");
}

TEST(SortingTest, QuickSortArraySequence) {
    test_sorting<tmn::sort::QuickSort<int>, tmn::sequence::ArraySequence<int>>("quicksort_arrayseq.txt");
}

TEST(SortingTest, QuickSortSmartSequence) {
  test_sorting<tmn::sort::QuickSort<int>, tmn::sequence::SmartSequence<int>>("quicksort_smartseq.txt");
}

TEST(SortingTest, ShellSortArraySequence) {
  test_sorting<tmn::sort::ShellSort<int>, tmn::sequence::ArraySequence<int>>("shellsort_arrayseq.txt");
}

TEST(SortingTest, ShellSortSmartSequence) {
  test_sorting<tmn::sort::ShellSort<int>, tmn::sequence::SmartSequence<int>>("shellsort_smartseq.txt");
}