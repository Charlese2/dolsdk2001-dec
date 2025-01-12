#ifndef _DOLPHIN_GDLIGHT_H_
#define _DOLPHIN_GDLIGHT_H_

#include <dolphin/gx/GXEnum.h>
#include <dolphin/gx/GXStruct.h>

#define XF_AMBIENT0_ID  0x100a
#define XF_MATERIAL0_ID 0x100c

#ifdef __cplusplus
extern "C" {
#endif

void GDSetLightAttn(GXLightID light, float a0, float a1, float a2, float k0,
                    float k1, float k2);

void GDSetLightSpot(GXLightID light, float cutoff, GXSpotFn spot_func);

void GDSetLightDistAttn(GXLightID light, float ref_dist, float ref_br,
                        GXDistAttnFn dist_func);

void GDSetLightColor(GXLightID light, GXColor* color);

void GDSetLightPos(GXLightID light, float x, float y, float z);

void GDSetLightDir(GXLightID light, float nx, float ny, float nz);

void GDSetSpecularDirHA(GXLightID light, float nx, float ny, float nz,
                        float hx, float hy, float hz);

void GDSetSpecularDir(GXLightID light, float nx, float ny, float nz);

void GDLoadLightObjIndx(u32 lt_obj_indx, GXLightID light);

void GDSetChanAmbColor(GXChannelID chan, GXColor* color);

void GDSetChanMatColor(GXChannelID chan, GXColor* color);

void GDSetChanCtrl(GXChannelID chan, u8 enable, GXColorSrc amb_src,
                   GXColorSrc mat_src, u32 light_mask, GXDiffuseFn diff_fn,
                   GXAttnFn attn_fn);


inline static u16 __GDLightID2Index(GXLightID id)
{
	u16 idx;

	idx = 0x1F - __cntlzw(id);
	if (idx > 7) {
		idx = 0;
	}
	return idx;
}

inline static u16 __GDLightID2Offset(GXLightID id)
{
	return __GDLightID2Index(id) * 16;
}

#ifdef __cplusplus
}
#endif

#endif
