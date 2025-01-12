#ifndef _DOLPHIN_GDTRANSFORM_H_
#define _DOLPHIN_GDTRANSFORM_H_

#include <dolphin/types.h>
#include <dolphin/gx/GXEnum.h>
#include <dolphin/mtx.h>

#ifdef __cplusplus
extern "C" {
#endif

void GDLoadPosMtxImm(MtxPtr mtx, u32 id);
void GDLoadPosMtxIndx(u16 mtx_indx, u32 id);
void GDLoadNrmMtxImm(MtxPtr mtx, u32 id);
void GDLoadNrmMtxImm3x3(ROMtxPtr mtx, u32 id);
void GDLoadNrmMtxIndx3x3(u16 mtx_indx, u32 id);
void GDLoadTexMtxImm(MtxPtr mtx, u32 id, GXTexMtxType type);
void GDLoadTexMtxIndx(u16 mtx_indx, u32 id, GXTexMtxType type);

void GDSetCurrentMtx(u32 pn, u32 t0, u32 t1, u32 t2, u32 t3, u32 t4, u32 t5,
                     u32 t6, u32 t7);

#ifdef __cplusplus
}
#endif

#endif
