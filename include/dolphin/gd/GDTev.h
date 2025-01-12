#ifndef _DOLPHIN_GD_TEV_H
#define _DOLPHIN_GD_TEV_H

#include <dolphin/gx/GXEnum.h>
#include <dolphin/gx/GXStruct.h>

#ifdef __cplusplus
extern "C" {
#endif

void GDSetTevOp(GXTevStageID stage, GXTevMode mode);

void GDSetTevColorCalc(GXTevStageID stage, GXTevColorArg a, GXTevColorArg b,
                       GXTevColorArg c, GXTevColorArg d, GXTevOp op,
                       GXTevBias bias, GXTevScale scale, u8 clamp,
                       GXTevRegID out_reg);

void GDSetTevAlphaCalcAndSwap(GXTevStageID stage, GXTevAlphaArg a,
                              GXTevAlphaArg b, GXTevAlphaArg c, GXTevAlphaArg d,
                              GXTevOp op, GXTevBias bias, GXTevScale scale,
                              u8 clamp, GXTevRegID out_reg,
                              GXTevSwapSel ras_sel, GXTevSwapSel tex_sel);

void GDSetTevColor(GXTevRegID reg, GXColor* color);

void GDSetTevColorS10(GXTevRegID reg, GXColorS10* color);

void GDSetTevKColor(GXTevKColorID reg, GXColor* color);

void GDSetTevKonstantSel(GXTevStageID evenStage, GXTevKColorSel kcsel0,
                         GXTevKAlphaSel kasel0, GXTevKColorSel kcsel1,
                         GXTevKAlphaSel kasel1);

void GDSetTevSwapModeTable(GXTevSwapSel table, GXTevColorChan red,
                           GXTevColorChan green, GXTevColorChan blue,
                           GXTevColorChan alpha);

void GDSetAlphaCompare(GXCompare comp0, u8 ref0, GXAlphaOp op, GXCompare comp1,
                       u8 ref1);

void GDSetZTexture(GXZTexOp op, GXTexFmt fmt, u32 bias);

void GDSetTevOrder(GXTevStageID evenStage, GXTexCoordID coord0, GXTexMapID map0,
                   GXChannelID color0, GXTexCoordID coord1, GXTexMapID map1,
                   GXChannelID color1);

#ifdef __cplusplus
}
#endif

#endif
