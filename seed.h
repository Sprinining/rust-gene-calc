#pragma once
#include "consts.h"
#include <array>
#include <cstddef>    // std::size_t
#include <functional> // std::hash
#include <memory>

using std::array;
using std::hash;
using std::shared_ptr;
using std::size_t;

// Seed 结构体表示一个种子，包含6个基因
struct Seed {
    // 六个基因，每个基因类型为 AppConsts::GeneType 枚举
    array<AppConsts::GeneType, 6> genes_;

    // 判断两个 Seed 是否相等（基因逐个比较）
    bool operator==(const Seed &other) const { return genes_ == other.genes_; }
};

namespace std {
// 针对 Seed 类型的哈希函数特化
template <> struct hash<Seed> {
    size_t operator()(const Seed &s) const {
        size_t h = 0;
        // 使用类似 boost::hash_combine 的方法对每个基因进行哈希混合
        for (const auto &gene : s.genes_) {
            h ^= hash<AppConsts::GeneType>{}(gene) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
};
} // namespace std

// 自定义用于 unordered_set 等容器中对 shared_ptr<Seed> 的相等比较
struct SeedPtrEqual {
    bool operator()(const shared_ptr<Seed> &a, const shared_ptr<Seed> &b) const {
        // 比较两个 shared_ptr 指向的 Seed 对象是否相等
        // 调用上面写的基因逐个比较的 operator==
        return *a == *b;
    }
};

// 自定义用于 unordered_set 等容器中对 shared_ptr<Seed> 的哈希函数
struct SeedPtrHash {
    size_t operator()(const shared_ptr<Seed> &s) const {
        // 对 shared_ptr 指向的 Seed 对象进行哈希，调用上面写的 Seed
        // 类型的哈希函数特化 std::hash<Seed>
        return hash<Seed>{}(*s);
    }
};
