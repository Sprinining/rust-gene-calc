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

int GeneCalculator::calcQuality() const {}

void GeneCalculator::calcOffspringSeed() {}

const array<shared_ptr<Seed>, 4> &GeneCalculator::getBreedingSeeds() const {
    return breeding_seeds_;
}
