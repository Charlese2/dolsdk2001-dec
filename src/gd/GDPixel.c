#include <dolphin/gd/GDPixel.h>
#include <dolphin/os.h>
#include <macros.h>
#include "__gd.h"

void GDSetFog(GXFogType type, float startz, float endz,
		          float nearz, float farz, GXColor * color)
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

  GDWriteBPCmd(a_hex >> 12 | 0xee000000);
  GDWriteBPCmd(b_m | 0xef000000);
  GDWriteBPCmd(b_expn | 0xf0000000);
  GDWriteBPCmd(c_hex >> 12 | type << 21 | 0xf1000000);
  GDWriteBPCmd(color->b | color->g << 8 | color->r << 16 | 0xf2000000);
}

void GDSetBlendMode(GXBlendMode type, GXBlendFactor src_factor,
                    GXBlendFactor dst_factor, GXLogicOp logic_op)
{
	GDWriteBPCmd(0xfe001fe3);
	// clang-format off
	GDWriteBPCmd(
		((type == GX_BM_BLEND) || (type == GX_BM_SUBTRACT))
		| (type == GX_BM_LOGIC) << 1
		| dst_factor << 5
		| src_factor << 8
		| (type == GX_BM_SUBTRACT) << 11
		| logic_op << 12
		| 0x41000000
	);
	// clang-format on
}

void GDSetBlendModeEtc(GXBlendMode type, GXBlendFactor src_factor,
                       GXBlendFactor dst_factor, GXLogicOp logic_op,
                       u8 color_update_enable, u8 alpha_update_enable,
                       u8 dither_enable)
{
	// clang-format off
	GDWriteBPCmd(
		((type == GX_BM_BLEND) || (type == GX_BM_SUBTRACT))
		| (type == GX_BM_LOGIC) << 1
		| dither_enable << 2
		| color_update_enable << 3
		| alpha_update_enable << 4
		| dst_factor << 5
		| src_factor << 8
		| (type == GX_BM_SUBTRACT) << 11
		| logic_op << 12
		| 0x41000000
	);
	// clang-format on
}

void GDSetZMode(u8 compare_enable, GXCompare func, u8 update_enable)
{
	// clang-format off
	GDWriteBPCmd(
		compare_enable
		| func << 1
		| update_enable << 4
		| 1u << 30
	);
	// clang-format on
}

void GDSetDstAlpha(u8 enable, u8 alpha)
{
	// clang-format off
	GDWriteBPCmd(
		alpha
		| enable << 8
		| 0x42000000
	);
	// clang-format on
}

void GDSetDrawSync(u16 token)
{
	GDWriteBPCmd(token | 0x48000000);
	GDWriteBPCmd(token | 0x47000000);
}
