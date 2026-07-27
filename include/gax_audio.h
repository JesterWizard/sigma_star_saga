#ifndef GUARD_GAX_AUDIO_H
#define GUARD_GAX_AUDIO_H

#include "gba/types.h"

void GaxPlayMusic(u16 id);
void GaxStopMusic(void);
void GaxPlayVoice(u16 id, s16 pan);
void GaxStopVoice(void);

/* LynJump body — AgbMain gax boot @ 0x080038D8 */
void GaxBootInit__Replacement(void);
/* LynJump — TalkAdvance @ 0x080102BC when custom_gax_audio */
const u8 *TalkAdvance_Gax__Replacement(const u8 *stream);

#endif /* GUARD_GAX_AUDIO_H */
