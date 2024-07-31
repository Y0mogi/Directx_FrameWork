#include "main.h"
#include <string>
#include <vector>
#include <cassert>
#include <mmreg.h>
#include <chrono>
#include "audio.h"

IXAudio2* Audio::m_Xaudio = NULL;
IXAudio2MasteringVoice* Audio::m_MasteringVoice = NULL;
std::vector<AudioClip*> Audio::m_AudioClips;

void Audio::InitMaster()
{
    // COM初期化
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    // XAudio生成
    XAudio2Create(&m_Xaudio, 0);

    // マスタリングボイス生成
    m_Xaudio->CreateMasteringVoice(&m_MasteringVoice);
}

void Audio::UninitMaster()
{
    m_MasteringVoice->DestroyVoice();
    m_Xaudio->Release();
    CoUninitialize();
}

AudioClip* Audio::CreateAudioClip(const char* path, bool load)
{
    AudioClip* tmp = new AudioClip;
    // サウンドデータ読込
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
        tmp->m_SampleRate = wfx.nSamplesPerSec;

        mmioClose(hmmio, 0);
    }

    // サウンドソース生成
    m_Xaudio->CreateSourceVoice(&tmp->m_SourceVoice, &wfx);
    assert(tmp->m_SourceVoice);

    // マップに追加
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
    // サウンドデータ読込
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
        tmp->m_SampleRate = wfx.nSamplesPerSec;

        mmioClose(hmmio, 0);
    }

    // サウンドソース生成
    m_Xaudio->CreateSourceVoice(&tmp->m_SourceVoice, &wfx);
    assert(tmp->m_SourceVoice);

    // コンテナに追加
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
    if (!CompareAudioClip(clip)) {
        m_AudioClips.push_back(clip);
    }
}

void Audio::Uninit()
{
    if (!m_AudioClips.empty()) {
        // サウンドソースを停止して解放
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

void Audio::Play(const std::string& tag, bool Loop)
{
    // 値を検索
    auto it = std::find_if(m_AudioClips.begin(), m_AudioClips.end(), [&tag](AudioClip* clip) {
        return clip->m_ClipName == tag;
        });

    if (it == m_AudioClips.end()) return;

    (*it)->m_SourceVoice->Stop();
    (*it)->m_SourceVoice->FlushSourceBuffers();

    // バッファ設定
    XAUDIO2_BUFFER bufinfo;

    memset(&bufinfo, 0x00, sizeof(bufinfo));
    bufinfo.AudioBytes = (*it)->m_Length;
    bufinfo.pAudioData = (*it)->m_SoundData;
    bufinfo.PlayBegin = 0;
    bufinfo.PlayLength = (*it)->m_PlayLength;

    // ループ設定
    if (Loop)
    {
        bufinfo.LoopBegin = 0;
        bufinfo.LoopLength = (*it)->m_PlayLength;
        bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
        (*it)->m_IsLooping = true;
    }
    else
    {
        (*it)->m_IsLooping = false;
    }

    (*it)->m_SourceVoice->SubmitSourceBuffer(&bufinfo, NULL);

    // 再生開始時間を記録
    (*it)->m_StartTime = std::chrono::steady_clock::now();

    // 再生
    (*it)->m_SourceVoice->Start();
}

void Audio::Play(AudioClip* clip, bool loop)
{
    // 値を検索
    auto it = std::find_if(m_AudioClips.begin(), m_AudioClips.end(), [&clip](const auto& it) {
        return it == clip;
        });

    if (it != m_AudioClips.end()) return;

    (*it)->m_SourceVoice->Stop();
    (*it)->m_SourceVoice->FlushSourceBuffers();

    // バッファ設定
    XAUDIO2_BUFFER bufinfo;

    memset(&bufinfo, 0x00, sizeof(bufinfo));
    bufinfo.AudioBytes = (*it)->m_Length;
    bufinfo.pAudioData = (*it)->m_SoundData;
    bufinfo.PlayBegin = 0;
    bufinfo.PlayLength = (*it)->m_PlayLength;

    // ループ設定
    if (loop)
    {
        bufinfo.LoopBegin = 0;
        bufinfo.LoopLength = (*it)->m_PlayLength;
        bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
        (*it)->m_IsLooping = true;
    }
    else
    {
        (*it)->m_IsLooping = false;
    }

    (*it)->m_SourceVoice->SubmitSourceBuffer(&bufinfo, NULL);

    // 再生開始時間を記録
    (*it)->m_StartTime = std::chrono::steady_clock::now();

    // 再生
    (*it)->m_SourceVoice->Start();
}

bool Audio::IsSoundPlaying(const std::string& tag)
{
    XAUDIO2_VOICE_STATE xa2state;

    // 値を検索
    auto it = std::find_if(m_AudioClips.begin(), m_AudioClips.end(), [&tag](AudioClip* clip) {
        return clip->m_ClipName == tag;
        });

    // 状態取得
    (*it)->m_SourceVoice->GetState(&xa2state);

    return (xa2state.BuffersQueued != 0);
}

bool Audio::IsSoundPlaying(AudioClip* clip)
{
    XAUDIO2_VOICE_STATE xa2state;

    // 存在するか確認
    if (!CompareAudioClip(clip)) return false;

    // 値を検索
    auto it = std::find_if(m_AudioClips.begin(), m_AudioClips.end(), [&clip](AudioClip* a) {
        return a == clip;
        });

    // 状態取得
    (*it)->m_SourceVoice->GetState(&xa2state);

    return (xa2state.BuffersQueued != 0);
}

bool Audio::IsLooping(const std::string& tag)
{
    // 対象のAudioClipを検索
    auto it = std::find_if(m_AudioClips.begin(), m_AudioClips.end(), [&tag](AudioClip* clip) {
        return clip->m_ClipName == tag;
        });

    if (it == m_AudioClips.end()) return false;

    return (*it)->m_IsLooping;
}

bool Audio::IsLooping(AudioClip* clip)
{
    // 対象のAudioClipを検索
    auto it = std::find_if(m_AudioClips.begin(), m_AudioClips.end(), [&clip](AudioClip* a) {
        return a == clip;
        });

    if (it == m_AudioClips.end()) return false;

    return (*it)->m_IsLooping;
}

bool Audio::CompareAudioClip(AudioClip* clip)
{
    // m_AudioClips内からポインタが一致しているものを探す
    for (const auto& it : m_AudioClips) {
        if (it == clip) return true;
    }
    return false;
}

void Audio::Stop(const std::string& tag)
{
    XAUDIO2_VOICE_STATE xa2state;

    // 値を検索
    auto it = std::find_if(m_AudioClips.begin(), m_AudioClips.end(), [&tag](AudioClip* clip) {
        return clip->m_ClipName == tag;
        });

    // 状態取得
    (*it)->m_SourceVoice->GetState(&xa2state);

    if (xa2state.BuffersQueued != 0)
    {// 再生中
        // 一時停止
        (*it)->m_SourceVoice->Stop(0);

        // オーディオバッファの削除
        (*it)->m_SourceVoice->FlushSourceBuffers();

        // 再生開始時間をリセット
        (*it)->m_StartTime = std::chrono::steady_clock::time_point();
    }
}

void Audio::Stop(AudioClip* clip)
{
    XAUDIO2_VOICE_STATE xa2state;

    // 状態取得
    clip->m_SourceVoice->GetState(&xa2state);
    if (xa2state.BuffersQueued != 0)
    {// 再生中

        // 一時停止
        clip->m_SourceVoice->Stop(0);

        // オーディオバッファの削除
        clip->m_SourceVoice->FlushSourceBuffers();

        // 再生開始時間をリセット
        clip->m_StartTime = std::chrono::steady_clock::time_point();
    }
}

float Audio::GetCurrentPlayTime(const std::string& tag)
{
    // 対象のAudioClipを検索
    auto it = std::find_if(m_AudioClips.begin(), m_AudioClips.end(), [&tag](AudioClip* clip) {
        return clip->m_ClipName == tag;
        });

    if (it == m_AudioClips.end()) return 0.0f;

    // 再生状態を取得
    XAUDIO2_VOICE_STATE xa2state;
    (*it)->m_SourceVoice->GetState(&xa2state);

    if (xa2state.BuffersQueued == 0)
    {
        // 再生中でない場合、再生時間を0にする
        return 0.0f;
    }

    // 経過時間を取得
    auto now = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(now - (*it)->m_StartTime).count();

    return static_cast<float>(elapsedTime);
}

void Audio::CompInfo()
{
    using namespace ImGui;
    SeparatorText("AudioComponent");
    XAUDIO2_VOICE_STATE xa2state;

    if (TreeNode("Audioinfo")) {

        // 現在再生中のオーディオ
        SeparatorText("NowPlayingAudio");
        for (auto& it : m_AudioClips) {
            // 状態取得
            it->m_SourceVoice->GetState(&xa2state);


            // 再生時間を取得
            float playTime = GetCurrentPlayTime(it->m_ClipName);
            bool isLooping = it->m_IsLooping;

            Text("Name: %s", it->m_ClipName.c_str());
            Text("Length: %d bytes", it->m_Length);
            Text("PlayLength: %d samples", it->m_PlayLength);
            Text("Current Play Time: %.2f seconds", playTime);
            Text("Looping: %s", isLooping ? "Yes" : "No");
            
        }

        TreePop();
    }
}
