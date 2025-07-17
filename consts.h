#pragma once

namespace AppConsts {
// GeneType 枚举表示 Rust 游戏中种子的基因类型：
enum class GeneType {
    G, // Growth - 提高生长速度（绿色正面基因）
    Y, // Yield - 提高作物产量（绿色正面基因）
    H, // Hardiness - 增强温度耐受性 / 抵抗力（绿色正面基因）
    W, // Water Increase - 增加水分需求（红色负面基因）
    X  // Dummy / Empty - 占位，无效果，可被其他基因覆盖（红色负面基因）
};

} // namespace AppConsts
