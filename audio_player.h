#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <memory>
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QString>

class audio_player
{
public:
    audio_player();
    ~audio_player() {}

    void play(const QString &filepath);

private:
    std::unique_ptr<QMediaPlayer> m_media_player;
    std::unique_ptr<QAudioOutput> m_audio_output;
};

#endif // AUDIO_PLAYER_H
