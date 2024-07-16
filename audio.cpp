
#include "main.h"
#include <string>
#include <vector>
#include "audio.h"


IXAudio2*				Audio::m_Xaudio = NULL;
IXAudio2MasteringVoice*	Audio::m_MasteringVoice = NULL;
std::vector<AudioClip*>	Audio::m_AudioClips;

void Audio::InitMaster()
{
	// COM������
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio����
	XAudio2Create(&m_Xaudio, 0);

	// �}�X�^�����O�{�C�X����
	m_Xaudio->CreateMasteringVoice(&m_MasteringVoice);
}


void Audio::UninitMaster()
{
	m_MasteringVoice->DestroyVoice();
	m_Xaudio->Release();
	CoUninitialize();
}

AudioClip* Audio::CreateAudioClip(const char* path,bool load)
{
	AudioClip* tmp = new AudioClip;
	// �T�E���h�f�[�^�Ǎ�
	WAVEFORMATEX wfx = { 0 };

	{
		HMMIO hmmio = NULL;
		MMIOINFO mmioinfo = { 0 };
		MMCKINFO riffchunkinfo = { 0 };
		MMCKINFO datachunkinfo = { 0 };
		MMCKINFO mmckinfo = { 0 };
		UINT32 buflen;
		LONG readlen;


		hmmio = mmioOpen((LPSTR)path, &mmioinfo, MMIO_READ);
		assert(hmmio);

		riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmioDescend(hmmio, &riffchunkinfo, NULL, MMIO_FINDRIFF);

		mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend(hmmio, &mmckinfo, &riffchunkinfo, MMIO_FINDCHUNK);

		if (mmckinfo.cksize >= sizeof(WAVEFORMATEX))
		{
			mmioRead(hmmio, (HPSTR)&wfx, sizeof(wfx));
		}
		else
		{
			PCMWAVEFORMAT pcmwf = { 0 };
			mmioRead(hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));
			memset(&wfx, 0x00, sizeof(wfx));
			memcpy(&wfx, &pcmwf, sizeof(pcmwf));
			wfx.cbSize = 0;
		}
		mmioAscend(hmmio, &mmckinfo, 0);

		datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmioDescend(hmmio, &datachunkinfo, &riffchunkinfo, MMIO_FINDCHUNK);



		buflen = datachunkinfo.cksize;
		tmp->m_SoundData = new unsigned char[buflen];
		readlen = mmioRead(hmmio, (HPSTR)tmp->m_SoundData, buflen);


		tmp->m_Length = readlen;
		tmp->m_PlayLength = readlen / wfx.nBlockAlign;


		mmioClose(hmmio, 0);
	}


	// �T�E���h�\�[�X����
	m_Xaudio->CreateSourceVoice(&tmp->m_SourceVoice, &wfx);
	assert(tmp->m_SourceVoice);

	// �}�b�v�ɒǉ�
	if (!CompareAudioClip(tmp) && load) {
		m_AudioClips.push_back(tmp);
		return tmp;
	}
	else
	{
		tmp->m_SourceVoice->DestroyVoice();
		delete tmp->m_SoundData;
		delete tmp;
		return nullptr;
	}
	return tmp;
}



void Audio::Load(const char* FileName, const std::string& tag)
{
	AudioClip* tmp = new AudioClip;
	// �T�E���h�f�[�^�Ǎ�
	WAVEFORMATEX wfx = { 0 };

	{
		HMMIO hmmio = NULL;
		MMIOINFO mmioinfo = { 0 };
		MMCKINFO riffchunkinfo = { 0 };
		MMCKINFO datachunkinfo = { 0 };
		MMCKINFO mmckinfo = { 0 };
		UINT32 buflen;
		LONG readlen;


		hmmio = mmioOpen((LPSTR)FileName, &mmioinfo, MMIO_READ);
		assert(hmmio);

		riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmioDescend(hmmio, &riffchunkinfo, NULL, MMIO_FINDRIFF);

		mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend(hmmio, &mmckinfo, &riffchunkinfo, MMIO_FINDCHUNK);

		if (mmckinfo.cksize >= sizeof(WAVEFORMATEX))
		{
			mmioRead(hmmio, (HPSTR)&wfx, sizeof(wfx));
		}
		else
		{
			PCMWAVEFORMAT pcmwf = { 0 };
			mmioRead(hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));
			memset(&wfx, 0x00, sizeof(wfx));
			memcpy(&wfx, &pcmwf, sizeof(pcmwf));
			wfx.cbSize = 0;
		}
		mmioAscend(hmmio, &mmckinfo, 0);

		datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmioDescend(hmmio, &datachunkinfo, &riffchunkinfo, MMIO_FINDCHUNK);



		buflen = datachunkinfo.cksize;
		tmp->m_SoundData = new unsigned char[buflen];
		readlen = mmioRead(hmmio, (HPSTR)tmp->m_SoundData, buflen);


		tmp->m_Length = readlen;
		tmp->m_PlayLength = readlen / wfx.nBlockAlign;


		mmioClose(hmmio, 0);
	}


	// �T�E���h�\�[�X����
	m_Xaudio->CreateSourceVoice(&tmp->m_SourceVoice, &wfx);
	assert(tmp->m_SourceVoice);

	// �R���e�i�ɒǉ�
	if (!CompareAudioClip(tmp)) {
		tmp->m_ClipName = tag;
		m_AudioClips.push_back(tmp);
	}
	else
	{
		tmp->m_SourceVoice->DestroyVoice();
		delete tmp->m_SoundData;
		delete tmp;
	}
}

void Audio::Load(AudioClip* clip)
{
	//
	if (!CompareAudioClip(clip)) {
		m_AudioClips.push_back(clip);
	}
}


void Audio::Uninit()
{
	if (!m_AudioClips.empty()) {
		// �T�E���h�\�[�X���~���ĉ��
		for (auto& it : m_AudioClips)
		{
			it->m_SourceVoice->Stop();
			it->m_SourceVoice->DestroyVoice();
			delete it->m_SoundData;
			delete it;
		}
		m_AudioClips.clear();
	}
}

/// <summary>
/// �ۑ�����Ă���T�E���h���Đ�����
/// </summary>
/// <param name="tag">�o�^��</param>
/// <param name="Loop">���[�v���邩</param>
void Audio::Play(const std::string& tag, bool Loop)
{
	// �l������
	auto it = std::find_if(m_AudioClips.begin(), m_AudioClips.end(), [&tag](AudioClip* clip) {
		return clip->m_ClipName == tag;
		});

	if (it == m_AudioClips.end()) return;

	(*it)->m_SourceVoice->Stop();
	(*it)->m_SourceVoice->FlushSourceBuffers();


	// �o�b�t�@�ݒ�
	XAUDIO2_BUFFER bufinfo;

	memset(&bufinfo, 0x00, sizeof(bufinfo));
	bufinfo.AudioBytes = (*it)->m_Length;
	bufinfo.pAudioData = (*it)->m_SoundData;
	bufinfo.PlayBegin = 0;
	bufinfo.PlayLength = (*it)->m_PlayLength;

	// ���[�v�ݒ�
	if (Loop)
	{
		bufinfo.LoopBegin = 0;
		bufinfo.LoopLength = (*it)->m_PlayLength;
		bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	(*it)->m_SourceVoice->SubmitSourceBuffer(&bufinfo, NULL);

	/*
		float outputMatrix[4] = { 0.0f , 0.0f, 1.0f , 0.0f };
		m_SourceVoice->SetOutputMatrix(m_MasteringVoice, 2, 2, outputMatrix);
		//m_SourceVoice->SetVolume(0.1f);
	*/


	// �Đ�
	(*it)->m_SourceVoice->Start();

}


void Audio::Play(AudioClip* clip, bool loop)
{
	// �l������
	auto it = std::find_if(m_AudioClips.begin(), m_AudioClips.end(), [&clip](const auto& it) {
		return it == clip;
		});

	if (it != m_AudioClips.end()) return;

	(*it)->m_SourceVoice->Stop();
	(*it)->m_SourceVoice->FlushSourceBuffers();


	// �o�b�t�@�ݒ�
	XAUDIO2_BUFFER bufinfo;

	memset(&bufinfo, 0x00, sizeof(bufinfo));
	bufinfo.AudioBytes = (*it)->m_Length;
	bufinfo.pAudioData = (*it)->m_SoundData;
	bufinfo.PlayBegin = 0;
	bufinfo.PlayLength = (*it)->m_PlayLength;

	// ���[�v�ݒ�
	if (loop)
	{
		bufinfo.LoopBegin = 0;
		bufinfo.LoopLength = (*it)->m_PlayLength;
		bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	(*it)->m_SourceVoice->SubmitSourceBuffer(&bufinfo, NULL);

	/*
		float outputMatrix[4] = { 0.0f , 0.0f, 1.0f , 0.0f };
		m_SourceVoice->SetOutputMatrix(m_MasteringVoice, 2, 2, outputMatrix);
		//m_SourceVoice->SetVolume(0.1f);
	*/


	// �Đ�
	(*it)->m_SourceVoice->Start();

}



/// <summary>
/// ���݂̍Đ���Ԃ̎擾
/// </summary>
/// <returns>true:�Đ��� false:��~��</returns>
bool Audio::IsSoundPlaying(const std::string& tag)
{
	XAUDIO2_VOICE_STATE xa2state;

	// �l������
	auto it = std::find_if(m_AudioClips.begin(), m_AudioClips.end(), [&tag](AudioClip* clip) {
		return clip->m_ClipName == tag;
		});
	
	// ��Ԏ擾
	(*it)->m_SourceVoice->GetState(&xa2state);

	return (xa2state.BuffersQueued != 0);
}

bool Audio::IsSoundPlaying(AudioClip* clip)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ���݂��邩�m�F
	if (!CompareAudioClip(clip)) false;

	// �l������
	auto it = std::find_if(m_AudioClips.begin(), m_AudioClips.end(), [&clip](AudioClip* a) {
		return a == clip;
		});

	// ��Ԏ擾
	(*it)->m_SourceVoice->GetState(&xa2state);

	return (xa2state.BuffersQueued != 0);
}

bool Audio::CompareAudioClip(AudioClip* clip)
{
	// m_AudioClips������|�C���^����v���Ă�����̂�T��
	for (const auto& it : m_AudioClips) {
		if (it == clip) return true;
	}
	return false;
}

/// <summary>
/// ���ݍĐ����̉����~�߂�
/// </summary>
void Audio::Stop(const std::string& tag)
{
	XAUDIO2_VOICE_STATE xa2state;

	// �l������
	auto it = std::find_if(m_AudioClips.begin(), m_AudioClips.end(), [&tag](AudioClip* clip) {
		return clip->m_ClipName == tag;
		});

	// ��Ԏ擾
	(*it)->m_SourceVoice->GetState(&xa2state);

	if (xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		(*it)->m_SourceVoice->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		(*it)->m_SourceVoice->FlushSourceBuffers();
	}
}

void Audio::Stop(AudioClip* clip)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	clip->m_SourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���

		// �ꎞ��~
		clip->m_SourceVoice->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		clip->m_SourceVoice->FlushSourceBuffers();
	}
}
