//=============================================================================
// Contents   : 指定したクラスのインスタンスを生成するクラス
//              
// Author     : 髙橋 脩也
// LastUpdate : 2024/07/22
// Since      : 2024/07/22
//
// === 更新ログ ===
//
// 
// 
//
//=============================================================================
#pragma once
#include <memory>

/// <summary>
/// 指定したクラスのインスタンスを生成
/// </summary>
/// <typeparam name="T">生成したいクラス</typeparam>
template<typename T>
class TemplateFactory
{
public:
    /// <summary>
    /// 指定した引数でクラスのユニークポインタインスタンスを生成
    /// </summary>
    /// <typeparam name="Args">インスタンスのコンストラクタに渡す引数の型リスト</typeparam>
    /// <param name="args">インスタンス生成に使用する引数</param>
    /// <returns>指定したクラスのユニークポインタ</returns>
    template<typename... Args>
    static std::unique_ptr<T> CreateInstance(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    /// <summary>
    /// 指定した引数でクラスの共有ポインタインスタンスを生成
    /// </summary>
    /// <typeparam name="Args">インスタンスのコンストラクタに渡す引数の型リスト</typeparam>
    /// <param name="args">インスタンス生成に使用する引数</param>
    /// <returns>指定したクラスの共有ポインタ</returns>
    template<typename... Args>
    static std::shared_ptr<T> CreateInstance(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

};

