#ifndef VOICECHAT_H
#define VOICECHAT_H

#include <memory>
#include <QAudioInput>
#include <QAudioOutput>
#include <QByteArray>
#include <QMediaPlayer>

class VoiceChat
{
public:
    VoiceChat();
    ~VoiceChat() = default;

    void play_buffer(QByteArray buf);

private:
    std::unique_ptr<QAudioInput>  audio_in;
    std::unique_ptr<QAudioOutput> audio_out;
    std::unique_ptr<QMediaPlayer> media_player;
};

#endif // VOICECHAT_H
