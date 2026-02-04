#pragma once

#include <optional>
#include <unordered_map>

#include "OperationKey.h"

class Cache {
 public:
  std::optional<long long> get(const OperationKey& key) const {
    const auto it = data_.find(key);
    if (it == data_.end()) return std::nullopt;

    return it->second;
  }
  void put(const OperationKey& key, long long value) { data_[key] = value; }

 private:
  std::unordered_map<OperationKey, long long, OperationKeyHash> data_;
};