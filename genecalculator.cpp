#include "genecalculator.h"
#include <QDebug>
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
    best_quality_ = -100;
}

void GeneCalculator::calculate() {
    std::array<std::shared_ptr<Seed>, 4> breeding_seeds;
    int n = seeds_.size();
    for (int i = 0; i < n; ++i) {
        breeding_seeds[0] = seeds_[i];
        for (int j = i + 1; j < n; ++j) {
            breeding_seeds[1] = seeds_[j];
            for (int k = j + 1; k < n; ++k) {
                breeding_seeds[2] = seeds_[k];
                for (int t = k + 1; t < n; ++t) {
                    breeding_seeds[3] = seeds_[t];
                    Seed offspring = calcOffspringSeed(breeding_seeds);
                    int quality = calcQuality(offspring);

                    // 打印杂交结果
                    printBreedingResult(breeding_seeds, offspring);

                    // 如果更优，保存当前组合
                    if (quality > best_quality_) {
                        best_quality_ = quality;
                        // 复制四个角落的引用
                        breeding_seeds_ = breeding_seeds;
                        offspring_seed_ = std::move(offspring);
                    }
                }
            }
        }
    }
}

const Seed &GeneCalculator::getOffspringSeed() const { return offspring_seed_; }

int GeneCalculator::calcQuality(const Seed &offspring) const {
    int countG = 0, countY = 0, countH = 0, countW = 0, countX = 0;
    for (auto g : offspring.genes_) {
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

Seed GeneCalculator::calcOffspringSeed(
    const std::array<std::shared_ptr<Seed>, 4> &breeding_seeds) const {
    Seed result;
    // 随机数生成器（用于平票随机选择）
    static std::random_device rd;
    static std::mt19937 gen(rd());

    for (int i = 0; i < 6; ++i) {
        std::unordered_map<AppConsts::GeneType, double> weight;

        // 统计每种基因在该位的总权重
        for (const auto &p : breeding_seeds) {
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

    // 虽然 result 是局部变量，但现代 C++
    // 编译器通常会直接在调用者栈上构造返回值（RVO）， 或在无法 RVO
    // 的情况下使用移动构造函数，因此此处不会发生实际拷贝。
    return result;
}

const std::array<std::shared_ptr<Seed>, 4> &
GeneCalculator::getBreedingSeeds() const {
    return breeding_seeds_;
}

char GeneCalculator::geneTypeToChar(AppConsts::GeneType g) {
    switch (g) {
    case AppConsts::GeneType::G:
        return 'G';
    case AppConsts::GeneType::Y:
        return 'Y';
    case AppConsts::GeneType::H:
        return 'H';
    case AppConsts::GeneType::W:
        return 'W';
    case AppConsts::GeneType::X:
        return 'X';
    default:
        return '?';
    }
}

void GeneCalculator::printBreedingResult(
    const std::array<std::shared_ptr<Seed>, 4> &breeding_seeds,
    const Seed &offspring) const {
    qDebug() << "=== Breeding Round ===";
    for (size_t i = 0; i < 4; ++i) {
        QString line = QString("Parent %1: ").arg(i + 1);
        if (breeding_seeds[i]) {
            for (const auto &gene : breeding_seeds[i]->genes_) {
                line += geneTypeToChar(gene);
                line += ' ';
            }
        } else {
            line += "(null)";
        }
        qDebug().noquote() << line;
    }
    QString offspringLine = "Offspring : ";
    for (const auto &gene : offspring.genes_) {
        offspringLine += geneTypeToChar(gene);
        offspringLine += ' ';
    }
    qDebug().noquote() << offspringLine;
}
