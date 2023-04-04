#include "audio_player.h"

audio_player::audio_player() :
    m_media_player(new QMediaPlayer()),
    m_audio_output(new QAudioOutput())
{}

void audio_player::play(const QString &filepath)
{
    m_media_player->setAudioOutput(m_audio_output.get());
    m_media_player->setSource(filepath);
    m_media_player->play();
}
