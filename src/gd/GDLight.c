#include <dolphin/gd/GDLight.h>
#include <math.h>
#include "__gd.h"

void GDSetLightAttn(GXLightID light, float a0, float a1, float a2, float k0,
                    float k1, float k2)
{
	GDWriteXFCmdHdr(__GDLightID2Offset(light) + 0x604, 6);
	GDWrite_f32(a0);
	GDWrite_f32(a1);
	GDWrite_f32(a2);
	GDWrite_f32(k0);
	GDWrite_f32(k1);
	GDWrite_f32(k2);
}

void GDSetLightSpot(GXLightID light, float cutoff, GXSpotFn spot_func)
{
  float a0;
  float a1;
  float a2;

	float r;
  float d;
  float cr;

  if (cutoff <= 0.0f || cutoff > 90.0f) {
    spot_func = 0;
  }

	r = 3.1415926535f * cutoff / 180.0f;
  cr = cosf(r);

  switch(spot_func) {
  case GX_SP_FLAT:
    a0 = -1000.0f * cr;
    a1 = 1000.0f;
    a2 = 0.0f;
    break;
  case GX_SP_COS:
    a0 = -cr / (1.0f - cr);
    a1 = 1.0f / (1.0f - cr);
    a2 = 0.0f;
    break;
  case GX_SP_COS2:
    a0 = 0.0f;
    a1 = (-cr / (1.0f - cr));
    a2 = (1.0f / (1.0f - cr));
    break;
  case GX_SP_SHARP:
    d = (1.0f - cr) * (1.0f - cr);
    a0 = ((cr * (cr - 2.0f)) / d);
    a1 = (2.0f / d);
    a2 = (-1.0f / d);
    break;
  case GX_SP_RING1:
    d = (1.0f - cr) * (1.0f - cr);
    a0 = ((-4.0f * cr) / d);
    a1 = ((4.0f * (1.0f + cr)) / d);
    a2 = (-4.0f / d);
    break;
  case GX_SP_RING2:
    d = (1.0f - cr) * (1.0f - cr);
    a0 = (1.0f -
                    (2.0f * cr * cr) / d);
    a1 = ((4.0f * cr) / d);
    a2 = (-2.0f / d);
		break;
  case GX_SP_OFF:
	default:
    a0 = 1.0f;
    a1 = 0.0f;
    a2 = 0.0f;
		break;
  }
  GDWriteXFCmdHdr(__GDLightID2Offset(light) + 0x604,3);
  GDWrite_f32(a0);
  GDWrite_f32(a1);
  GDWrite_f32(a2);
}

void GDSetLightDistAttn(GXLightID light, float ref_dist, float ref_br,
                        GXDistAttnFn dist_func)
{
	float k0;
	float k1;
	float k2;

	if (ref_dist < 0.0f || ref_br <= 0.0f || ref_br >= 1.0f)
	{
		dist_func = 0;
	}

	switch (dist_func)
	{
		case GX_DA_GENTLE:
			k0 = 1.0f;
			k1 = ((1.0f - ref_br) / (ref_br * ref_dist));
			k2 = 0.0f;
			break;
		case GX_DA_MEDIUM:
			k0 = 1.0f;
			k1 = ((0.5f * (1.0f - ref_br)) /
											(ref_br * ref_dist));
			k2 = ((0.5f * (1.0f - ref_br)) /
											(ref_dist * (ref_br * ref_dist)));
			break;
		case GX_DA_STEEP:
			k0 = 1.0f;
      k1 = 0.0f;
      k2 = ((1.0f - ref_br) /
                      (ref_dist * (ref_br * ref_dist)));
			break;
		default:
		case GX_DA_OFF:
			k0 = 1.0f;
			k1 = 0.0f;
			k2 = 0.0f;
			break;
	}

  GDWriteXFCmdHdr(__GDLightID2Offset(light) + 0x607,3);
  GDWrite_f32(k0);
  GDWrite_f32(k1);
  GDWrite_f32(k2);
}

void GDSetLightColor(GXLightID light, GXColor* color)
{
	GDWriteXFCmd(__GDLightID2Offset(light) + 0x603,
	             color->r << 24 | color->g << 16 | color->b << 8 | color->a);
}

void GDSetLightPos(GXLightID light, float x, float y, float z)
{
	GDWriteXFCmdHdr(__GDLightID2Offset(light) + 0x60a, 3);
	GDWrite_f32(x);
	GDWrite_f32(y);
	GDWrite_f32(z);
}

void GDSetLightDir(GXLightID light, float nx, float ny, float nz)
{
	GDWriteXFCmdHdr(__GDLightID2Offset(light) + 0x60d, 3);
	GDWrite_f32(nx);
	GDWrite_f32(ny);
	GDWrite_f32(nz);
}

void GDSetSpecularDirHA(GXLightID light, float nx, float ny, float nz,
                        float hx, float hy, float hz)
{
	float px;
	float py;
	float pz;

	px = 1048576.0f * -nx;
  py = 1048576.0f * -ny;
  pz = 1048576.0f * -nz;
  GDWriteXFCmdHdr(__GDLightID2Offset(light) + 0x60a,6);
  GDWrite_f32(px);
  GDWrite_f32(py);
  GDWrite_f32(pz);
  GDWrite_f32(hx);
  GDWrite_f32(hy);
  GDWrite_f32(hz);
}

void GDSetSpecularDir(GXLightID light, float nx, float ny, float nz)
{
	float px;
	float py;
	float pz;
	float hx;
	float hy;
	float hz;
	float mag;

	hx = -nx;
  hy = -ny;
  hz = 1.0f + -nz;
	mag = 1.0f / sqrtf(hx * hx + hy * hy + hz * hz);
  hx = hx * mag;
  hy = hy * mag;
  hz = hz * mag;
  px = 1048576.0f * -nx;
  py = 1048576.0f * -ny;
  pz = 1048576.0f * -nz;

  GDWriteXFCmdHdr(__GDLightID2Offset(light) + 0x60a, 6);
  GDWrite_f32(px);
  GDWrite_f32(py);
  GDWrite_f32(pz);
  GDWrite_f32(hx);
  GDWrite_f32(hy);
  GDWrite_f32(hz);
}

void GDLoadLightObjIndx(u32 lt_obj_indx, GXLightID light)
{
  GDWriteXFIndxDCmd(__GDLightID2Offset(light) + 0x600, 0x10, lt_obj_indx);
}

void GDSetChanAmbColor(GXChannelID chan, GXColor* color)
{
  GDWriteXFCmd((chan & 1) + 0x100a,
	             color->r << 24 | color->g << 16 | color->b << 8 | color->a);
}

void GDSetChanMatColor(GXChannelID chan, GXColor* color)
{
	GDWriteXFCmd((chan & 1) + 0x100c,
	             color->r << 24 | color->g << 16 | color->b << 8 | color->a);
}

void GDSetChanCtrl(GXChannelID chan, u8 enable, GXColorSrc amb_src,
                   GXColorSrc mat_src, u32 light_mask, GXDiffuseFn diff_fn,
                   GXAttnFn attn_fn)
{
	u32 reg;

	reg = mat_src
		| enable << 1
		| (light_mask & 0xf) << 2
		| amb_src << 6
		| (attn_fn == 0 ? 0 : diff_fn) << 7
		| (attn_fn != 2) << 9
		| (attn_fn != 0) << 10
		| (light_mask & 0xf0) << 7;
	GDWriteXFCmd((chan & 3) + 0x100e, reg);
	if ((chan == 4) || (chan == 5)) {
		GDWriteXFCmd(chan + 0x100c, reg);
	}
}
