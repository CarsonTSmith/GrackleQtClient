#ifndef VOICECHAT_H
#define VOICECHAT_H

#include <memory>
#include <QAudioInput>
#include <QAudioOutput>

class VoiceChat
{
public:
    VoiceChat();
    ~VoiceChat();

private:
    std::unique_ptr<QAudioInput>  audio_in;
    std::unique_ptr<QAudioOutput> audio_out;
};

#endif // VOICECHAT_H
