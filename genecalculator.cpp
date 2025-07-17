#include "genecalculator.h"

GeneCalculator::GeneCalculator() {
    // 预先分配容量，避免多次动态扩容
    seeds_.reserve(100);
}

void GeneCalculator::addSeed(Seed seed) {
    auto s = std::make_shared<Seed>(std::move(seed));
    // 如果已有会插入失败
    if (seeds_set_.insert(s).second) {
        seeds_.emplace_back(s);
    }
}

void GeneCalculator::clearSeeds() {
    seeds_set_.clear();
    seeds_.clear();
    breeding_seeds_.fill(nullptr);
    offspring_seed_ = Seed{};
}

void GeneCalculator::calculate() {}

const Seed &GeneCalculator::getOffspringSeed() const { return offspring_seed_; }

int GeneCalculator::calcQuality() const {
    int countG = 0, countY = 0, countH = 0, countW = 0, countX = 0;
    for (auto g : offspring_seed_.genes_) {
        switch (g) {
        case AppConsts::GeneType::G:
            ++countG;
            break;
        case AppConsts::GeneType::Y:
            ++countY;
            break;
        case AppConsts::GeneType::H:
            ++countH;
            break;
        case AppConsts::GeneType::W:
            ++countW;
            break;
        case AppConsts::GeneType::X:
            ++countX;
            break;
        }
    }

    // 若为最优组合 GGGYYY，直接返回最大分数
    if (countG == 3 && countY == 3)
        return 100;

    // 否则按权重计算评分
    int score = countG * 2 + countY * 3 + countH * 1 - countW * 2 - countX * 1;
    return score;
}

void GeneCalculator::calcOffspringSeed() {}

const array<shared_ptr<Seed>, 4> &GeneCalculator::getBreedingSeeds() const {
    return breeding_seeds_;
}
