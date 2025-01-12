#include <dolphin/gd/GDTexture.h>
#include <dolphin/os.h>
#include <macros.h>
#include "__gd.h"

__declspec(section ".data") u8 GD2HWFiltConv[6] = {0, 4, 1, 5, 2, 6};

u8 GDTexMode0Ids[8] = {128, 129, 130, 131, 160, 161, 162, 163};
u8 GDTexMode1Ids[8] = {132, 133, 134, 135, 164, 165, 166, 167};
u8 GDTexImage0Ids[8] = {136, 137, 138, 139, 168, 169, 170, 171};
u8 GDTexImage1Ids[8] = {140, 141, 142, 143, 172, 173, 174, 175};
u8 GDTexImage2Ids[8] = {144, 145, 146, 147, 176, 177, 178, 179};
u8 GDTexImage3Ids[8] = {148, 149, 150, 151, 180, 181, 182, 183};
u8 GDTexTlutIds[8] = {152, 153, 154, 155, 184, 185, 186, 187};

void GDSetTexLookupMode(GXTexMapID id, GXTexWrapMode wrap_s,
                        GXTexWrapMode wrap_t, GXTexFilter min_filt,
                        GXTexFilter mag_filt, float min_lod, float max_lod,
                        float lod_bias, u8 bias_clamp, u8 do_edge_lod,
                        GXAnisotropy max_aniso)
{
  GDWriteBPCmd(
    wrap_s |
    wrap_t << 2 |
    (mag_filt == 1) << 4 |
    GD2HWFiltConv[min_filt] << 5 |
    !do_edge_lod << 8 |
    ((u8)(32.0f * lod_bias)) << 9 |
    max_aniso << 19 |
    bias_clamp << 21 |
    GDTexMode0Ids[id] << 24
  );

  GDWriteBPCmd(
    (u8)(16.0f * min_lod) |
    ((u8)(16.0f * max_lod)) << 8 |
    GDTexMode1Ids[id] << 24
  );
}

void GDSetTexImgAttr(GXTexMapID id, u16 width, u16 height, GXTexFmt format)
{
  // clang-format off
  GDWriteBPCmd(
    width - 1 |
    (height - 1) << 10 |
    format << 20 |
    GDTexImage0Ids[id] << 24
  );
  // clang-format on
}

void GDSetTexImgPtr(GXTexMapID id, void * image_ptr)
{
  GDWriteBPCmd(GDTexImage3Ids[id] << 0x18 | OSCachedToPhysical(image_ptr) >> 5);
}

void GDSetTexImgPtrRaw(GXTexMapID id, u32 image_ptr_raw)
{
  GDWriteBPCmd(image_ptr_raw | GDTexImage3Ids[id] << 24);
}

void GDPatchTexImgPtr(void * image_ptr)
{
  GDWrite_u24(OSCachedToPhysical(image_ptr) >> 5);
}

void GDSetTexCached(GXTexMapID id, u32 tmem_even, GXTexCacheSize size_even,
                    u32 tmem_odd, GXTexCacheSize size_odd)
{
  // clang-format off
  GDWriteBPCmd(
    tmem_even >> 5 |
    (size_even + 3) << 15 |
    (size_even + 3) << 18 |
    GDTexImage1Ids[id] << 24
  );
  // clang-format on
  if (size_odd != 3 && tmem_odd < 0x100000) {
    // clang-format off
    GDWriteBPCmd(
      tmem_odd >> 5 |
      (size_odd + 3) << 15 |
      (size_odd + 3) << 18 |
      GDTexImage2Ids[id] << 24
    );
    // clang-format on
  }
}


void GDSetTexPreLoaded(GXTexMapID id, u32 tmem_even, u32 tmem_odd)
{
  GDWriteBPCmd(tmem_even >> 5 | 0x200000 | GDTexImage1Ids[id] << 24);
  if (tmem_odd < 0x100000) {
    GDWriteBPCmd(tmem_odd >> 5 | GDTexImage2Ids[id] << 24);
  }
}

void GDSetTexTlut(GXTexMapID id, u32 tmem_addr, GXTlutFmt format)
{
  // clang-format off
  GDWriteBPCmd(
    tmem_addr - 0x80000U >> 9 |
    format << 10 |
    GDTexTlutIds[id] << 24
  );
  // clang-format on
}

void GDSetTexCoordScale(GXTexCoordID coord, u16 s_scale, u16 t_scale)
{
  GDWriteBPCmd(0xfe00ffff);
  // clang-format off
  GDWriteBPCmd(
    s_scale - 1 |
    (2 * coord + 0x30) << 24
  );
  // clang-format on
  GDWriteBPCmd(0xfe00ffff);
  // clang-format off
  GDWriteBPCmd(
    t_scale - 1 |
    (2 * coord + 0x31) << 24
  );
  // clang-format on
}

void GDSetTexCoordScale2(GXTexCoordID coord, u16 s_scale, u8 s_bias,
                         u8 s_wrap, u16 t_scale, u8 t_bias, u8 t_wrap)
{
  GDWriteBPCmd(0xfe03ffff);
  // clang-format off
  GDWriteBPCmd(
    s_scale - 1 |
    s_bias << 0x10 |
    s_wrap << 0x11 |
    (2 * coord + 0x30) << 24
  );
  GDWriteBPCmd(
    t_scale - 1 |
    t_bias << 0x10 |
    t_wrap << 0x11 |
    (2 * coord + 0x31) << 24
  );
  // clang-format on
}

void GDSetTexCoordScaleAndTOEs(GXTexCoordID coord, u16 s_scale, u8 s_bias,
                               u8 s_wrap, u16 t_scale, u8 t_bias, u8 t_wrap,
                               u8 line_offset, u8 point_offset)
{
  // clang-format off
  GDWriteBPCmd(
    s_scale - 1 |
    s_bias << 16 |
    s_wrap << 17 |
    line_offset << 18 |
    point_offset << 19 |
    (2 * coord + 0x30) << 24
  );
  GDWriteBPCmd(
    t_scale - 1 |
    t_bias << 16 |
    t_wrap << 17 |
    (2 * coord + 0x31) << 24
  );
  // clang-format on
}

void GDLoadTlut(void* tlut_ptr, u32 tmem_addr, GXTlutSize size)
{
  ASSERTMSGLINE(488, !(tmem_addr & 0x1ff),
    "GDLoadTlut: invalid TMEM pointer");
	ASSERTMSGLINE(489, size <= 0x400,
    "GDLoadTlut: invalid TLUT size");

  GDWriteBPCmd(0xfeffff00);
  GDWriteBPCmd(0xf000000);
  GDWriteBPCmd(OSCachedToPhysical(tlut_ptr) >> 5 | 0x64000000);
  GDWriteBPCmd(tmem_addr - 0x80000 >> 9 | size << 10 | 0x65000000);
  GDWriteBPCmd(0xfeffff00);
  GDWriteBPCmd(0xf000000);
}

void GDLoadTlutRaw(u32 tlut_ptr_raw, u32 tmem_addr, GXTlutSize size)
{
	ASSERTMSGLINE(527, size <= 0x400,
    "GDLoadTlut: invalid TLUT size");

  GDWriteBPCmd(0xfeffff00);
  GDWriteBPCmd(0xf000000);
  GDWriteBPCmd(tlut_ptr_raw | 0x64000000);
  GDWriteBPCmd(tmem_addr - 0x80000U >> 9 | size << 10 | 0x65000000);
  GDWriteBPCmd(0xfeffff00);
  GDWriteBPCmd(0xf000000);
}
