export module tree03;
import std;

export class Solution {
public:
  std::vector<long long> windowMin(const std::vector<long long> &values,
                                   int k) {
    std::vector<long long> res;
    std::deque<std::pair<std::size_t, long long>> mono;
    auto push = [&](std::size_t position, long long value) {
      while (!mono.empty() && mono.back().second > value)
        mono.pop_back();
      mono.emplace_back(position, value);
    };
    std::size_t window = k;
    for (std::size_t i = 0; i < window; ++i)
      push(i, values[i]);
    res.push_back(mono.front().second);
    for (std::size_t i = window; i < values.size(); ++i) {
      push(i, values[i]);
      std::size_t s = i + 1 - window;
      while (mono.front().first < s)
        mono.pop_front();
      res.push_back(mono.front().second);
    }
    return res;
  }
};
