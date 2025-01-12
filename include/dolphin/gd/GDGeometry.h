#ifndef _DOLPHIN_GDGEOMETRY_H_
#define _DOLPHIN_GDGEOMETRY_H_

#include <dolphin/gx/GXStruct.h>

#ifdef __cplusplus
extern "C" {
#endif

void GDSetVtxDescv(const GXVtxDescList* attrPtr);
void GDSetVtxAttrFmtv(GXVtxFmt vtxfmt, const GXVtxAttrFmtList* list);
void GDSetArray(GXAttr attr, void* base_ptr, u8 stride);
void GDSetArrayRaw(GXAttr attr, u32 base_ptr_raw, u8 stride);
void GDPatchArrayPtr(void* base_ptr);
void GDSetTexCoordGen(GXTexCoordID dst_coord, GXTexGenType func,
                      GXTexGenSrc src_param, u8 normalize, u32 postmtx);
void GDSetCullMode(GXCullMode mode);
void GDSetGenMode(u8 nTexGens, u8 nChans, u8 nTevs);
void GDSetGenMode2(u8 nTexGens, u8 nChans, u8 nTevs, u8 nInds, GXCullMode cm);
void GDSetLPSize(u8 lineWidth, u8 pointSize, GXTexOffset lineOffset,
                 GXTexOffset pointOffset, u8 lineHalfAspect);
void GDSetCoPlanar(u8 enable);

#ifdef __cplusplus
}
#endif

#endif
