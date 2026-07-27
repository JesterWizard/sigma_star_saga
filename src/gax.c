#include "global.h"
#include "gax.h"
#include "ram_map.h"

/*
 * GAX 3.05A wrappers — decompiled call shapes from early ROM stubs and
 * AgbMain boot @ 0x080038D8..0x080038F9 (gax2_new through gax2_init).
 * Custom speech / catalog logic lives in src_custom/gax_audio_hooks.c.
 */

typedef void (*Gax2NewFunc)(Gax2Params *params);
typedef void (*Gax2InitFunc)(Gax2Params *params);
typedef void (*GaxFxFunc)(u32 id, u32 pan);
typedef void (*GaxSongFunc)(Gax2Params *params);
typedef void (*GaxStopMusicFunc)(void);
typedef void (*GaxStopFxFunc)(u32 unused, s32 channel);
typedef void (*GaxMusicVolFunc)(s32 channel, u32 vol);
typedef void (*GaxFxVolFunc)(s32 channel, u32 vol);

#define Gax2NewRom ((Gax2NewFunc)0x08054AB5)
#define Gax2InitRom ((Gax2InitFunc)0x08054F95)
#define GaxFxRom ((GaxFxFunc)0x08055C8D)
#define GaxSongApplyRom ((GaxSongFunc)0x08054891)
#define GaxStopMusicRom ((GaxStopMusicFunc)0x08056509)
#define GaxStopFxRom ((GaxStopFxFunc)0x08056571)
#define GaxMusicVolRom ((GaxMusicVolFunc)0x080560CD)
#define GaxFxVolRom ((GaxFxVolFunc)0x080565D5)

void Gax2New(Gax2Params *params)
{
    Gax2NewRom(params);
}

void Gax2Init(Gax2Params *params)
{
    Gax2InitRom(params);
}

void PlaySfx(u32 id, u32 pan)
{
    GaxFxRom(id, pan);
}

void PlayBgm(const void *songModule)
{
    if (gGaxCurrentSong == songModule)
        return;

    gGaxCurrentSong = (void *)songModule;
    gGaxParams.music = songModule;
    GaxSongApplyRom(&gGaxParams);

    if (gGaxParams.wramSize <= 0x1000)
        Gax2InitRom(&gGaxParams);

    GaxMusicVolRom(-1, gGaxMusicVol);
}

void StopBgm(void)
{
    GaxStopMusicRom();
    gGaxCurrentSong = (void *)(s32)-1;
}

void StopAllFx(void)
{
    GaxStopFxRom(0, -1);
}

void SetMusicVol(u32 channelOrMinus1, u32 vol0to255)
{
    GaxMusicVolRom((s32)channelOrMinus1, vol0to255);
}

void SetMasterVol(u32 vol0toFFFF)
{
    gGaxMusicVol = (u16)vol0toFFFF;
    GaxMusicVolRom(-1, vol0toFFFF > 255 ? 255 : vol0toFFFF);
    GaxFxVolRom(-1, vol0toFFFF > 255 ? 255 : vol0toFFFF);
}

void GaxBootInit(void)
{
    Gax2New(&gGaxParams);
    gGaxParams.debugAssert = 0;
    gGaxParams.wramSize = GAX_MIX_BUFFER_BYTES;
    gGaxParams.wram = gGaxMixBuffer;
    gGaxParams.package = GAX_PACKAGE_ROM;
    gGaxParams.flags = (u16)(gGaxParams.flags & ~GAX_FLAG_SPEECH);
    Gax2Init(&gGaxParams);
}
