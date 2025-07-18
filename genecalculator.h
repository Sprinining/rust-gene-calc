#ifndef GENECALCULATOR_H
#define GENECALCULATOR_H

#include "seed.h"
#include <array>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class GeneCalculator {
public:
    GeneCalculator();

    // 记录种子
    void addSeed(Seed);

    // 清除所有的种子记录
    void clearSeeds();

    // 根据已有的种子，计算出能通过杂交获得的最好的基因，以及四个角落所需要的种子
    void calculate();

    // 获取用于杂交的四个种子
    const std::array<std::shared_ptr<Seed>, 4> &getBreedingSeeds() const;

    // 获取最终能得到的最好的种子
    const Seed &getOffspringSeed() const;

    // 将基因枚举转成字符
    static char geneTypeToChar(AppConsts::GeneType);

private:
    // 所有的种子
    std::vector<std::shared_ptr<Seed>> seeds_;
    // 用于对种子去重
    std::unordered_set<std::shared_ptr<Seed>> seeds_set_;
    // 四个角落用于杂交的种子
    std::array<std::shared_ptr<Seed>, 4> breeding_seeds_;
    // 中心位置杂交出的种子
    Seed offspring_seed_;
    // 最好的种子的优劣程度
    int best_quality_{-100};
    // <四个基因生成的 key, 根据四个基因的权重生成的基因>
    std::unordered_map<uint64_t, AppConsts::GeneType> gene_key_map_;

    // 计算种子的质量，数值越大种子的基因越好
    int calcQuality(const Seed &) const;

    // 根据四个角落计算中心杂交出的种子基因
    Seed calcOffspringSeed(const std::array<std::shared_ptr<Seed>, 4> &);

    // 打印杂交结果
    void printBreedingResult(const std::array<std::shared_ptr<Seed>, 4> &,
                             const Seed &) const;

    // 四个基因（顺序无关）唯一映射成一个 uint64_t 整数键，方便用作哈希表
    // key，实现去重缓存（记忆化）
    uint64_t encodeGeneKeySorted(const AppConsts::GeneType &g1,
                                 const AppConsts::GeneType &g2,
                                 const AppConsts::GeneType &g3,
                                 const AppConsts::GeneType &g4);
};

#endif // GENECALCULATOR_H
