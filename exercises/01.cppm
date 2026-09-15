export module tree01;
import std;

export class Solution {
public:
  std::vector<long long>
  rangeMin(const std::vector<long long> &values,
           const std::vector<std::array<int, 2>> &queries) {
    auto whole = std::span(values);
    return queries
      | std::views::transform([&](auto range) {
        auto span = whole.subspan(range[0], range[1] - range[0] + 1);
        return std::ranges::min(span);
      })
      | std::ranges::to<std::vector>();
  }
};
