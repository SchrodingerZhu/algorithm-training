export module tree02;
import std;

export class Solution {
  class RMQIndex {
    std::vector<std::vector<long long>> min;

    constexpr static std::size_t ilog2(std::size_t length) {
      return std::numeric_limits<std::size_t>::digits -
             std::countl_zero(length) - 1;
    }

  public:
    RMQIndex(std::span<const long long> whole) : min{} {
      min.push_back(std::ranges::to<std::vector>(whole));
      std::size_t bits = ilog2(whole.size());
      for (std::size_t i = 1; i <= bits; ++i) {
        std::size_t len = 1uz << i;
        min.emplace_back(whole.size() - len + 1);
        for (std::size_t j = 0; j < min.back().size(); ++j)
          min[i][j] = std::min(min[i - 1][j], min[i - 1][j + len / 2]);
      }
    }
    long long query(std::size_t offset, std::size_t length) {
      std::size_t level = ilog2(length);
      std::size_t len = 1uz << level;
      return std::min(min[level][offset], min[level][offset + length - len]);
    }
  };

public:
  std::vector<long long>
  rangeMin(const std::vector<long long> &values,
           const std::vector<std::array<int, 2>> &queries) {
    RMQIndex index(values);
    return queries | std::views::transform([&](std::array<int, 2> q) {
             return index.query(q[0], q[1] - q[0] + 1);
           }) |
           std::ranges::to<std::vector>();
  }
};
