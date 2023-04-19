#include "voicechat.h"

VoiceChat::VoiceChat()
{
    audio_in = std::unique_ptr<QAudioInput>();
    audio_out = std::unique_ptr<QAudioOutput>();
}
