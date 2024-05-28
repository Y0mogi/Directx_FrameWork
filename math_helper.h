#pragma once
#ifndef MATH_HELPER_H
#define MATH_HELPER_H


// 自作マクロヘッター

#define M_PI 3.141592653

// 度からラジアンへの変換マクロ
#define DEG_TO_RAD(deg) ((deg) * (M_PI / 180.0))

// ラジアンから度への変換マクロ
#define RAD_TO_DEG(rad) ((rad) * (180.0 / M_PI))

#endif // MATH_HELPER_H