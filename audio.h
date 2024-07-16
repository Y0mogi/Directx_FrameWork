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
	/// �I�[�f�B�I�N���b�v�̍쐬
	/// </summary>
	/// <param name="path">�t�@�C���̑��΃p�X</param>
	/// <param name="load">������N���b�v��ۑ����邩</param>
	/// <returns>AudioClip�|�C���^</returns>
	[[nodiscard]] static AudioClip* CreateAudioClip(const char* path, bool load = false)noexcept(false);

	using Component::Component;

	void Uninit();

	/// <summary>
	/// �T�E���h�t�@�C����ǂݍ��ݕۑ�����
	/// </summary>
	/// <param name="FileName">�t�@�C���ւ̑��΃p�X</param>
	/// <param name="tag">�o�^��</param>
	void Load(const char* FileName, const std::string& tag)noexcept(false);

	/// <summary>
	/// �I�[�f�B�I�N���b�v��ۑ�����
	/// </summary>
	/// <param name="clip">�ۑ��������N���b�v�̃|�C���^</param>
	void Load(AudioClip* clip)noexcept(false);

	/// <summary>
	/// �ۑ�����Ă���T�E���h���Đ�����
	/// </summary>
	/// <param name="tag">�o�^��</param>
	/// <param name="Loop">���[�v���邩</param>
	void Play(const std::string& tag, bool loop = false)noexcept(false);

	/// <summary>
	/// �ۑ�����Ă���T�E���h���Đ�����
	/// </summary>
	/// <param name="clip">�ۑ�����Ă���N���b�v�̃|�C���^</param>
	/// <param name="loop">���[�v���邩</param>
	void Play(AudioClip* clip, bool loop = false)noexcept(false);

	/// <summary>
	/// �w�肵���Đ����̉����~�߂�
	/// </summary>
	/// <param name="tag">�o�^��</param>
	void Stop(const std::string& tag)noexcept(false);

	/// <summary>
	/// �w�肵���Đ����̉����~�߂�
	/// </summary>
	/// <param name="clip">�ۑ�����Ă���N���b�v�̃|�C���^</param>
	void Stop(AudioClip* clip)noexcept(false);


	/// <summary>
	/// ���݂̍Đ���Ԃ̎擾
	/// </summary>
	/// <param name="tag">�o�^��</param>
	/// <returns>TRUE:�Đ��� FALSE:��~��</returns>
	bool IsSoundPlaying(const std::string& tag)noexcept(false);

	/// <summary>
	/// ���݂̍Đ���Ԃ̎擾
	/// </summary>
	/// <param name="clip">�ۑ�����Ă���N���b�v�̃|�C���^</param>
	/// <returns>TRUE:�Đ��� FALSE:��~��</returns>
	bool IsSoundPlaying(AudioClip* clip)noexcept(false);

private:
	/// <summary>
	/// clip���ۑ�����Ă��邩�̊m�F
	/// </summary>
	/// <param name="clip">�^�[�Q�b�g</param>
	/// <returns>TRUE:�ۑ��ς� FALSE:���ۑ�</returns>
	static bool CompareAudioClip(AudioClip* target)noexcept(false);

private:
	static IXAudio2*				m_Xaudio;
	static IXAudio2MasteringVoice*	m_MasteringVoice;
	static std::vector<AudioClip*>	m_AudioClips;
};

/// <summary>
/// �I�[�f�B�I�N���b�v�\����
/// </summary>
struct AudioClip 
{
	std::string				m_ClipName = "Default";	// �N���b�v��
	IXAudio2SourceVoice*	m_SourceVoice{};		// 
	BYTE*					m_SoundData{};			//
	int						m_PlayLength{};			// 
	int						m_Length{};				// 
};