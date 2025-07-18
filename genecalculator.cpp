#include "genecalculator.h"
#include <random>
#include <unordered_map>

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

void GeneCalculator::calcOffspringSeed() {
    Seed result;
    // 随机数生成器（用于平票随机选择）
    static std::random_device rd;
    static std::mt19937 gen(rd());

    for (int i = 0; i < 6; ++i) {
        std::unordered_map<AppConsts::GeneType, double> weight;

        // 统计每种基因在该位的总权重
        for (const auto &p : breeding_seeds_) {
            if (!p)
                continue;
            auto g = p->genes_[i];
            double w = (g == AppConsts::GeneType::W || g == AppConsts::GeneType::X)
                           ? 1.2
                           : 1.0;
            weight[g] += w;
        }

        // 找最大权重
        double max_weight = -1.0;
        for (const auto &[g, w] : weight) {
            if (w > max_weight) {
                max_weight = w;
            }
        }

        // 收集权重最大的所有基因（可能多个）
        std::vector<AppConsts::GeneType> candidates;
        for (const auto &[g, w] : weight) {
            if (std::abs(w - max_weight) < 1e-6) {
                candidates.push_back(g);
            }
        }

        // 若候选基因只有一个，直接使用；多个则随机选择一个
        AppConsts::GeneType chosen;
        if (candidates.size() == 1) {
            chosen = candidates[0];
        } else {
            std::uniform_int_distribution<> dist(0, candidates.size() - 1);
            chosen = candidates[dist(gen)];
        }

        result.genes_[i] = chosen;
    }

    offspring_seed_ = result;
}

const std::array<std::shared_ptr<Seed>, 4> &
GeneCalculator::getBreedingSeeds() const {
    return breeding_seeds_;
}
