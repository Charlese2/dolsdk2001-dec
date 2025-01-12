#ifndef _DOLPHIN_GD_FILE_H
#define _DOLPHIN_GD_FILE_H

#include <dolphin/types.h>
#include <dolphin/gx/GXEnum.h>

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus


void GDSetTexLookupMode(GXTexMapID id, GXTexWrapMode wrap_s,
                        GXTexWrapMode wrap_t, GXTexFilter min_filt,
                        GXTexFilter mag_filt, float min_lod, float max_lod,
                        float lod_bias, u8 bias_clamp, u8 do_edge_lod,
                        GXAnisotropy max_aniso);

void GDSetTexImgAttr(GXTexMapID id, u16 width, u16 height, GXTexFmt format);

void GDSetTexImgPtr(GXTexMapID id, void * image_ptr);

void GDSetTexImgPtrRaw(GXTexMapID id, u32 image_ptr_raw);

void GDPatchTexImgPtr(void * image_ptr);

void GDSetTexCached(GXTexMapID id, u32 tmem_even, GXTexCacheSize size_even,
                    u32 tmem_odd, GXTexCacheSize size_odd);

void GDSetTexPreLoaded(GXTexMapID id, u32 tmem_even, u32 tmem_odd);

void GDSetTexTlut(GXTexMapID id, u32 tmem_addr, GXTlutFmt format);

void GDSetTexCoordScale(GXTexCoordID coord, u16 s_scale, u16 t_scale);

void GDSetTexCoordScale2(GXTexCoordID coord, u16 s_scale, u8 s_bias,
                         u8 s_wrap, u16 t_scale, u8 t_bias, u8 t_wrap);

void GDSetTexCoordScaleAndTOEs(GXTexCoordID coord, u16 s_scale, u8 s_bias,
                               u8 s_wrap, u16 t_scale, u8 t_bias, u8 t_wrap,
                               u8 line_offset, u8 point_offset);

void GDLoadTlut(void* tlut_ptr, u32 tmem_addr, GXTlutSize size);

void GDLoadTlutRaw(u32 tlut_ptr_raw, u32 tmem_addr, GXTlutSize size);


#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif
