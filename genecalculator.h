#ifndef GENECALCULATOR_H
#define GENECALCULATOR_H

#include "seed.h"
#include <array>
#include <memory>
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

private:
    // 所有的种子
    std::vector<std::shared_ptr<Seed>> seeds_;
    // 用于对种子去重
    std::unordered_set<std::shared_ptr<Seed>> seeds_set_;
    // 四个角落用于杂交的种子
    std::array<std::shared_ptr<Seed>, 4> breeding_seeds_;
    // 中心位置杂交出的种子
    Seed offspring_seed_;

    // 计算种子的质量，数值越大种子的基因越好
    int calcQuality() const;

    // 根据四个角落计算中心杂交出的种子基因
    void calcOffspringSeed();
};

#endif // GENECALCULATOR_H
