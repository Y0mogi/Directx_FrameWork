#pragma once
#include "component.h"
#include <xaudio2.h>


struct AudioClip;

class Audio : public Component
{
public:
	static void InitMaster();
	static void UninitMaster();

	/// <summary>
	/// オーディオクリップの作成
	/// </summary>
	/// <param name="path">ファイルの相対パス</param>
	/// <param name="load">作ったクリップを保存するか</param>
	/// <returns>AudioClipポインタ</returns>
	[[nodiscard]] static AudioClip* CreateAudioClip(const char* path, bool load = false)noexcept(false);

	using Component::Component;

	void Uninit();

	/// <summary>
	/// サウンドファイルを読み込み保存する
	/// </summary>
	/// <param name="FileName">ファイルへの相対パス</param>
	/// <param name="tag">登録名</param>
	void Load(const char* FileName, const std::string& tag)noexcept(false);

	/// <summary>
	/// オーディオクリップを保存する
	/// </summary>
	/// <param name="clip">保存したいクリップのポインタ</param>
	void Load(AudioClip* clip)noexcept(false);

	/// <summary>
	/// 保存されているサウンドを再生する
	/// </summary>
	/// <param name="tag">登録名</param>
	/// <param name="Loop">ループするか</param>
	void Play(const std::string& tag, bool loop = false)noexcept(false);

	/// <summary>
	/// 保存されているサウンドを再生する
	/// </summary>
	/// <param name="clip">保存されているクリップのポインタ</param>
	/// <param name="loop">ループするか</param>
	void Play(AudioClip* clip, bool loop = false)noexcept(false);

	/// <summary>
	/// 指定した再生中の音を止める
	/// </summary>
	/// <param name="tag">登録名</param>
	void Stop(const std::string& tag)noexcept(false);

	/// <summary>
	/// 指定した再生中の音を止める
	/// </summary>
	/// <param name="clip">保存されているクリップのポインタ</param>
	void Stop(AudioClip* clip)noexcept(false);


	/// <summary>
	/// 現在の再生状態の取得
	/// </summary>
	/// <param name="tag">登録名</param>
	/// <returns>TRUE:再生中 FALSE:停止中</returns>
	bool IsSoundPlaying(const std::string& tag)noexcept(false);

	/// <summary>
	/// 現在の再生状態の取得
	/// </summary>
	/// <param name="clip">保存されているクリップのポインタ</param>
	/// <returns>TRUE:再生中 FALSE:停止中</returns>
	bool IsSoundPlaying(AudioClip* clip)noexcept(false);

private:
	/// <summary>
	/// clipが保存されているかの確認
	/// </summary>
	/// <param name="clip">ターゲット</param>
	/// <returns>TRUE:保存済み FALSE:未保存</returns>
	static bool CompareAudioClip(AudioClip* target)noexcept(false);

private:
	static IXAudio2*				m_Xaudio;
	static IXAudio2MasteringVoice*	m_MasteringVoice;
	static std::vector<AudioClip*>	m_AudioClips;
};

/// <summary>
/// オーディオクリップ構造体
/// </summary>
struct AudioClip 
{
	std::string				m_ClipName = "Default";	// クリップ名
	IXAudio2SourceVoice*	m_SourceVoice{};		// 
	BYTE*					m_SoundData{};			//
	int						m_PlayLength{};			// 
	int						m_Length{};				// 
};