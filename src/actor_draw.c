#include "global.h"
#include "actor.h"
#include "ram_map.h"

/*
 * DrawActors — decompiled from baserom @ 0x08006C0C (span 0x1E4).
 *
 * Linked into append ROM (not peeled): LynJump calls DrawActors__Continue into
 * the baserom copy so we cannot drift from vanilla draw behaviour. This C is
 * the readable reference and exports GetActorScreenPos for HP-bar placement.
 */

typedef void (*VoidFunc)(void);
typedef void (*ActorFunc)(u8 *actor);

#define DrawActor ((ActorFunc)0x08006439)
#define UpdateSpriteAnims ((VoidFunc)0x0800A3A5)

static void DrawActorLayer(u32 layer)
{
    u32 i;

    for (i = 0; i < ACTOR_COUNT; i++)
    {
        u8 *actor = &gActorPool[i * ACTOR_STRIDE];

        if ((*(u16 *)(actor + ACTOR_OFF_FLAGS) & ACTOR_FLAG_ACTIVE) == 0)
            continue;
        if (*(u16 *)(actor + ACTOR_OFF_ANIM) == 0)
            continue;
        if ((actor[ACTOR_OFF_KIND] & ACTOR_KIND_LAYER_MASK) != layer)
            continue;
        DrawActor(actor);
    }
}

void GetActorScreenPos(const u8 *actor, s32 *outX, s32 *outY)
{
    u8 cam = actor[ACTOR_OFF_CAMERA];
    s32 wx = *(s32 *)(actor + ACTOR_OFF_X);
    s32 wy = *(s32 *)(actor + ACTOR_OFF_Y);
    s32 sx;
    s32 sy;

    if (cam > 3)
    {
        sx = wx >> 16;
        sy = wy >> 16;
    }
    else
    {
        u8 *camera = gCameras + (u32)cam * CAMERA_STRIDE;
        sx = ((wx & 0xFFFF0000) - (*(s32 *)(camera + CAMERA_OFF_X) & 0xFFFF0000)) >> 16;
        sy = ((wy & 0xFFFF0000) - (*(s32 *)(camera + CAMERA_OFF_Y) & 0xFFFF0000)) >> 16;
    }

    sx += (s16)(*(u16 *)(actor + ACTOR_OFF_DRAW_OX));
    sy += (s16)(*(u16 *)(actor + ACTOR_OFF_DRAW_OY));
    *outX = sx;
    *outY = sy;
}

void DrawActors(void)
{
    u32 i;
    u32 layer;
    u8 *head;
    u8 *cur;
    u8 *prev;
    u8 *actor;

    gAffineSlotCounter = 0;
    gOamCursor = gActorOamStart;
    UpdateSpriteAnims();

    if (gUseYSorting)
    {
        head = NULL;
        for (i = 0; i < ACTOR_COUNT; i++)
        {
            actor = &gActorPool[i * ACTOR_STRIDE];
            if ((*(u16 *)(actor + ACTOR_OFF_FLAGS) & ACTOR_FLAG_ACTIVE) == 0)
                continue;
            if (*(u16 *)(actor + ACTOR_OFF_ANIM) == 0)
                continue;
            if (actor[ACTOR_OFF_KIND] & ACTOR_KIND_YSORT_EXCL)
                continue;

            *(u8 **)(actor + ACTOR_OFF_DRAW_NEXT) = NULL;
            if (head == NULL)
            {
                head = actor;
                continue;
            }

            prev = head;
            cur = head;
            while (cur != NULL)
            {
                if (*(s32 *)(actor + ACTOR_OFF_Y) > *(s32 *)(cur + ACTOR_OFF_Y))
                {
                    if (cur == head)
                        head = actor;
                    else
                        *(u8 **)(prev + ACTOR_OFF_DRAW_NEXT) = actor;
                    *(u8 **)(actor + ACTOR_OFF_DRAW_NEXT) = cur;
                    break;
                }
                prev = cur;
                cur = *(u8 **)(cur + ACTOR_OFF_DRAW_NEXT);
            }
            if (cur == NULL)
            {
                *(u8 **)(prev + ACTOR_OFF_DRAW_NEXT) = actor;
                *(u8 **)(actor + ACTOR_OFF_DRAW_NEXT) = NULL;
            }
        }

        for (cur = head; cur != NULL; cur = *(u8 **)(cur + ACTOR_OFF_DRAW_NEXT))
            DrawActor(cur);

        for (layer = 0x40; layer <= 0x60; layer += 0x10)
            DrawActorLayer(layer);
    }
    else
    {
        for (layer = 0x00; layer <= 0x60; layer += 0x10)
            DrawActorLayer(layer);
    }

    while (gOamCursor < gSoftOamCount)
    {
        u16 *entry = &gSoftOam[gOamCursor * 4];
        entry[0] = (entry[0] & ~0x0300) | 0x0200;
        gOamCursor++;
    }
}
