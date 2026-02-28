#pragma once

#include <vector>
#include <utility>

#include "../core/Operations.h"
#include "../core/Request.h"

inline constexpr std::size_t kHashGoldenRatio64 = 0x9e3779b97f4a7c15ULL;

struct OperationKey {
  OpType op{};
  std::vector<int> args;

  friend bool operator==(const OperationKey& a, const OperationKey& b) noexcept {
   return a.op == b.op && a.args == b.args;
  }
};

inline void normalize(OperationKey& key) {
 const bool commutative = (key.op == OpType::ADD || key.op == OpType::MUL);
 if (!commutative) return;

 if (key.args.size() == 2 && key.args[0] > key.args[1]) {
  std::swap(key.args[0], key.args[1]);
 }
}

inline OperationKey makeOperationKey(const Request& request) {
 OperationKey key;
 key.op = opFromString(request.operation());
 key.args = request.args();

 normalize(key);
 return key;
}

struct OperationKeyHash {
 std::size_t operator() (const OperationKey& key) const noexcept {
  std::size_t h = std::hash<int>{}(static_cast<int>(key.op));

  for (auto v : key.args) {
   std::size_t hv = std::hash<int>{}(v);
   h ^= hv + kHashGoldenRatio64 + (h << 6) + (h >> 2);
  }

  return h;
 }
};