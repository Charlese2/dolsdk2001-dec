#include <dolphin/gd/GDPixel.h>
#include <dolphin/gd/GDBase.h>
#include <dolphin/os.h>
#include <macros.h>

void GDSetFog(GXFogType type, float startz, float endz,
		          float nearz, float farz, GXColor color)
{
  float A;
  float B;
	float B_mant;
  float C;
	float A_f;
  u32 b_expn;
	u32 b_m;
	u32 a_hex;
	u32 c_hex;

	ASSERTMSGLINE(55, farz >= 0.0f,
		"GDSetFog: The farz should be positive value");
	ASSERTMSGLINE(56, farz >= nearz,
		"GDSetFog: The farz should be larger than nearz");

	if ((farz == nearz) || (endz == startz)) {
		A = 0.0f;
		B = 0.5f;
		C = 0.0f;
	} else {
		A = (farz * nearz) / ((farz - nearz) * (endz - startz));
		B = (farz / (farz - nearz));
		C = (startz / (endz - startz));
	}

	B_mant = B;
	b_expn = 1;
	while (B_mant > 1.0) {
		B_mant *= 0.5f;
		b_expn++;
	}
	while (B_mant > 0.0f && B_mant < 0.5) {
		B_mant *= 2.0f;
		b_expn--;
	}

	A_f = A / (1 << b_expn);
	b_m = (u32) (8388638.0f * B_mant);

	a_hex = *(u32*)&A_f;
	c_hex = *(u32*)&C;

  GDWriteBPCmd(BP_FOG_UNK0(a_hex >> 12, 0xee));
  GDWriteBPCmd(BP_FOG_UNK1(b_m, 0xef));
  GDWriteBPCmd(BP_FOG_UNK2(b_expn, 0xf0));
  GDWriteBPCmd(BP_FOG_UNK3(c_hex >> 12, 0, type, 0xf1));
  GDWriteBPCmd(BP_FOG_COLOR(color.r, color.g, color.b, 0xf2));
}

void GDSetBlendMode(GXBlendMode type, GXBlendFactor src_factor,
                    GXBlendFactor dst_factor, GXLogicOp logic_op)
{
	GDWriteBPCmd(0xfe001fe3);
	// clang-format off
	GDWriteBPCmd(BP_BLEND_MODE(
		type == GX_BM_BLEND || type == GX_BM_SUBTRACT,
		type == GX_BM_LOGIC,
		0,
		0,
		0,
		dst_factor,
		src_factor,
		type == GX_BM_SUBTRACT,
		logic_op,
		0x41
	));
	// clang-format on
}

void GDSetBlendModeEtc(GXBlendMode type, GXBlendFactor src_factor,
                       GXBlendFactor dst_factor, GXLogicOp logic_op,
                       u8 color_update_enable, u8 alpha_update_enable,
                       u8 dither_enable)
{
	// clang-format off
	GDWriteBPCmd(BP_BLEND_MODE(
		type == GX_BM_BLEND || type == GX_BM_SUBTRACT,
		type == GX_BM_LOGIC,
		dither_enable,
		color_update_enable,
		alpha_update_enable,
		dst_factor,
		src_factor,
		type == GX_BM_SUBTRACT,
		logic_op,
		0x41
	));
	// clang-format on
}

void GDSetZMode(u8 compare_enable, GXCompare func, u8 update_enable)
{
	// clang-format off
	GDWriteBPCmd(BP_Z_MODE(compare_enable, func, update_enable, 0x40));
	// clang-format on
}

void GDSetDstAlpha(u8 enable, u8 alpha)
{
	// clang-format off
	GDWriteBPCmd(BP_DST_ALPHA(alpha, enable, 0x42));
	// clang-format on
}

void GDSetDrawSync(u16 token)
{
	GDWriteBPCmd(BP_TOKEN(token, 0x48));
	GDWriteBPCmd(BP_TOKEN(token, 0x47));
}
