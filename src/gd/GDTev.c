#include <dolphin/gd/GDTev.h>
#include <dolphin/os.h>
#include "__gd.h"

void GDSetTevOp(GXTevStageID stage, GXTevMode mode)
{
	GXTevColorArg carg = GX_CC_RASC;
	GXTevAlphaArg aarg = GX_CA_RASA;

	if (stage != 0) {
		carg = GX_CC_CPREV;
		aarg = GX_CA_APREV;
	}

	switch (mode) {
	case GX_MODULATE:
		GDSetTevColorCalc(stage, 0xf, 8, carg, 0xf, 0, 0, 0, 1, 0);
		GDSetTevAlphaCalcAndSwap(stage, 7, 4, aarg, 7, 0, 0, 0, 1, 0, 0, 0);
		break;
	case GX_DECAL:
		GDSetTevColorCalc(stage, carg, 8, 9, 0xf, 0, 0, 0, 1, 0);
		GDSetTevAlphaCalcAndSwap(stage, 7, 7, 7, aarg, 0, 0, 0, 1, 0, 0, 0);
		break;
	case GX_BLEND:
		GDSetTevColorCalc(stage, carg, 0xc, 8, 0xf, 0, 0, 0, 1, 0);
		GDSetTevAlphaCalcAndSwap(stage, 7, 4, aarg, 7, 0, 0, 0, 1, 0, 0, 0);
		break;
	case GX_REPLACE:
		GDSetTevColorCalc(stage, 0xf, 0xf, 0xf, 8, 0, 0, 0, 1, 0);
		GDSetTevAlphaCalcAndSwap(stage, 7, 7, 7, 4, 0, 0, 0, 1, 0, 0, 0);
		break;
	case GX_PASSCLR:
		GDSetTevColorCalc(stage, 0xf, 0xf, 0xf, carg, 0, 0, 0, 1, 0);
		GDSetTevAlphaCalcAndSwap(stage, 7, 7, 7, aarg, 0, 0, 0, 1, 0, 0, 0);
		break;
#if DEBUG
	default:
		OSPanic(__FILE__, 110, "GDSetTevOp: Invalid Tev Mode");
		break;
#endif
	}
}

void GDSetTevColorCalc(GXTevStageID stage, GXTevColorArg a, GXTevColorArg b,
                       GXTevColorArg c, GXTevColorArg d, GXTevOp op,
                       GXTevBias bias, GXTevScale scale, u8 clamp,
                       GXTevRegID out_reg)
{
	if (op <= 1) {
		// clang-format off
		GDWriteBPCmd(
			d |
			c << 4 |
			b << 8 |
			a << 12 |
			bias << 16 |
			(op & 1) << 18 |
			clamp << 19 |
			scale << 20 |
			out_reg << 22 |
			(((stage & 0x7fffffff) * 2 + 0xc0 & 0xffffffff) << 24)
		);
		// clang-format on
	} else {
		// clang-format off
		GDWriteBPCmd(
			d |
			c << 4 |
			b << 8 |
			a << 12 |
			0x30000 |
			(op & 1) << 18 |
			clamp << 19 |
			(op & 6) << 19 |
			out_reg << 22 |
			(((stage & 0x7fffffff) * 2 + 0xc0 & 0xffffffff) << 24)
		);
		// clang-format on
	}
}

void GDSetTevAlphaCalcAndSwap(GXTevStageID stage, GXTevAlphaArg a,
                              GXTevAlphaArg b, GXTevAlphaArg c, GXTevAlphaArg d,
                              GXTevOp op, GXTevBias bias, GXTevScale scale,
                              u8 clamp, GXTevRegID out_reg,
                              GXTevSwapSel ras_sel, GXTevSwapSel tex_sel)
{
	if (op <= 1) {
		// clang-format off
		GDWriteBPCmd(
			ras_sel |
			tex_sel << 2 |
			d << 4 |
			c << 7 |
			b << 10 |
			a << 13 |
			bias << 16 |
			(op & 1) << 18 |
			clamp << 19 |
			scale << 20 |
			out_reg << 22 |
			(((stage & 0x7fffffff) * 2 + 0xc1 & 0xffffffff) << 24)
		);
		// clang-format on
	} else {
		// clang-format off
		GDWriteBPCmd(
			ras_sel |
			tex_sel << 2 |
			d << 4 |
			c << 7 |
			b << 10 |
			a << 13 |
			0x30000 |
			(op & 1) << 18 |
			clamp << 19 |
			(op & 6) << 19 |
			out_reg << 22 |
			(((stage & 0x7fffffff) * 2 + 0xc1 & 0xffffffff) << 24)
		);
		// clang-format on
	}
}

void GDSetTevColor(GXTevRegID reg, GXColor* color)
{
	u32 regRA;
	u32 regBG;

	regRA = color->r | color->a << 12 |
		((reg & 0x7fffffff) * 2 + 0xe0 & 0xffffffff) << 24;
	regBG = color->b | color->g << 12 |
		((reg & 0x7fffffff) * 2 + 0xe1 & 0xffffffff) << 24;
	GDWriteBPCmd(regRA);
	GDWriteBPCmd(regBG);
	GDWriteBPCmd(regBG);
	GDWriteBPCmd(regBG);
}

void GDSetTevColorS10(GXTevRegID reg, GXColorS10* color)
{
	u32 regRA;
	u32 regBG;

	regRA = (color->r & 0x7ff) | (color->a & 0x7ff) << 12 |
		((reg & 0x7fffffff) * 2 + 0xe0 & 0xffffffff) << 24;
	regBG = (color->b & 0x7ff) | (color->g & 0x7ff) << 12 |
		((reg & 0x7fffffff) * 2 + 0xe1 & 0xffffffff) << 24;
	GDWriteBPCmd(regRA);
	GDWriteBPCmd(regBG);
	GDWriteBPCmd(regBG);
	GDWriteBPCmd(regBG);
}

void GDSetTevKColor(GXTevKColorID reg, GXColor* color)
{
	u32 regRA; // r30
	u32 regBG; // r29

	regRA = color->r | color->a << 12 | 1 << 23 |
		((reg & 0x7fffffff) * 2 + 0xe0 & 0xffffffff) << 24;
	regBG = color->b | color->g << 12 | 1 << 23 |
		((reg & 0x7fffffff) * 2 + 0xe1 & 0xffffffff) << 24;
	GDWriteBPCmd(regRA);
	GDWriteBPCmd(regBG);
}

void GDSetTevKonstantSel(GXTevStageID evenStage, GXTevKColorSel kcsel0,
                         GXTevKAlphaSel kasel0, GXTevKColorSel kcsel1,
                         GXTevKAlphaSel kasel1)
{
  GDWriteBPCmd(0xfefffff0);
	// clang-format off
  GDWriteBPCmd(
		kcsel0 << 4 |
		kasel0 << 9 |
		kcsel1 << 14 |
		kasel1 << 19 |
		(evenStage / 2 + 0xf6) << 24
	);
	// clang-format on
}

void GDSetTevSwapModeTable(GXTevSwapSel table, GXTevColorChan red,
                           GXTevColorChan green, GXTevColorChan blue,
                           GXTevColorChan alpha)
{
  GDWriteBPCmd(0xfe00000f);
	// clang-format off
  GDWriteBPCmd(
		red |
		green << 2 |
		((table & 0x7fffffff) * 2 + 0xf6 & 0xffffffff) << 24
	);
	// clang-format on
  GDWriteBPCmd(0xfe00000f);
	// clang-format off
  GDWriteBPCmd(
		blue |
		alpha << 2 |
		((table & 0x7fffffff) * 2 + 0xf7 & 0xffffffff) << 24
	);
	// clang-format on
}

void GDSetAlphaCompare(GXCompare comp0, u8 ref0, GXAlphaOp op, GXCompare comp1,
                       u8 ref1)
{
	GDWriteBPCmd(ref0 | ref1 << 0x8 | comp0 << 0x10 | comp1 << 0x13 | op << 0x16
	             | 0xf3000000);
}

void GDSetZTexture(GXZTexOp op, GXTexFmt fmt, u32 bias)
{
	u32 zfmt;

	switch (fmt)
	{
	case GX_TF_Z8:
		zfmt = 0;
		break;
	case GX_TF_Z16:
		zfmt = 1;
		break;
	case GX_TF_Z24X8:
		zfmt = 2;
		break;
	default:
#if DEBUG
		OSPanic(__FILE__, 399, "GDSetZTexture: Invalid format");
#endif
		zfmt = 2;
		break;
	}

  GDWriteBPCmd(bias | 0xf4000000);
  GDWriteBPCmd(zfmt | op << 2 | 0xf5000000);
}

void GDSetTevOrder(GXTevStageID evenStage, GXTexCoordID coord0, GXTexMapID map0,
                   GXChannelID color0, GXTexCoordID coord1, GXTexMapID map1,
                   GXChannelID color1)
{
	static u8 c2r[] = { 0, 1, 0, 1, 0, 1, 7, 5, 6, 0, 0, 0, 0, 0, 0, 7 };

	// clang-format off
	GDWriteBPCmd(
		map0 & 0x7 |
	  (coord0 & 0x7) << 3 |
	  ((map0 != 0xff) && !(map0 & 0x100)) << 6 |
	  c2r[color0 & 0xf] << 7 |

	  (map1 & 0x7) << 12 |
	  (coord1 & 7) << 15 |
	  ((map1 != 0xff) && !(map1 & 0x100)) << 18 |
	  c2r[color1 & 0xf] << 19 |

	  (evenStage / 2 + 0x28) << 24
	);
	// clang-format on
}
