//=============================================================================
// Contents   : DirectXMath�̃w���p�֐�
//              
// Author     : ���� ����
// LastUpdate : 2024/06/06
// Since      : 2024/06/01
// 
//   ���L������p
// https://qiita.com/HnniTns/items/6e7edc82775a86923cef
// https://qiita.com/HnniTns/items/f55102ca0bf4261028b3
//
// === �X�V���O ===
//
// 6 / 6 1340�s�ڂ����肩�疼�O��� XmfloatMath�̊֐���ǉ�
//
//
//=============================================================================

#pragma once
#ifndef XMFLOAT_HELPER_H
#define XMFLOAT_HELPER_H


#include <DirectXMath.h>
#include <initializer_list>
#include <array>
#include <cmath>
#include <cassert>
#include <limits>
#include <algorithm>
#include <numeric>


// float�͌덷����������̂ł��̊֐����`���Ă��܂�
template<typename Ty>
_NODISCARD static inline constexpr bool AdjEqual(const Ty epsilon_num, const Ty num)
{
    static_assert(std::is_floating_point<Ty>::value, "This type isn't floating point");

    // cmath��fabs��constexpr�ɑΉ����Ă��Ȃ��̂Ŏ蓮�ŏ����Ă��܂��B(���Ȃ��Ƃ�VS2019�ł�...)
    constexpr auto Fabs = [](const Ty num) constexpr {
        return (num >= static_cast<Ty>(0.0)) ? num : -num;
        };

    constexpr auto Epsilon = std::numeric_limits<Ty>::epsilon();
    auto dis = Fabs(epsilon_num - num);

    return (dis <= Epsilon);
}

//-------------------------------------------------------------------------------------------------------------
//. XMFLOAT4�n
//-------------------------------------------------------------------------------------------------------------

static inline void operator+= (DirectX::XMFLOAT4& v1, const DirectX::XMFLOAT4& v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
    v1.w += v2.w;
}

static inline void operator-= (DirectX::XMFLOAT4& v1, const DirectX::XMFLOAT4& v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
    v1.w -= v2.w;
}

static inline void operator/= (DirectX::XMFLOAT4& v1, const DirectX::XMFLOAT4& v2)
{
    v1.x /= v2.x;
    v1.y /= v2.y;
    v1.z /= v2.z;
    v1.w /= v2.w;
}

static inline void operator*= (DirectX::XMFLOAT4& v1, const DirectX::XMFLOAT4& v2)
{
    v1.x *= v2.x;
    v1.y *= v2.y;
    v1.z *= v2.z;
    v1.w *= v2.w;
}

static inline void operator%= (DirectX::XMFLOAT4& v1, const DirectX::XMFLOAT4& v2)
{
    v1.x = ::fmodf(v1.x, v2.x);
    v1.y = ::fmodf(v1.y, v2.y);
    v1.z = ::fmodf(v1.z, v2.z);
    v1.w = ::fmodf(v1.w, v2.w);
}

_NODISCARD static inline constexpr auto operator+ (const DirectX::XMFLOAT4& v1, const DirectX::XMFLOAT4& v2)
{
    return DirectX::XMFLOAT4{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
}

_NODISCARD static inline constexpr auto operator- (const DirectX::XMFLOAT4& v1, const DirectX::XMFLOAT4& v2)
{
    return DirectX::XMFLOAT4{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w };
}

_NODISCARD static inline constexpr auto operator* (const DirectX::XMFLOAT4& v1, const DirectX::XMFLOAT4& v2)
{
    return DirectX::XMFLOAT4{ v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w };
}

_NODISCARD static inline constexpr auto operator/ (const DirectX::XMFLOAT4& v1, const DirectX::XMFLOAT4& v2)
{
    return DirectX::XMFLOAT4{ v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w };
}

_NODISCARD static inline constexpr auto operator% (const DirectX::XMFLOAT4& v1, const DirectX::XMFLOAT4& v2)
{
    return DirectX::XMFLOAT4{ ::fmodf(v1.x, v2.x), ::fmodf(v1.y, v2.y), ::fmodf(v1.z, v2.z), ::fmodf(v1.w, v2.w) };
}

static inline void operator+= (DirectX::XMFLOAT4& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 4u && "�s���ȃC�j�V�����C�U�[���X�g");

    auto itr{ v2.begin() };
    v1.x += *itr;
    itr++;
    v1.y += *itr;
    itr++;
    v1.z += *itr;
    itr++;
    v1.w += *itr;
}

static inline void operator-= (DirectX::XMFLOAT4& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 4u && "�s���ȃC�j�V�����C�U�[���X�g");

    auto itr{ v2.begin() };
    v1.x -= *itr;
    itr++;
    v1.y -= *itr;
    itr++;
    v1.z -= *itr;
    itr++;
    v1.w -= *itr;
}

static inline void operator/= (DirectX::XMFLOAT4& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 4u && "�s���ȃC�j�V�����C�U�[���X�g");

    auto itr{ v2.begin() };
    v1.x /= *itr;
    itr++;
    v1.y /= *itr;
    itr++;
    v1.z /= *itr;
    itr++;
    v1.w /= *itr;
}

static inline void operator*= (DirectX::XMFLOAT4& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 4u && "�s���ȃC�j�V�����C�U�[���X�g");

    auto itr{ v2.begin() };
    v1.x *= *itr;
    itr++;
    v1.y *= *itr;
    itr++;
    v1.z *= *itr;
    itr++;
    v1.w *= *itr;
}

static inline void operator%= (DirectX::XMFLOAT4& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 4u && "�s���ȃC�j�V�����C�U�[���X�g");

    auto itr{ v2.begin() };
    v1.x = ::fmodf(v1.x, *itr);
    itr++;
    v1.y = ::fmodf(v1.y, *itr);
    itr++;
    v1.z = ::fmodf(v1.z, *itr);
    itr++;
    v1.w = ::fmodf(v1.w, *itr);
}

_NODISCARD static inline auto operator+ (const DirectX::XMFLOAT4& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 4u && "�s���ȃC�j�V�����C�U�[���X�g");

    DirectX::XMFLOAT4 temp{ v1 };
    auto itr{ v2.begin() };

    temp.x += *itr;
    itr++;
    temp.y += *itr;
    itr++;
    temp.z += *itr;
    itr++;
    temp.w += *itr;

    return temp;
}

_NODISCARD static inline auto operator- (const DirectX::XMFLOAT4& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 4u && "�s���ȃC�j�V�����C�U�[���X�g");

    DirectX::XMFLOAT4 temp{ v1 };
    auto itr{ v2.begin() };

    temp.x -= *itr;
    itr++;
    temp.y -= *itr;
    itr++;
    temp.z -= *itr;
    itr++;
    temp.w -= *itr;

    return temp;
}

_NODISCARD static inline auto operator* (const DirectX::XMFLOAT4& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 4u && "�s���ȃC�j�V�����C�U�[���X�g");

    DirectX::XMFLOAT4 temp{ v1 };
    auto itr{ v2.begin() };

    temp.x *= *itr;
    itr++;
    temp.y *= *itr;
    itr++;
    temp.z *= *itr;
    itr++;
    temp.w *= *itr;

    return temp;
}

_NODISCARD static inline auto operator/ (const DirectX::XMFLOAT4& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 4u && "�s���ȃC�j�V�����C�U�[���X�g");

    DirectX::XMFLOAT4 temp{ v1 };
    auto itr{ v2.begin() };

    temp.x /= *itr;
    itr++;
    temp.y /= *itr;
    itr++;
    temp.z /= *itr;
    itr++;
    temp.w /= *itr;

    return temp;
}

_NODISCARD static inline auto operator% (const DirectX::XMFLOAT4& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 4u && "�s���ȃC�j�V�����C�U�[���X�g");

    DirectX::XMFLOAT4 temp{ v1 };
    auto itr{ v2.begin() };

    temp.x = ::fmodf(v1.x, *itr);
    itr++;
    temp.y = ::fmodf(v1.y, *itr);
    itr++;
    temp.z = ::fmodf(v1.z, *itr);
    itr++;
    temp.w = ::fmodf(v1.w, *itr);

    return temp;
}

static inline void operator+= (DirectX::XMFLOAT4& v1, const float num)
{
    v1.x += num;
    v1.y += num;
    v1.z += num;
    v1.w += num;
}

static inline void operator-= (DirectX::XMFLOAT4& v1, const float num)
{
    v1.x -= num;
    v1.y -= num;
    v1.z -= num;
    v1.w -= num;
}

static inline void operator/= (DirectX::XMFLOAT4& v1, const float num)
{
    v1.x /= num;
    v1.y /= num;
    v1.z /= num;
    v1.w /= num;
}

static inline void operator*= (DirectX::XMFLOAT4& v1, const float num)
{
    v1.x *= num;
    v1.y *= num;
    v1.z *= num;
    v1.w *= num;
}

static inline void operator%= (DirectX::XMFLOAT4& v1, const float num)
{
    v1.x = ::fmodf(v1.x, num);
    v1.y = ::fmodf(v1.y, num);
    v1.z = ::fmodf(v1.z, num);
    v1.w = ::fmodf(v1.w, num);
}

_NODISCARD static inline constexpr auto operator+ (const DirectX::XMFLOAT4& v1, const float num)
{
    return DirectX::XMFLOAT4{ v1.x + num, v1.y + num, v1.z + num, v1.w + num };
}

_NODISCARD static inline constexpr auto operator- (const DirectX::XMFLOAT4& v1, const float num)
{
    return DirectX::XMFLOAT4{ v1.x - num, v1.y - num, v1.z - num, v1.w - num };
}

_NODISCARD static inline constexpr auto operator* (const DirectX::XMFLOAT4& v1, const float num)
{
    return DirectX::XMFLOAT4{ v1.x * num, v1.y * num, v1.z * num, v1.w * num };
}

_NODISCARD static inline constexpr auto operator/ (const DirectX::XMFLOAT4& v1, const float num)
{
    return DirectX::XMFLOAT4{ v1.x / num, v1.y / num, v1.z / num, v1.w / num };
}

_NODISCARD static inline constexpr auto operator% (const DirectX::XMFLOAT4& v1, const float num)
{
    return DirectX::XMFLOAT4{ ::fmodf(v1.x, num), ::fmodf(v1.y, num), ::fmodf(v1.z, num), ::fmodf(v1.w, num) };
}

_NODISCARD static inline constexpr auto operator+ (const float num, DirectX::XMFLOAT4& v1)
{
    return DirectX::XMFLOAT4{ num + v1.x, num + v1.y, num + v1.z, num + v1.w };
}

_NODISCARD static inline constexpr auto operator- (const float num, DirectX::XMFLOAT4& v1)
{
    return DirectX::XMFLOAT4{ num - v1.x, num - v1.y, num - v1.z, num - v1.w };
}

_NODISCARD static inline constexpr auto operator* (const float num, DirectX::XMFLOAT4& v1)
{
    return DirectX::XMFLOAT4{ num * v1.x, num * v1.y, num * v1.z, num * v1.w };
}

_NODISCARD static inline constexpr auto operator/ (const float num, DirectX::XMFLOAT4& v1)
{
    return DirectX::XMFLOAT4{ num / v1.x, num / v1.y, num / v1.z, num / v1.w };
}

_NODISCARD static inline constexpr auto operator% (const float num, DirectX::XMFLOAT4& v1)
{
    return DirectX::XMFLOAT4{ fmodf(num, v1.x), fmodf(num, v1.y), fmodf(num, v1.z), fmodf(num, v1.w) };
}

_NODISCARD static inline constexpr bool operator== (const DirectX::XMFLOAT4& v1, const DirectX::XMFLOAT4& v2)
{
    return (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z) && AdjEqual(v1.w, v2.w));
}

_NODISCARD static inline constexpr bool operator!= (const DirectX::XMFLOAT4& v1, const DirectX::XMFLOAT4& v2)
{
    return !(AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z) && AdjEqual(v1.w, v2.w));
}

_NODISCARD static inline constexpr bool operator== (const DirectX::XMFLOAT4& v1, const float num)
{
    return (AdjEqual(v1.x, num) && AdjEqual(v1.y, num) && AdjEqual(v1.z, num) && AdjEqual(v1.w, num));
}

_NODISCARD static inline constexpr bool operator!= (const DirectX::XMFLOAT4& v1, const float num)
{
    return !(AdjEqual(v1.x, num) && AdjEqual(v1.y, num) && AdjEqual(v1.z, num) && AdjEqual(v1.w, num));
}

_NODISCARD static inline constexpr bool operator< (const DirectX::XMFLOAT4& v1, const DirectX::XMFLOAT4& v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y) && (v1.z < v2.z) && (v1.w < v2.w));
}

_NODISCARD static inline constexpr bool operator> (const DirectX::XMFLOAT4& v1, const DirectX::XMFLOAT4& v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y) && (v1.z > v2.z) && (v1.w > v2.w));
}

_NODISCARD static inline constexpr bool operator<= (const DirectX::XMFLOAT4& v1, const DirectX::XMFLOAT4& v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y) && (v1.z < v2.z) && (v1.w < v2.w)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z) && AdjEqual(v1.w, v2.w));
}

_NODISCARD static inline constexpr bool operator>= (const DirectX::XMFLOAT4& v1, const DirectX::XMFLOAT4& v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y) && (v1.z > v2.z) && (v1.w > v2.w)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z) && AdjEqual(v1.w, v2.w));
}

_NODISCARD static inline constexpr auto operator- (const DirectX::XMFLOAT4& v1)
{
    return DirectX::XMFLOAT4{ -v1.x, -v1.y, -v1.z, -v1.w };
}

//-------------------------------------------------------------------------------------------------------------
//. XMFLOAT3�n
//-------------------------------------------------------------------------------------------------------------

static inline void operator+= (DirectX::XMFLOAT3& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 3u && "�s���ȃC�j�V�����C�U�[���X�g");

    auto itr{ v2.begin() };
    v1.x += *itr;
    itr++;
    v1.y += *itr;
    itr++;
    v1.z += *itr;
}

static inline void operator-= (DirectX::XMFLOAT3& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 3u && "�s���ȃC�j�V�����C�U�[���X�g");

    auto itr{ v2.begin() };
    v1.x -= *itr;
    itr++;
    v1.y -= *itr;
    itr++;
    v1.z -= *itr;
}

static inline void operator/= (DirectX::XMFLOAT3& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 3u && "�s���ȃC�j�V�����C�U�[���X�g");

    auto itr{ v2.begin() };
    v1.x /= *itr;
    itr++;
    v1.y /= *itr;
    itr++;
    v1.z /= *itr;
}

static inline void operator*= (DirectX::XMFLOAT3& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 3u && "�s���ȃC�j�V�����C�U�[���X�g");

    auto itr{ v2.begin() };
    v1.x *= *itr;
    itr++;
    v1.y *= *itr;
    itr++;
    v1.z *= *itr;
}

static inline void operator%= (DirectX::XMFLOAT3& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 3u && "�s���ȃC�j�V�����C�U�[���X�g");

    auto itr{ v2.begin() };
    v1.x = ::fmodf(v1.x, *itr);
    itr++;
    v1.y = ::fmodf(v1.y, *itr);
    itr++;
    v1.z = ::fmodf(v1.z, *itr);
}

_NODISCARD static inline auto operator+ (const DirectX::XMFLOAT3& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 3u && "�s���ȃC�j�V�����C�U�[���X�g");

    DirectX::XMFLOAT3 temp{ v1 };
    auto itr{ v2.begin() };

    temp.x += *itr;
    itr++;
    temp.y += *itr;
    itr++;
    temp.z += *itr;

    return temp;
}

_NODISCARD static inline auto operator- (const DirectX::XMFLOAT3& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 3u && "�s���ȃC�j�V�����C�U�[���X�g");

    DirectX::XMFLOAT3 temp{ v1 };
    auto itr{ v2.begin() };

    temp.x -= *itr;
    itr++;
    temp.y -= *itr;
    itr++;
    temp.z -= *itr;

    return temp;
}

_NODISCARD static inline auto operator* (const DirectX::XMFLOAT3& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 3u && "�s���ȃC�j�V�����C�U�[���X�g");

    DirectX::XMFLOAT3 temp{ v1 };
    auto itr{ v2.begin() };

    temp.x *= *itr;
    itr++;
    temp.y *= *itr;
    itr++;
    temp.z *= *itr;

    return temp;
}

_NODISCARD static inline auto operator/ (const DirectX::XMFLOAT3& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 3u && "�s���ȃC�j�V�����C�U�[���X�g");

    DirectX::XMFLOAT3 temp{ v1 };
    auto itr{ v2.begin() };

    temp.x /= *itr;
    itr++;
    temp.y /= *itr;
    itr++;
    temp.z /= *itr;

    return temp;
}

_NODISCARD static inline auto operator% (const DirectX::XMFLOAT3& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 3u && "�s���ȃC�j�V�����C�U�[���X�g");

    DirectX::XMFLOAT3 temp{ v1 };
    auto itr{ v2.begin() };

    temp.x = ::fmodf(v1.x, *itr);
    itr++;
    temp.y = ::fmodf(v1.y, *itr);
    itr++;
    temp.z = ::fmodf(v1.z, *itr);

    return temp;
}

static inline void operator+= (DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
}

static inline void operator-= (DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
}

static inline void operator*= (DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
    v1.x *= v2.x;
    v1.y *= v2.y;
    v1.z *= v2.z;
}

static inline void operator/= (DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
    v1.x /= v2.x;
    v1.y /= v2.y;
    v1.z /= v2.z;
}

static inline void operator%= (DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
    v1.x = ::fmodf(v1.x, v2.x);
    v1.y = ::fmodf(v1.y, v2.y);
    v1.z = ::fmodf(v1.z, v2.z);
}

_NODISCARD static inline constexpr auto operator+ (const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
    return DirectX::XMFLOAT3{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

_NODISCARD static inline constexpr auto operator- (const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
    return DirectX::XMFLOAT3{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

_NODISCARD static inline constexpr auto operator* (const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
    return DirectX::XMFLOAT3{ v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
}

_NODISCARD static inline constexpr auto operator/ (const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
    return DirectX::XMFLOAT3{ v1.x / v2.x, v1.y / v2.y, v1.z / v2.z };
}

_NODISCARD static inline constexpr auto operator% (const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
    return DirectX::XMFLOAT3{ ::fmodf(v1.x, v2.x), ::fmodf(v1.y, v2.y), ::fmodf(v1.z, v2.z) };
}

static inline void operator+= (DirectX::XMFLOAT3& v1, const float num)
{
    v1.x += num;
    v1.y += num;
    v1.z += num;
}

static inline void operator-= (DirectX::XMFLOAT3& v1, const float num)
{
    v1.x -= num;
    v1.y -= num;
    v1.z -= num;
}

static inline void operator/= (DirectX::XMFLOAT3& v1, const float num)
{
    v1.x /= num;
    v1.y /= num;
    v1.z /= num;
}

static inline void operator*= (DirectX::XMFLOAT3& v1, const float num)
{
    v1.x *= num;
    v1.y *= num;
    v1.z *= num;
}

static inline void operator%= (DirectX::XMFLOAT3& v1, const float num)
{
    v1.x = ::fmodf(v1.x, num);
    v1.y = ::fmodf(v1.y, num);
    v1.z = ::fmodf(v1.z, num);
}

_NODISCARD static inline constexpr auto operator+ (const DirectX::XMFLOAT3& v1, const float num)
{
    return DirectX::XMFLOAT3{ v1.x + num, v1.y + num, v1.z + num };
}

_NODISCARD static inline constexpr auto operator- (const DirectX::XMFLOAT3& v1, const float num)
{
    return DirectX::XMFLOAT3{ v1.x - num, v1.y - num, v1.z - num };
}

_NODISCARD static inline constexpr auto operator* (const DirectX::XMFLOAT3& v1, const float num)
{
    return DirectX::XMFLOAT3{ v1.x * num, v1.y * num, v1.z * num };
}

_NODISCARD static inline constexpr auto operator% (const DirectX::XMFLOAT3& v1, const float num)
{
    return DirectX::XMFLOAT3{ ::fmodf(v1.x, num), ::fmodf(v1.y, num), ::fmodf(v1.z, num) };
}

_NODISCARD static inline constexpr auto operator/ (const DirectX::XMFLOAT3& v1, const float num)
{
    return DirectX::XMFLOAT3{ v1.x / num, v1.y / num, v1.z / num };
}

_NODISCARD static inline constexpr auto operator+ (const float num, DirectX::XMFLOAT3& v1)
{
    return DirectX::XMFLOAT3{ num + v1.x, num - v1.y, num - v1.z };
}

_NODISCARD static inline constexpr auto operator- (const float num, DirectX::XMFLOAT3& v1)
{
    return DirectX::XMFLOAT3{ num - v1.x, num - v1.y, num - v1.z };
}

_NODISCARD static inline constexpr auto operator* (const float num, DirectX::XMFLOAT3& v1)
{
    return DirectX::XMFLOAT3{ num * v1.x, num - v1.y, num - v1.z };
}

_NODISCARD static inline constexpr auto operator/ (const float num, DirectX::XMFLOAT3& v1)
{
    return DirectX::XMFLOAT3{ num / v1.x, num / v1.y, num / v1.z };
}

_NODISCARD static inline constexpr auto operator% (const float num, DirectX::XMFLOAT3& v1)
{
    return DirectX::XMFLOAT3{ fmodf(num, v1.x), fmodf(num, v1.y), fmodf(num, v1.z) };
}

_NODISCARD static inline constexpr bool operator== (const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
    return (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z));
}

_NODISCARD static inline constexpr bool operator== (const DirectX::XMFLOAT3& v1, const float num)
{
    return (AdjEqual(v1.x, num) && AdjEqual(v1.y, num) && AdjEqual(v1.z, num));
}

_NODISCARD static inline constexpr bool operator!= (const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
    return !(AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z));
}

_NODISCARD static inline constexpr bool operator!= (const DirectX::XMFLOAT3& v1, const float num)
{
    return !(AdjEqual(v1.x, num) && AdjEqual(v1.y, num) && AdjEqual(v1.z, num));
}

_NODISCARD static inline constexpr bool operator< (const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y) && (v1.z < v2.z));
}

_NODISCARD static inline constexpr bool operator> (const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y) && (v1.z > v2.z));
}

_NODISCARD static inline constexpr bool operator<= (const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y) && (v1.z < v2.z)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z));
}

_NODISCARD static inline constexpr bool operator>= (const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y) && (v1.z > v2.z)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y) && AdjEqual(v1.z, v2.z));
}

_NODISCARD static inline constexpr auto operator- (const DirectX::XMFLOAT3& v1)
{
    return DirectX::XMFLOAT3{ -v1.x, -v1.y, -v1.z };
}

//-------------------------------------------------------------------------------------------------------------
//. XMFLOAT2�n
//-------------------------------------------------------------------------------------------------------------

static inline void operator+= (DirectX::XMFLOAT2& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 2u && "�s���ȃC�j�V�����C�U�[���X�g");

    auto itr{ v2.begin() };
    v1.x += *itr;
    itr++;
    v1.y += *itr;
}

static inline void operator-= (DirectX::XMFLOAT2& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 2u && "�s���ȃC�j�V�����C�U�[���X�g");

    auto itr{ v2.begin() };
    v1.x -= *itr;
    itr++;
    v1.y -= *itr;
}

static inline void operator/= (DirectX::XMFLOAT2& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 2u && "�s���ȃC�j�V�����C�U�[���X�g");

    auto itr{ v2.begin() };
    v1.x /= *itr;
    itr++;
    v1.y /= *itr;
}

static inline void operator*= (DirectX::XMFLOAT2& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 2u && "�s���ȃC�j�V�����C�U�[���X�g");

    auto itr{ v2.begin() };
    v1.x *= *itr;
    itr++;
    v1.y *= *itr;
}

static inline void operator%= (DirectX::XMFLOAT2& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 2u && "�s���ȃC�j�V�����C�U�[���X�g");

    auto itr{ v2.begin() };
    v1.x = ::fmodf(v1.x, *itr);
    itr++;
    v1.y = ::fmodf(v1.y, *itr);
}

_NODISCARD static inline auto operator+ (const DirectX::XMFLOAT2& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 2u && "�s���ȃC�j�V�����C�U�[���X�g");

    DirectX::XMFLOAT2 temp{ v1 };
    auto itr{ v2.begin() };

    temp.x += *itr;
    itr++;
    temp.y += *itr;

    return temp;
}

_NODISCARD static inline auto operator- (const DirectX::XMFLOAT2& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 2u && "�s���ȃC�j�V�����C�U�[���X�g");

    DirectX::XMFLOAT2 temp{ v1 };
    auto itr{ v2.begin() };

    temp.x -= *itr;
    itr++;
    temp.y -= *itr;

    return temp;
}

_NODISCARD static inline auto operator* (const DirectX::XMFLOAT2& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 2u && "�s���ȃC�j�V�����C�U�[���X�g");

    DirectX::XMFLOAT2 temp{ v1 };
    auto itr{ v2.begin() };

    temp.x *= *itr;
    itr++;
    temp.y *= *itr;

    return temp;
}

_NODISCARD static inline auto operator/ (const DirectX::XMFLOAT2& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 2u && "�s���ȃC�j�V�����C�U�[���X�g");

    DirectX::XMFLOAT2 temp{ v1 };
    auto itr{ v2.begin() };

    temp.x /= *itr;
    itr++;
    temp.y /= *itr;

    return temp;
}

_NODISCARD static inline auto operator% (const DirectX::XMFLOAT2& v1, const std::initializer_list<float>& v2)
{
    assert(v2.size() == 2u && "�s���ȃC�j�V�����C�U�[���X�g");

    DirectX::XMFLOAT2 temp{ v1 };
    auto itr{ v2.begin() };

    temp.x = ::fmodf(v1.x, *itr);
    itr++;
    temp.y = ::fmodf(v1.y, *itr);

    return temp;
}

static inline void operator+= (DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
}

static inline void operator-= (DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
}

static inline void operator/= (DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
    v1.x /= v2.x;
    v1.y /= v2.y;
}

static inline void operator*= (DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
    v1.x *= v2.x;
    v1.y *= v2.y;
}

static inline void operator%= (DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
    v1.x = ::fmodf(v1.x, v2.x);
    v1.y = ::fmodf(v1.y, v2.y);
}

_NODISCARD static inline constexpr auto operator+ (const DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
    return DirectX::XMFLOAT2{ v1.x + v2.x, v1.y + v2.y };
}

_NODISCARD static inline constexpr auto operator- (const DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
    return DirectX::XMFLOAT2{ v1.x - v2.x, v1.y - v2.y };
}

_NODISCARD static inline constexpr auto operator* (const DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
    return DirectX::XMFLOAT2{ v1.x * v2.x, v1.y * v2.y };
}

_NODISCARD static inline constexpr auto operator/ (const DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
    return DirectX::XMFLOAT2{ v1.x / v2.x, v1.y / v2.y };
}

_NODISCARD static inline constexpr auto operator% (const DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
    return DirectX::XMFLOAT2{ ::fmodf(v1.x, v2.x), ::fmodf(v1.y, v2.y) };
}

static inline void operator+= (DirectX::XMFLOAT2& v1, const float num)
{
    v1.x += num;
    v1.y += num;
}

static inline void operator-= (DirectX::XMFLOAT2& v1, const float num)
{
    v1.x -= num;
    v1.y -= num;
}

static inline void operator/= (DirectX::XMFLOAT2& v1, const float num)
{
    v1.x /= num;
    v1.y /= num;
}

static inline void operator*= (DirectX::XMFLOAT2& v1, const float num)
{
    v1.x *= num;
    v1.y *= num;
}

static inline void operator%= (DirectX::XMFLOAT2& v1, const float num)
{
    v1.x = ::fmodf(v1.x, num);
    v1.y = ::fmodf(v1.y, num);
}

_NODISCARD static inline constexpr auto operator+ (const DirectX::XMFLOAT2& v1, const float num)
{
    return DirectX::XMFLOAT2{ v1.x + num, v1.y + num };
}

_NODISCARD static inline constexpr auto operator- (const DirectX::XMFLOAT2& v1, const float num)
{
    return DirectX::XMFLOAT2{ v1.x - num, v1.y - num };
}

_NODISCARD static inline constexpr auto operator* (const DirectX::XMFLOAT2& v1, const float num)
{
    return DirectX::XMFLOAT2{ v1.x * num, v1.y * num };
}

_NODISCARD static inline constexpr auto operator/ (const DirectX::XMFLOAT2& v1, const float num)
{
    return DirectX::XMFLOAT2{ v1.x / num, v1.y / num };
}

_NODISCARD static inline constexpr auto operator% (const DirectX::XMFLOAT2& v1, const float num)
{
    return DirectX::XMFLOAT2{ ::fmodf(v1.x, num), ::fmodf(v1.y, num) };
}

_NODISCARD static inline constexpr auto operator+ (const float num, DirectX::XMFLOAT2& v1)
{
    return DirectX::XMFLOAT2{ num + v1.x, num + v1.y };
}

_NODISCARD static inline constexpr auto operator- (const float num, DirectX::XMFLOAT2& v1)
{
    return DirectX::XMFLOAT2{ num - v1.x, num - v1.y };
}

_NODISCARD static inline constexpr auto operator* (const float num, DirectX::XMFLOAT2& v1)
{
    return DirectX::XMFLOAT2{ num * v1.x, num * v1.y };
}

_NODISCARD static inline constexpr auto operator/ (const float num, DirectX::XMFLOAT2& v1)
{
    return DirectX::XMFLOAT2{ num / v1.x, num / v1.y };
}

_NODISCARD static inline constexpr auto operator% (const float num, DirectX::XMFLOAT2& v1)
{
    return DirectX::XMFLOAT2{ fmodf(num, v1.x), fmodf(num, v1.y) };
}

_NODISCARD static inline constexpr bool operator== (const DirectX::XMFLOAT2& v1, const float num)
{
    return (AdjEqual(v1.x, num) && AdjEqual(v1.y, num));
}

_NODISCARD static inline constexpr bool operator!= (const DirectX::XMFLOAT2& v1, const float num)
{
    return !(AdjEqual(v1.x, num) && AdjEqual(v1.y, num));
}

_NODISCARD static inline constexpr bool operator== (const DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
    return (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y));
}

_NODISCARD static inline constexpr bool operator!= (const DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
    return !(AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y));
}

_NODISCARD static inline constexpr bool operator< (const DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y));
}

_NODISCARD static inline constexpr bool operator> (const DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y));
}

_NODISCARD static inline constexpr bool operator<= (const DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
    return ((v1.x < v2.x) && (v1.y < v2.y)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y));
}

_NODISCARD static inline constexpr bool operator>= (const DirectX::XMFLOAT2& v1, const DirectX::XMFLOAT2& v2)
{
    return ((v1.x > v2.x) && (v1.y > v2.y)) || (AdjEqual(v1.x, v2.x) && AdjEqual(v1.y, v2.y));
}

_NODISCARD static inline constexpr auto operator- (const DirectX::XMFLOAT2& v1)
{
    return DirectX::XMFLOAT2{ -v1.x, -v1.y };
}

//-------------------------------------------------------------------------------------------------------------
// �ϊ��֐��n
//-------------------------------------------------------------------------------------------------------------

//. XMVECTOR�ϊ�-----------------------------------------------------------------------------------------------

_NODISCARD static inline auto ToXMVECTOR(const DirectX::XMFLOAT4& vec)
{
    return DirectX::XMLoadFloat4(&vec);
}

_NODISCARD static inline auto ToXMVECTOR(const DirectX::XMFLOAT3& vec)
{
    return DirectX::XMLoadFloat3(&vec);
}

_NODISCARD static inline auto ToXMVECTOR(const DirectX::XMFLOAT2& vec)
{
    return DirectX::XMLoadFloat2(&vec);
}

_NODISCARD static inline auto ToXMVECTOR(const float vec)
{
    return DirectX::XMLoadFloat(&vec);
}

//. XMVECTOR�ϊ�(���K��)---------------------------------------------------------------------------------------

_NODISCARD static inline auto ToNormalizeXMVECTOR(const DirectX::XMFLOAT4& vec)
{
    return DirectX::XMVector4Normalize(DirectX::XMLoadFloat4(&vec));
}

_NODISCARD static inline auto ToNormalizeXMVECTOR(const DirectX::XMFLOAT3& vec)
{
    return DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&vec));
}

_NODISCARD static inline auto ToNormalizeXMVECTOR(const DirectX::XMFLOAT2& vec)
{
    return DirectX::XMVector2Normalize(DirectX::XMLoadFloat2(&vec));
}

//. XMFLOAT�Ȃǂɕϊ�-----------------------------------------------------------------------------------------------

_NODISCARD static inline auto ToXMFLOAT4(const DirectX::XMVECTOR& vec)
{
    DirectX::XMFLOAT4 rv;

    DirectX::XMStoreFloat4(&rv, vec);

    return rv;
}

_NODISCARD static inline auto ToXMFLOAT3(const DirectX::XMVECTOR& vec)
{
    DirectX::XMFLOAT3 rv;

    DirectX::XMStoreFloat3(&rv, vec);

    return rv;
}

_NODISCARD static inline auto ToXMFLOAT2(const DirectX::XMVECTOR& vec)
{
    DirectX::XMFLOAT2 rv;

    DirectX::XMStoreFloat2(&rv, vec);

    return rv;
}

_NODISCARD static inline auto ToXMFLOAT1(const DirectX::XMVECTOR& vec)
{
    float rv;

    DirectX::XMStoreFloat(&rv, vec);

    return rv;
}

_NODISCARD static inline auto ToXMFLOAT(const DirectX::XMINT4& vec)
{
    DirectX::XMFLOAT4 rv{ (float)vec.x, (float)vec.y, (float)vec.z, (float)vec.w };

    return rv;
}

_NODISCARD static inline auto ToXMFLOAT(const DirectX::XMINT3& vec)
{
    DirectX::XMFLOAT3 rv{ (float)vec.x, (float)vec.y, (float)vec.z };

    return rv;
}

_NODISCARD static inline auto ToXMFLOAT(const DirectX::XMINT2& vec)
{
    DirectX::XMFLOAT2 rv{ (float)vec.x, (float)vec.y };

    return rv;
}

_NODISCARD static inline auto ToXMINT(const DirectX::XMFLOAT4& vec)
{
    DirectX::XMINT4 rv{ (int)vec.x, (int)vec.y, (int)vec.z, (int)vec.w };

    return rv;
}

_NODISCARD static inline auto ToXMINT(const DirectX::XMFLOAT3& vec)
{
    DirectX::XMINT3 rv{ (int)vec.x, (int)vec.y, (int)vec.z };

    return rv;
}

_NODISCARD static inline auto ToXMINT(const DirectX::XMFLOAT2& vec)
{
    DirectX::XMINT2 rv{ (int)vec.x, (int)vec.y };

    return rv;
}

_NODISCARD static inline auto ToXMFLOAT(const DirectX::XMUINT4& vec)
{
    DirectX::XMFLOAT4 rv{ (float)vec.x, (float)vec.y, (float)vec.z, (float)vec.w };

    return rv;
}

_NODISCARD static inline auto ToXMFLOAT(const DirectX::XMUINT3& vec)
{
    DirectX::XMFLOAT3 rv{ (float)vec.x, (float)vec.y, (float)vec.z };

    return rv;
}

_NODISCARD static inline auto ToXMFLOAT(const DirectX::XMUINT2& vec)
{
    DirectX::XMFLOAT2 rv{ (float)vec.x, (float)vec.y };

    return rv;
}

_NODISCARD static inline auto ToXMUINT(const DirectX::XMFLOAT4& vec)
{
    DirectX::XMUINT4 rv{ (uint32_t)vec.x, (uint32_t)vec.y, (uint32_t)vec.z, (uint32_t)vec.w };

    return rv;
}

_NODISCARD static inline auto ToXMUINT(const DirectX::XMFLOAT3& vec)
{
    DirectX::XMUINT3 rv{ (uint32_t)vec.x, (uint32_t)vec.y, (uint32_t)vec.z };

    return rv;
}

_NODISCARD static inline auto ToXMUINT(const DirectX::XMFLOAT2& vec)
{
    DirectX::XMUINT2 rv{ (uint32_t)vec.x, (uint32_t)vec.y };

    return rv;
}

_NODISCARD static inline auto ToXMFLOAT(const std::array<float, 4>& vec)
{
    DirectX::XMFLOAT4 rv{ vec.front(), vec.at(1), vec.at(2), vec.back() };

    return rv;
}

_NODISCARD static inline auto ToXMFLOAT(const std::array<float, 3>& vec)
{
    DirectX::XMFLOAT3 rv{ vec.front(), vec.at(1), vec.back() };

    return rv;
}

_NODISCARD static inline auto ToXMFLOAT(const std::array<float, 2>& vec)
{
    DirectX::XMFLOAT2 rv{ vec.front(), vec.back() };

    return rv;
}

_NODISCARD static inline auto ToArray(const DirectX::XMFLOAT4& vec)
{
    std::array<float, 4> rv{ vec.x, vec.y, vec.z, vec.w };

    return rv;
}

_NODISCARD static inline auto ToArray(const DirectX::XMFLOAT3& vec)
{
    std::array<float, 3> rv{ vec.x, vec.y, vec.z };

    return rv;
}

_NODISCARD static inline auto ToArray(const DirectX::XMFLOAT2& vec)
{
    std::array<float, 2> rv{ vec.x, vec.y };

    return rv;
}

//. XMMATRIX�ϊ�-----------------------------------------------------------------------------------------

_NODISCARD static inline auto ToXMMATRIX(const DirectX::XMFLOAT4X4& vec)
{
    return DirectX::XMLoadFloat4x4(&vec);
}

_NODISCARD static inline auto ToXMMATRIX(const DirectX::XMFLOAT3X3& vec)
{
    return DirectX::XMLoadFloat3x3(&vec);
}

_NODISCARD static inline auto ToXMMATRIX(const DirectX::XMFLOAT3X4& vec)
{
    return DirectX::XMLoadFloat3x4(&vec);
}

_NODISCARD static inline auto ToXMMATRIX(const DirectX::XMFLOAT4X3& vec)
{
    return DirectX::XMLoadFloat4x3(&vec);
}

//. XMFLOAT�ϊ�-----------------------------------------------------------------------------------------------

_NODISCARD static inline auto ToXMFLOAT4X4(const DirectX::XMMATRIX& mat)
{
    DirectX::XMFLOAT4X4 rv;

    DirectX::XMStoreFloat4x4(&rv, mat);

    return rv;
}

_NODISCARD static inline auto ToXMFLOAT3X3(const DirectX::XMMATRIX& mat)
{
    DirectX::XMFLOAT3X3 rv;

    DirectX::XMStoreFloat3x3(&rv, mat);

    return rv;
}

_NODISCARD static inline auto ToXMFLOAT3X4(const DirectX::XMMATRIX& mat)
{
    DirectX::XMFLOAT3X4 rv;

    DirectX::XMStoreFloat3x4(&rv, mat);

    return rv;
}

_NODISCARD static inline auto ToXMFLOAT4X3(const DirectX::XMMATRIX& mat)
{
    DirectX::XMFLOAT4X3 rv;

    DirectX::XMStoreFloat4x3(&rv, mat);

    return rv;
}

//-------------------------------------------------------------------------------------------------------------
// ����ϊ��֐�
//-------------------------------------------------------------------------------------------------------------

// �^�グ-----------------------------------------------------------------------------------------------

_NODISCARD static inline auto RaiseToXMFLOAT4(const DirectX::XMFLOAT3& vec, const float w_component = 0.f)
{
    return DirectX::XMFLOAT4{ vec.x, vec.y, vec.z, w_component };
}

_NODISCARD static inline auto RaiseToXMFLOAT4(const DirectX::XMFLOAT2& vec, const float z_component = 0.f, const float w_component = 0.f)
{
    return DirectX::XMFLOAT4{ vec.x, vec.y, z_component, w_component };
}

_NODISCARD static inline auto RaiseToXMFLOAT3(const DirectX::XMFLOAT2& vec, const float z_component = 0.f)
{
    return DirectX::XMFLOAT3{ vec.x, vec.y, z_component };
}

// �^����-----------------------------------------------------------------------------------------------

// W, Z�������؂�̂Ă���
_NODISCARD static inline auto LowerToXMFLOAT2(const DirectX::XMFLOAT4& vec)
{
    return DirectX::XMFLOAT2{ vec.x, vec.y };
}

// Z�������؂�̂Ă���
_NODISCARD static inline auto LowerToXMFLOAT2(const DirectX::XMFLOAT3& vec)
{
    return DirectX::XMFLOAT2{ vec.x, vec.y };
}

// W�������؂�̂Ă���
_NODISCARD static inline auto LowerToXMFLOAT3(const DirectX::XMFLOAT4& vec)
{
    return DirectX::XMFLOAT3{ vec.x, vec.y, vec.z };
}

// 2024/06/06 �ǉ�

inline namespace XmfloatMath
{
    // �N�����v---------------------------------------------------------------------------------------------------
    _NODISCARD static inline auto Clamp(const DirectX::XMFLOAT2& vf2, const DirectX::XMFLOAT2& low, const DirectX::XMFLOAT2& high)
    {
        DirectX::XMFLOAT2 temp{};

        temp.x = std::clamp(vf2.x, low.x, high.x);
        temp.y = std::clamp(vf2.y, low.y, high.y);

        return temp;
    }

    _NODISCARD static inline auto Clamp(const DirectX::XMFLOAT3& vf3, const DirectX::XMFLOAT3& low, const DirectX::XMFLOAT3& high)
    {
        DirectX::XMFLOAT3 temp{};

        temp.x = std::clamp(vf3.x, low.x, high.x);
        temp.y = std::clamp(vf3.y, low.y, high.y);
        temp.z = std::clamp(vf3.z, low.z, high.z);

        return temp;
    }

    _NODISCARD static inline auto Clamp(const DirectX::XMFLOAT4& vf4, const DirectX::XMFLOAT4& low, const DirectX::XMFLOAT4& high)
    {
        DirectX::XMFLOAT4 temp{};

        temp.x = std::clamp(vf4.x, low.x, high.x);
        temp.y = std::clamp(vf4.y, low.y, high.y);
        temp.z = std::clamp(vf4.z, low.z, high.z);
        temp.w = std::clamp(vf4.w, low.w, high.w);

        return temp;
    }

    _NODISCARD static inline auto Clamp(const DirectX::XMFLOAT2& vf2, const float low, const float high)
    {
        DirectX::XMFLOAT2 temp{};

        temp.x = std::clamp(vf2.x, low, high);
        temp.y = std::clamp(vf2.y, low, high);

        return temp;
    }

    _NODISCARD static inline auto Clamp(const DirectX::XMFLOAT3& vf3, const float low, const float high)
    {
        DirectX::XMFLOAT3 temp{};

        temp.x = std::clamp(vf3.x, low, high);
        temp.y = std::clamp(vf3.y, low, high);
        temp.z = std::clamp(vf3.z, low, high);

        return temp;
    }

    _NODISCARD static inline auto Clamp(const DirectX::XMFLOAT4& vf4, const float low, const float high)
    {
        DirectX::XMFLOAT4 temp{};

        temp.x = std::clamp(vf4.x, low, high);
        temp.y = std::clamp(vf4.y, low, high);
        temp.z = std::clamp(vf4.z, low, high);
        temp.w = std::clamp(vf4.w, low, high);

        return temp;
    }

    // �}�b�N�X---------------------------------------------------------------------------------------------------
    _NODISCARD static inline auto Max(const DirectX::XMFLOAT2& left, const DirectX::XMFLOAT2& right)
    {
        DirectX::XMFLOAT2 temp{};

        temp.x = (std::max)(left.x, right.x);
        temp.y = (std::max)(left.y, right.y);

        return temp;
    }

    _NODISCARD static inline auto Max(const DirectX::XMFLOAT3& left, const DirectX::XMFLOAT3& right)
    {
        DirectX::XMFLOAT3 temp{};

        temp.x = (std::max)(left.x, right.x);
        temp.y = (std::max)(left.y, right.y);
        temp.z = (std::max)(left.z, right.z);

        return temp;
    }

    _NODISCARD static inline auto Max(const DirectX::XMFLOAT4& left, const DirectX::XMFLOAT4& right)
    {
        DirectX::XMFLOAT4 temp{};

        temp.x = (std::max)(left.x, right.x);
        temp.y = (std::max)(left.y, right.y);
        temp.z = (std::max)(left.z, right.z);
        temp.w = (std::max)(left.w, right.w);

        return temp;
    }

    _NODISCARD static inline auto Max(const DirectX::XMFLOAT2& left, const float right)
    {
        DirectX::XMFLOAT2 temp{};

        temp.x = (std::max)(left.x, right);
        temp.y = (std::max)(left.y, right);

        return temp;
    }

    _NODISCARD static inline auto Max(const DirectX::XMFLOAT3& left, const float right)
    {
        DirectX::XMFLOAT3 temp{};

        temp.x = (std::max)(left.x, right);
        temp.y = (std::max)(left.y, right);
        temp.z = (std::max)(left.z, right);

        return temp;
    }

    _NODISCARD static inline auto Max(const DirectX::XMFLOAT4& left, const float right)
    {
        DirectX::XMFLOAT4 temp{};

        temp.x = (std::max)(left.x, right);
        temp.y = (std::max)(left.y, right);
        temp.z = (std::max)(left.z, right);
        temp.w = (std::max)(left.w, right);

        return temp;
    }

    // �~�j---------------------------------------------------------------------------------------------------
    _NODISCARD static inline auto Min(const DirectX::XMFLOAT2& left, const DirectX::XMFLOAT2& right)
    {
        DirectX::XMFLOAT2 temp{};

        temp.x = (std::min)(left.x, right.x);
        temp.y = (std::min)(left.y, right.y);

        return temp;
    }

    _NODISCARD static inline auto Min(const DirectX::XMFLOAT3& left, const DirectX::XMFLOAT3& right)
    {
        DirectX::XMFLOAT3 temp{};

        temp.x = (std::min)(left.x, right.x);
        temp.y = (std::min)(left.y, right.y);
        temp.z = (std::min)(left.z, right.z);

        return temp;
    }

    _NODISCARD static inline auto Min(const DirectX::XMFLOAT4& left, const DirectX::XMFLOAT4& right)
    {
        DirectX::XMFLOAT4 temp{};

        temp.x = (std::min)(left.x, right.x);
        temp.y = (std::min)(left.y, right.y);
        temp.z = (std::min)(left.z, right.z);
        temp.w = (std::min)(left.w, right.w);

        return temp;
    }

    _NODISCARD static inline auto Min(const DirectX::XMFLOAT2& left, const float right)
    {
        DirectX::XMFLOAT2 temp{};

        temp.x = (std::min)(left.x, right);
        temp.y = (std::min)(left.y, right);

        return temp;
    }

    _NODISCARD static inline auto Min(const DirectX::XMFLOAT3& left, const float right)
    {
        DirectX::XMFLOAT3 temp{};

        temp.x = (std::min)(left.x, right);
        temp.y = (std::min)(left.y, right);
        temp.z = (std::min)(left.z, right);

        return temp;
    }

    _NODISCARD static inline auto Min(const DirectX::XMFLOAT4& left, const float right)
    {
        DirectX::XMFLOAT4 temp{};

        temp.x = (std::min)(left.x, right);
        temp.y = (std::min)(left.y, right);
        temp.z = (std::min)(left.z, right);
        temp.w = (std::min)(left.w, right);

        return temp;
    }

    // 2 ���Ƃ���w���֐�---------------------------------------------------------------------------------------------------
    // 2 �� ���� ���Ԃ��B

    _NODISCARD static inline auto Exp2(const DirectX::XMFLOAT2& vf2)
    {
        DirectX::XMFLOAT2 temp{};

        temp.x = exp2f(vf2.x);
        temp.y = exp2f(vf2.y);

        return temp;
    }

    _NODISCARD static inline auto Exp2(const DirectX::XMFLOAT3& vf3)
    {
        DirectX::XMFLOAT3 temp{};

        temp.x = exp2f(vf3.x);
        temp.y = exp2f(vf3.y);
        temp.z = exp2f(vf3.z);

        return temp;
    }

    _NODISCARD static inline auto Exp2(const DirectX::XMFLOAT4& vf4)
    {
        DirectX::XMFLOAT4 temp{};

        temp.x = exp2f(vf4.x);
        temp.y = exp2f(vf4.y);
        temp.z = exp2f(vf4.z);
        temp.w = exp2f(vf4.w);

        return temp;
    }

    // 2 ���Ƃ����i�ΐ�---------------------------------------------------------------------------------------------------
    // ���� �� 2 ���Ƃ����i�ΐ���Ԃ��B�i�������Q�̉��悩��Ԃ��j

    _NODISCARD static inline auto Log2(const DirectX::XMFLOAT2& vf2)
    {
        DirectX::XMFLOAT2 temp{};

        temp.x = log2f(vf2.x);
        temp.y = log2f(vf2.y);

        return temp;
    }

    _NODISCARD static inline auto Log2(const DirectX::XMFLOAT3& vf3)
    {
        DirectX::XMFLOAT3 temp{};

        temp.x = log2f(vf3.x);
        temp.y = log2f(vf3.y);
        temp.z = log2f(vf3.z);

        return temp;
    }

    _NODISCARD static inline auto Log2(const DirectX::XMFLOAT4& vf4)
    {
        DirectX::XMFLOAT4 temp{};

        temp.x = log2f(vf4.x);
        temp.y = log2f(vf4.y);
        temp.z = log2f(vf4.z);
        temp.w = log2f(vf4.w);

        return temp;
    }

    // 2 ���Ƃ����i�ΐ�---------------------------------------------------------------------------------------------------
    // ���� x �� 10 ���Ƃ����p�ΐ���Ԃ��B(������10�̉��悩��Ԃ�)

    _NODISCARD static inline auto Log10(const DirectX::XMFLOAT2& vf2)
    {
        DirectX::XMFLOAT2 temp{};

        temp.x = log10f(vf2.x);
        temp.y = log10f(vf2.y);

        return temp;
    }

    _NODISCARD static inline auto Log10(const DirectX::XMFLOAT3& vf3)
    {
        DirectX::XMFLOAT3 temp{};

        temp.x = log10f(vf3.x);
        temp.y = log10f(vf3.y);
        temp.z = log10f(vf3.z);

        return temp;
    }

    _NODISCARD static inline auto Log10(const DirectX::XMFLOAT4& vf4)
    {
        DirectX::XMFLOAT4 temp{};

        temp.x = log10f(vf4.x);
        temp.y = log10f(vf4.y);
        temp.z = log10f(vf4.z);
        temp.w = log10f(vf4.w);

        return temp;
    }

    // �؂�グ---------------------------------------------------------------------------------------------------

    _NODISCARD static inline auto Ceil(const DirectX::XMFLOAT2& vf2)
    {
        DirectX::XMFLOAT2 temp{};

        temp.x = ceilf(vf2.x);
        temp.y = ceilf(vf2.y);

        return temp;
    }

    _NODISCARD static inline auto Ceil(const DirectX::XMFLOAT3& vf3)
    {
        DirectX::XMFLOAT3 temp{};

        temp.x = ceilf(vf3.x);
        temp.y = ceilf(vf3.y);
        temp.z = ceilf(vf3.z);

        return temp;
    }

    _NODISCARD static inline auto Ceil(const DirectX::XMFLOAT4& vf4)
    {
        DirectX::XMFLOAT4 temp{};

        temp.x = ceilf(vf4.x);
        temp.y = ceilf(vf4.y);
        temp.z = ceilf(vf4.z);
        temp.w = ceilf(vf4.w);

        return temp;
    }

    // �؂�̂�---------------------------------------------------------------------------------------------------

    _NODISCARD static inline auto Floor(const DirectX::XMFLOAT2& vf2)
    {
        DirectX::XMFLOAT2 temp{};

        temp.x = floorf(vf2.x);
        temp.y = floorf(vf2.y);

        return temp;
    }

    _NODISCARD static inline auto Floor(const DirectX::XMFLOAT3& vf3)
    {
        DirectX::XMFLOAT3 temp{};

        temp.x = floorf(vf3.x);
        temp.y = floorf(vf3.y);
        temp.z = floorf(vf3.z);

        return temp;
    }

    _NODISCARD static inline auto Floor(const DirectX::XMFLOAT4& vf4)
    {
        DirectX::XMFLOAT4 temp{};

        temp.x = floorf(vf4.x);
        temp.y = floorf(vf4.y);
        temp.z = floorf(vf4.z);
        temp.w = floorf(vf4.w);

        return temp;
    }

    // �[�������ւ̊ۂ�---------------------------------------------------------------------------------------------------

    _NODISCARD static inline auto Trunc(const DirectX::XMFLOAT2& vf2)
    {
        DirectX::XMFLOAT2 temp{};

        temp.x = truncf(vf2.x);
        temp.y = truncf(vf2.y);

        return temp;
    }

    _NODISCARD static inline auto Trunc(const DirectX::XMFLOAT3& vf3)
    {
        DirectX::XMFLOAT3 temp{};

        temp.x = truncf(vf3.x);
        temp.y = truncf(vf3.y);
        temp.z = truncf(vf3.z);

        return temp;
    }

    _NODISCARD static inline auto Trunc(const DirectX::XMFLOAT4& vf4)
    {
        DirectX::XMFLOAT4 temp{};

        temp.x = truncf(vf4.x);
        temp.y = truncf(vf4.y);
        temp.z = truncf(vf4.z);
        temp.w = truncf(vf4.w);

        return temp;
    }

    // �l�̌ܓ�---------------------------------------------------------------------------------------------------

    _NODISCARD static inline auto Round(const DirectX::XMFLOAT2& vf2)
    {
        DirectX::XMFLOAT2 temp{};

        temp.x = roundf(vf2.x);
        temp.y = roundf(vf2.y);

        return temp;
    }

    _NODISCARD static inline auto Round(const DirectX::XMFLOAT3& vf3)
    {
        DirectX::XMFLOAT3 temp{};

        temp.x = roundf(vf3.x);
        temp.y = roundf(vf3.y);
        temp.z = roundf(vf3.z);

        return temp;
    }

    _NODISCARD static inline auto Round(const DirectX::XMFLOAT4& vf4)
    {
        DirectX::XMFLOAT4 temp{};

        temp.x = roundf(vf4.x);
        temp.y = roundf(vf4.y);
        temp.z = roundf(vf4.z);
        temp.w = roundf(vf4.w);

        return temp;
    }

    //. XMFFLOAT�̑S�Ă̕ϐ��ɓ����l����-------------------------------------------------------------------------

    _NODISCARD static constexpr inline auto Fill2(const float num)
    {
        DirectX::XMFLOAT2 temp{ num, num };

        return temp;
    }

    _NODISCARD static constexpr inline auto Fill3(const float num)
    {
        DirectX::XMFLOAT3 temp{ num, num, num };

        return temp;
    }

    _NODISCARD static constexpr inline auto Fill4(const float num)
    {
        DirectX::XMFLOAT4 temp{ num, num, num, num };

        return temp;
    }

    // �ݏ�---------------------------------------------------------------------------------------------------

    _NODISCARD static inline auto Pow(const DirectX::XMFLOAT2& vf2, const float pow_num)
    {
        DirectX::XMFLOAT2 temp{};

        temp.x = powf(vf2.x, pow_num);
        temp.y = powf(vf2.y, pow_num);

        return temp;
    }

    _NODISCARD static inline auto Pow(const DirectX::XMFLOAT3& vf3, const float pow_num)
    {
        DirectX::XMFLOAT3 temp{};

        temp.x = powf(vf3.x, pow_num);
        temp.y = powf(vf3.y, pow_num);
        temp.z = powf(vf3.z, pow_num);

        return temp;
    }

    _NODISCARD static inline auto Pow(const DirectX::XMFLOAT4& vf4, const float pow_num)
    {
        DirectX::XMFLOAT4 temp{};

        temp.x = powf(vf4.x, pow_num);
        temp.y = powf(vf4.y, pow_num);
        temp.z = powf(vf4.z, pow_num);
        temp.w = powf(vf4.w, pow_num);

        return temp;
    }

    // ������---------------------------------------------------------------------------------------------------

    _NODISCARD static inline auto Sqrt(const DirectX::XMFLOAT2& vf2)
    {
        DirectX::XMFLOAT2 temp{};

        temp.x = sqrtf(vf2.x);
        temp.y = sqrtf(vf2.y);

        return temp;
    }

    _NODISCARD static inline auto Sqrt(const DirectX::XMFLOAT3& vf3)
    {
        DirectX::XMFLOAT3 temp{};

        temp.x = sqrtf(vf3.x);
        temp.y = sqrtf(vf3.y);
        temp.z = sqrtf(vf3.z);

        return temp;
    }

    _NODISCARD static inline auto Sqrt(const DirectX::XMFLOAT4& vf4)
    {
        DirectX::XMFLOAT4 temp{};

        temp.x = sqrtf(vf4.x);
        temp.y = sqrtf(vf4.y);
        temp.z = sqrtf(vf4.z);
        temp.w = sqrtf(vf4.w);

        return temp;
    }

    // ������---------------------------------------------------------------------------------------------------

    _NODISCARD static inline auto Cbrt(const DirectX::XMFLOAT2& vf2)
    {
        DirectX::XMFLOAT2 temp{};

        temp.x = cbrtf(vf2.x);
        temp.y = cbrtf(vf2.y);

        return temp;
    }

    _NODISCARD static inline auto Cbrt(const DirectX::XMFLOAT3& vf3)
    {
        DirectX::XMFLOAT3 temp{};

        temp.x = cbrtf(vf3.x);
        temp.y = cbrtf(vf3.y);
        temp.z = cbrtf(vf3.z);

        return temp;
    }

    _NODISCARD static inline auto Cbrt(const DirectX::XMFLOAT4& vf4)
    {
        DirectX::XMFLOAT4 temp{};

        temp.x = cbrtf(vf4.x);
        temp.y = cbrtf(vf4.y);
        temp.z = cbrtf(vf4.z);
        temp.w = cbrtf(vf4.w);

        return temp;
    }

    // ��Βl---------------------------------------------------------------------------------------------------

    _NODISCARD static inline auto FAbs(const DirectX::XMFLOAT2& vf2)
    {
        DirectX::XMFLOAT2 temp{};

        temp.x = fabs(vf2.x);
        temp.y = fabs(vf2.y);

        return temp;
    }

    _NODISCARD static inline auto FAbs(const DirectX::XMFLOAT3& vf3)
    {
        DirectX::XMFLOAT3 temp{};

        temp.x = fabs(vf3.x);
        temp.y = fabs(vf3.y);
        temp.z = fabs(vf3.z);

        return temp;
    }

    _NODISCARD static inline auto FAbs(const DirectX::XMFLOAT4& vf4)
    {
        DirectX::XMFLOAT4 temp{};

        temp.x = fabs(vf4.x);
        temp.y = fabs(vf4.y);
        temp.z = fabs(vf4.z);
        temp.w = fabs(vf4.w);

        return temp;
    }

    // �O�N���A�[--------------------------------------------------------------------------------------------------

    static inline void Clear(DirectX::XMFLOAT2& vf2)
    {
        vf2 = { 0.f, 0.f };
    }

    static inline void Clear(DirectX::XMFLOAT3& vf3)
    {
        vf3 = { 0.f, 0.f, 0.f };
    }

    static inline void Clear(DirectX::XMFLOAT4& vf4)
    {
        vf4 = { 0.f, 0.f, 0.f, 0.f };
    }

    // ���K��-----------------------------------------------------------------------------------------------------

    _NODISCARD static inline auto VectorNormalize(const DirectX::XMFLOAT2& vf2)
    {
        DirectX::XMFLOAT2 rv{ vf2 };

        auto&& vec{ DirectX::XMLoadFloat2(&rv) };

        DirectX::XMStoreFloat2(&rv, DirectX::XMVector2Normalize(vec));

        return rv;
    }

    _NODISCARD static inline auto VectorNormalize(const DirectX::XMFLOAT3& vf3)
    {
        DirectX::XMFLOAT3 rv{ vf3 };

        auto&& vec{ DirectX::XMLoadFloat3(&rv) };

        DirectX::XMStoreFloat3(&rv, DirectX::XMVector3Normalize(vec));

        return rv;
    }

    _NODISCARD static inline auto VectorNormalize(const DirectX::XMFLOAT4& vf4)
    {
        DirectX::XMFLOAT4 rv{ vf4 };

        auto&& vec{ DirectX::XMLoadFloat4(&rv) };

        DirectX::XMStoreFloat4(&rv, DirectX::XMVector4Normalize(vec));

        return rv;
    }

    // �x�N�g���̒������擾----------------------------------------------------------------------------------------------------

    _NODISCARD static inline float VectorLength(const DirectX::XMFLOAT2& vf2)
    {
        float len{};

        auto&& vec{ DirectX::XMLoadFloat2(&vf2) };

        DirectX::XMStoreFloat(&len, DirectX::XMVector2Length(vec));

        return len;
    }

    _NODISCARD static inline float VectorLength(const DirectX::XMFLOAT3& vf3)
    {
        float len{};

        auto&& vec{ DirectX::XMLoadFloat3(&vf3) };

        DirectX::XMStoreFloat(&len, DirectX::XMVector3Length(vec));

        return len;
    }

    _NODISCARD static inline float VectorLength(const DirectX::XMFLOAT4& vf4)
    {
        float len{};

        auto&& vec{ DirectX::XMLoadFloat4(&vf4) };

        DirectX::XMStoreFloat(&len, DirectX::XMVector4Length(vec));

        return len;
    }

    // �x�N�g���̒����̓����擾----------------------------------------------------------------------------------------------------

    _NODISCARD static inline float VectorLengthSq(const DirectX::XMFLOAT2& vf2)
    {
        float len{};

        auto&& vec{ DirectX::XMLoadFloat2(&vf2) };

        DirectX::XMStoreFloat(&len, DirectX::XMVector2LengthSq(vec));

        return len;
    }

    _NODISCARD static inline float VectorLengthSq(const DirectX::XMFLOAT3& vf3)
    {
        float len{};

        auto&& vec{ DirectX::XMLoadFloat3(&vf3) };

        DirectX::XMStoreFloat(&len, DirectX::XMVector3LengthSq(vec));

        return len;
    }

    _NODISCARD static inline float VectorLengthSq(const DirectX::XMFLOAT4& vf4)
    {
        float len{};

        auto&& vec{ DirectX::XMLoadFloat4(&vf4) };

        DirectX::XMStoreFloat(&len, DirectX::XMVector4LengthSq(vec));

        return len;
    }

    // �x�N�g���̒��s����x�N�g�����v�Z----------------------------------------------------------------------------------------------------

    _NODISCARD static inline auto VectorOrthogonal(const DirectX::XMFLOAT2& vf2)
    {
        DirectX::XMFLOAT2 rv{};

        auto&& vec{ DirectX::XMLoadFloat2(&vf2) };

        DirectX::XMStoreFloat2(&rv, DirectX::XMVector2Orthogonal(vec));

        return rv;
    }

    _NODISCARD static inline auto VectorOrthogonal(const DirectX::XMFLOAT3& vf3)
    {
        DirectX::XMFLOAT3 rv{};

        auto&& vec{ DirectX::XMLoadFloat3(&vf3) };

        DirectX::XMStoreFloat3(&rv, DirectX::XMVector3Orthogonal(vec));

        return rv;
    }

    _NODISCARD static inline auto VectorOrthogonal(const DirectX::XMFLOAT4& vf4)
    {
        DirectX::XMFLOAT4 rv{};

        auto&& vec{ DirectX::XMLoadFloat4(&vf4) };

        DirectX::XMStoreFloat4(&rv, DirectX::XMVector4Orthogonal(vec));

        return rv;
    }

    // �x�N�g���̓��ς��v�Z----------------------------------------------------------------------------------------------------

    _NODISCARD static inline auto VectorDot(const DirectX::XMFLOAT2& vec1, const DirectX::XMFLOAT2& vec2)
    {
        float rv{};

        const auto&& v1{ DirectX::XMLoadFloat2(&vec1) }, && v2{ DirectX::XMLoadFloat2(&vec2) };

        DirectX::XMStoreFloat(&rv, DirectX::XMVector2Dot(v1, v2));

        return rv;
    }

    _NODISCARD static inline auto VectorDot(const DirectX::XMFLOAT3& vec1, const DirectX::XMFLOAT3& vec2)
    {
        float rv{};

        const auto&& v1{ DirectX::XMLoadFloat3(&vec1) }, && v2{ DirectX::XMLoadFloat3(&vec2) };

        DirectX::XMStoreFloat(&rv, DirectX::XMVector3Dot(v1, v2));

        return rv;
    }

    _NODISCARD static inline auto VectorDot(const DirectX::XMFLOAT4& vec1, const DirectX::XMFLOAT4& vec2)
    {
        float rv{};

        const auto&& v1{ DirectX::XMLoadFloat4(&vec1) }, && v2{ DirectX::XMLoadFloat4(&vec2) };

        DirectX::XMStoreFloat(&rv, DirectX::XMVector4Dot(v1, v2));

        return rv;
    }

    // �x�N�g���̊O�ς��v�Z----------------------------------------------------------------------------------------------------

    _NODISCARD static inline auto VectorCross(const DirectX::XMFLOAT2& vec1, const DirectX::XMFLOAT2& vec2)
    {
        float rv{};

        const auto&& v1{ DirectX::XMLoadFloat2(&vec1) }, && v2{ DirectX::XMLoadFloat2(&vec2) };

        DirectX::XMStoreFloat(&rv, DirectX::XMVector2Cross(v1, v2));

        return rv;
    }

    _NODISCARD static inline auto VectorCross(const DirectX::XMFLOAT3& vec1, const DirectX::XMFLOAT3& vec2)
    {
        DirectX::XMFLOAT3 rv{};

        const auto&& v1{ DirectX::XMLoadFloat3(&vec1) }, && v2{ DirectX::XMLoadFloat3(&vec2) };

        DirectX::XMStoreFloat3(&rv, DirectX::XMVector3Cross(v1, v2));

        return rv;
    }

    // �x�N�g���Ԃ̃��W�A���p�x���v�Z-----------------------------------------------------------------------------------------

    _NODISCARD static inline float AngleBetweenVectors(const DirectX::XMFLOAT2& vf2_left, const DirectX::XMFLOAT2& vf2_right)
    {
        float rad{};

        auto&& l_vec{ DirectX::XMLoadFloat2(&vf2_left) }, && r_vec{ DirectX::XMLoadFloat2(&vf2_right) };

        DirectX::XMStoreFloat(&rad, DirectX::XMVector2AngleBetweenVectors(l_vec, r_vec));

        return rad;
    }

    _NODISCARD static inline float AngleBetweenVectors(const DirectX::XMFLOAT3& vf3_left, const DirectX::XMFLOAT3& vf3_right)
    {
        float rad{};

        auto&& l_vec{ DirectX::XMLoadFloat3(&vf3_left) }, && r_vec{ DirectX::XMLoadFloat3(&vf3_right) };

        DirectX::XMStoreFloat(&rad, DirectX::XMVector3AngleBetweenVectors(l_vec, r_vec));

        return rad;
    }

    _NODISCARD static inline float AngleBetweenVectors(const DirectX::XMFLOAT4& vf4_left, const DirectX::XMFLOAT4& vf4_right)
    {
        float rad{};

        auto&& l_vec{ DirectX::XMLoadFloat4(&vf4_left) }, && r_vec{ DirectX::XMLoadFloat4(&vf4_right) };

        DirectX::XMStoreFloat(&rad, DirectX::XMVector4AngleBetweenVectors(l_vec, r_vec));

        return rad;
    }

    // �x�N�g���Ԃ̃��W�A���p�x���v�Z-----------------------------------------------------------------------------------------

    _NODISCARD static inline float AngleBetweenNormals(const DirectX::XMFLOAT2& vf2_left, const DirectX::XMFLOAT2& vf2_right)
    {
        float rad{};

        auto&& l_vec{ DirectX::XMLoadFloat2(&vf2_left) }, && r_vec{ DirectX::XMLoadFloat2(&vf2_right) };

        DirectX::XMStoreFloat(&rad, DirectX::XMVector2AngleBetweenNormals(l_vec, r_vec));

        return rad;
    }

    _NODISCARD static inline float AngleBetweenNormals(const DirectX::XMFLOAT3& vf3_left, const DirectX::XMFLOAT3& vf3_right)
    {
        float rad{};

        auto&& l_vec{ DirectX::XMLoadFloat3(&vf3_left) }, && r_vec{ DirectX::XMLoadFloat3(&vf3_right) };

        DirectX::XMStoreFloat(&rad, DirectX::XMVector3AngleBetweenNormals(l_vec, r_vec));

        return rad;
    }

    _NODISCARD static inline float AngleBetweenNormals(const DirectX::XMFLOAT4& vf4_left, const DirectX::XMFLOAT4& vf4_right)
    {
        float rad{};

        auto&& l_vec{ DirectX::XMLoadFloat4(&vf4_left) }, && r_vec{ DirectX::XMLoadFloat4(&vf4_right) };

        DirectX::XMStoreFloat(&rad, DirectX::XMVector4AngleBetweenNormals(l_vec, r_vec));

        return rad;
    }

    // �ʖ@���v�Z-----------------------------------------------------------------------------------------

    _NODISCARD static inline auto SurfaceNormal(
        const DirectX::XMFLOAT3& base_pos, const DirectX::XMFLOAT3& pos1, const DirectX::XMFLOAT3& pos2)
    {
        using DirectX::XMLoadFloat3;

        DirectX::XMFLOAT3 normal{};
        const DirectX::XMFLOAT3&& vec1{ pos1.x - base_pos.x, pos1.y - base_pos.y, pos1.z - base_pos.z };
        const DirectX::XMFLOAT3&& vec2{ pos2.x - base_pos.x, pos2.y - base_pos.y, pos2.z - base_pos.z };

        DirectX::XMStoreFloat3(&normal,
            DirectX::XMVector3Normalize(DirectX::XMVector3Cross(XMLoadFloat3(&vec1), XMLoadFloat3(&vec2))));

        return normal;
    }

    // �p�x�̒P�ʂ�ύX-----------------------------------------------------------------------------------------

    _NODISCARD static inline constexpr auto ToRadian(const DirectX::XMFLOAT3& degree)
    {
        DirectX::XMFLOAT3 rv{};

        rv.x = DirectX::XMConvertToRadians(degree.x);
        rv.y = DirectX::XMConvertToRadians(degree.y);
        rv.z = DirectX::XMConvertToRadians(degree.z);

        return rv;
    }

    _NODISCARD static inline constexpr auto ToRadian(const DirectX::XMFLOAT2& degree)
    {
        DirectX::XMFLOAT2 rv{};

        rv.x = DirectX::XMConvertToRadians(degree.x);
        rv.y = DirectX::XMConvertToRadians(degree.y);

        return rv;
    }

    _NODISCARD static inline constexpr auto ToDegrees(const DirectX::XMFLOAT3& radian)
    {
        DirectX::XMFLOAT3 rv{};

        rv.x = DirectX::XMConvertToDegrees(radian.x);
        rv.y = DirectX::XMConvertToDegrees(radian.y);
        rv.z = DirectX::XMConvertToDegrees(radian.z);

        return rv;
    }

    _NODISCARD static inline constexpr auto ToDegrees(const DirectX::XMFLOAT2& radian)
    {
        DirectX::XMFLOAT2 rv{};

        rv.x = DirectX::XMConvertToDegrees(radian.x);
        rv.y = DirectX::XMConvertToDegrees(radian.y);

        return rv;
    }

    _NODISCARD static inline constexpr auto ToRadian(const float deg_x, const float deg_y, const float deg_z)
    {
        DirectX::XMFLOAT3 rv{};

        rv.x = DirectX::XMConvertToRadians(deg_x);
        rv.y = DirectX::XMConvertToRadians(deg_y);
        rv.z = DirectX::XMConvertToRadians(deg_z);

        return rv;
    }

    _NODISCARD static inline constexpr auto ToRadian(const float deg_x, const float deg_y)
    {
        DirectX::XMFLOAT2 rv{};

        rv.x = DirectX::XMConvertToRadians(deg_x);
        rv.y = DirectX::XMConvertToRadians(deg_y);

        return rv;
    }

    _NODISCARD static inline constexpr auto ToDegrees(const float rad_x, const float rad_y, const float rad_z)
    {
        DirectX::XMFLOAT3 rv{};

        rv.x = DirectX::XMConvertToDegrees(rad_x);
        rv.y = DirectX::XMConvertToDegrees(rad_y);
        rv.z = DirectX::XMConvertToDegrees(rad_z);

        return rv;
    }

    _NODISCARD static inline constexpr auto ToDegrees(const float rad_x, const float rad_y)
    {
        DirectX::XMFLOAT2 rv{};

        rv.x = DirectX::XMConvertToDegrees(rad_x);
        rv.y = DirectX::XMConvertToDegrees(rad_y);

        return rv;
    }

    // ����W���瑼�̍��W�ւ̊p�x�����߂�--------------------------------------------------------------

    _NODISCARD static inline float PosToRadian(const DirectX::XMFLOAT2& base_pos,
        const DirectX::XMFLOAT2& another_pos)
    {
        return ::atan2f(another_pos.x - base_pos.x, another_pos.y - base_pos.y);
    }

    // ���������߂�-------------------------------------------------------------------------------

    _NODISCARD static inline float Distance(const DirectX::XMFLOAT2& pos1, const DirectX::XMFLOAT2& pos2)
    {
        return (::sqrtf(::powf(pos2.x - pos1.x, 2.f) + ::powf(pos2.y - pos1.y, 2.f)));
    }

    _NODISCARD static inline float Distance(const DirectX::XMFLOAT3& pos1, const DirectX::XMFLOAT3& pos2)
    {
        return (::sqrtf(::powf(pos2.x - pos1.x, 2.f) + ::powf(pos2.y - pos1.y, 2.f) + ::powf(pos2.z - pos1.z, 2.f)));
    }

    // �����̓������߂�-------------------------------------------------------------------------------

    _NODISCARD static inline float DistanceSq(const DirectX::XMFLOAT3& pos1, const DirectX::XMFLOAT3& pos2)
    {
        return (::powf(pos2.x - pos1.x, 2.f) + ::powf(pos2.y - pos1.y, 2.f) + ::powf(pos2.z - pos1.z, 2.f));
    }

    _NODISCARD static inline float DistanceSq(const DirectX::XMFLOAT2& pos1, const DirectX::XMFLOAT2& pos2)
    {
        return (::powf(pos2.x - pos1.x, 2.f) + ::powf(pos2.y - pos1.y, 2.f));
    }

    // ��_�Ԃ̒��_�����߂�----------------------------------------------------------------------------------

    _NODISCARD static inline DirectX::XMFLOAT2 MiddlePoint(const DirectX::XMFLOAT2& pos1, const DirectX::XMFLOAT2& pos2)
    {
        return { ((pos1.x + pos2.x) / 2.f), ((pos1.y + pos2.y) / 2.f) };
    }

    _NODISCARD static inline DirectX::XMFLOAT3 MiddlePoint(const DirectX::XMFLOAT3& pos1, const DirectX::XMFLOAT3& pos2)
    {
        return  { ((pos1.x + pos2.x) / 2.f), ((pos1.y + pos2.y) / 2.f), ((pos1.x + pos2.x) / 2.f) };
    }

    // ��_�Ԃ̋��������߂�i�ȈՔŁj--------------------------------------------------------------------------

    _NODISCARD static inline float EasyDistance(const DirectX::XMFLOAT3& pos1, const DirectX::XMFLOAT3& pos2)
    {
        auto FAbs{ [](const auto& vf3, const auto& vf3b) {
            DirectX::XMFLOAT3 temp{};

            temp.x = fabs(vf3.x - vf3b.x);
            temp.y = fabs(vf3.y - vf3b.y);
            temp.z = fabs(vf3.z - vf3b.z);

            return temp;
        } };

        const auto&& dis_vec3{ FAbs(pos2, pos1) };

        return (dis_vec3.x + dis_vec3.y + dis_vec3.z);
    }

    _NODISCARD static inline float EasyDistance(const DirectX::XMFLOAT2& pos1, const DirectX::XMFLOAT2& pos2)
    {
        auto FAbs{ [](const auto& vf3, const auto& vf3b) {
            DirectX::XMFLOAT2 temp{};

            temp.x = fabs(vf3.x - vf3b.x);
            temp.y = fabs(vf3.y - vf3b.y);

            return temp;
        } };

        const auto&& dis_vec3{ FAbs(pos2, pos1) };

        return (dis_vec3.x + dis_vec3.y);
    }

    // �l���덷���܂񂾏�œ������ǂ���-----------------------------------------------------------------------

    _NODISCARD static inline bool NearyEqual(
        const DirectX::XMFLOAT2& vec1, const DirectX::XMFLOAT2& vec2, const float error_num)
    {
        auto FAbs{ [](const auto& vf3, const auto& vf3b)
        {
            DirectX::XMFLOAT2 temp{};

            temp.x = fabs(vf3.x - vf3b.x);
            temp.y = fabs(vf3.y - vf3b.y);

            return temp;
        } };

        auto dis{ FAbs(vec1, vec2) };

        return (dis.x <= error_num && dis.y <= error_num);
    }

    _NODISCARD static inline bool NearyEqual(
        const DirectX::XMFLOAT3& vec1, const DirectX::XMFLOAT3& vec2, const float error_num)
    {
        auto FAbs{ [](const auto& vf3, const auto& vf3b)
        {
            DirectX::XMFLOAT3 temp{};

            temp.x = fabs(vf3.x - vf3b.x);
            temp.y = fabs(vf3.y - vf3b.y);
            temp.z = fabs(vf3.z - vf3b.z);

            return temp;
        } };

        auto dis{ FAbs(vec1, vec2) };

        return (dis.x <= error_num && dis.y <= error_num && dis.z <= error_num);
    }

    _NODISCARD static inline bool NearyEqual(
        const DirectX::XMFLOAT4& vec1, const DirectX::XMFLOAT4& vec2, const float error_num)
    {
        auto FAbs{ [](const auto& vf3, const auto& vf3b)
        {
            DirectX::XMFLOAT4 temp{};

            temp.x = fabs(vf3.x - vf3b.x);
            temp.y = fabs(vf3.y - vf3b.y);
            temp.z = fabs(vf3.z - vf3b.z);
            temp.z = fabs(vf3.w - vf3b.w);

            return temp;
        } };

        auto dis{ FAbs(vec1, vec2) };

        return (dis.x <= error_num && dis.y <= error_num && dis.z <= error_num && dis.w <= error_num);
    }
}



#endif // !XMFLOAT_HELPER_H