#pragma once
#ifndef MATH_HELPER_H
#define MATH_HELPER_H


// 自作マクロヘッター

//#define M_PI 3.141592653

// 度からラジアンへの変換マクロ
template<typename T>
constexpr auto DEG_TO_RAD(T deg) { return ((deg) * (M_PI / 180.0)); }

// ラジアンから度への変換マクロ
template<typename T>
constexpr auto RAD_TO_DEG(T rad) { return ((rad) * (180.0 / M_PI)); }

#endif // MATH_HELPER_H