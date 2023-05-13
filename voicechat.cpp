#include "voicechat.h"

#include <QBuffer>
#include <QMediaContent>
#include <QMediaPlayer>

VoiceChat::VoiceChat() : audio_in(std::make_unique<QAudioInput>()),
                         audio_out(std::make_unique<QAudioOutput>()),
                         media_player(std::make_unique<QMediaPlayer>())
{

}



void VoiceChat::play_buffer(QByteArray buf)
{
    QBuffer media_stream(&buf);
    media_stream.open(QIODevice::ReadOnly);
    media_player->setMedia(QMediaContent(), &media_stream);
    media_player->play();
}
