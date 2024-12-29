#include "../../include/Sequence/ArraySequence.hpp"

using Matrix = tmn::sequence::ArraySequence<tmn::sequence::ArraySequence<int>>;

template <typename VertexId>
void pretty_print_matrix(int num, const std::string& test_name, const Matrix& matrix, const tmn::sequence::ArraySequence<VertexId>& seq);

bool is_symmetric_matrix(const Matrix& matrix);

struct complex_num {
public:
  double real;
  double imag;

  complex_num(double r, double i);

  double abs() const noexcept;

  bool operator<(const complex_num& other) const;

  bool operator>(const complex_num& other) const;
  bool operator<=(const complex_num& other) const;
  bool operator>=(const complex_num& other) const;
  bool operator==(const complex_num& other) const;
  bool operator!=(const complex_num& other) const;
};

#include "graph_test_utils.tpp"