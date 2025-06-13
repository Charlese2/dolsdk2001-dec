#ifndef _DOLPHIN_MTX_H_
#define _DOLPHIN_MTX_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	f32 x, y, z;
} Vec, *VecPtr, Point3d, *Point3dPtr;

typedef struct
{
    s16 x, y, z;
} S16Vec, *S16VecPtr;

typedef struct
{
	f32 x, y, z, w;
} Quaternion, *QuaternionPtr, Qtrn, *QtrnPtr;

typedef f32 Mtx[3][4];
typedef f32 Mtx44[4][4];

typedef f32 (*MtxPtr)[4];
typedef f32 (*Mtx44Ptr)[4];

typedef f32 ROMtx[4][3];
typedef f32 (*ROMtxPtr)[3];

#ifdef DEBUG
#define VECSquareMag C_VECSquareMag
#define VECNormalize C_VECNormalize
#define VECAdd C_VECAdd
#define VECDotProduct C_VECDotProduct
#define VECSquareDistance C_VECSquareDistance
#define VECCrossProduct C_VECCrossProduct
#define MTXMultVec C_MTXMultVec
#define MTXMultVecArray C_MTXMultVecArray
#define MTXCopy C_MTXCopy
#define MTXConcat C_MTXConcat
#define MTXInverse C_MTXInverse
#define MTXTranspose C_MTXTranspose
#define MTXIdentity  C_MTXIdentity
#define MTX44Copy C_MTX44Copy
#else
#define VECSquareMag PSVECSquareMag
#define VECNormalize PSVECNormalize
#define VECAdd PSVECAdd
#define VECDotProduct PSVECDotProduct
#define VECSquareDistance PSVECSquareDistance
#define VECCrossProduct PSVECCrossProduct
#define MTXMultVec PSMTXMultVec
#define MTXMultVecArray PSMTXMultVecArray
#define MTXCopy PSMTXCopy
#define MTXConcat PSMTXConcat
#define MTXInverse PSMTXInverse
#define MTXTranspose PSMTXTranspose
#define MTXIdentity  PSMTXIdentity
#define MTX44Copy PSMTX44Copy
#endif

// asm only
#define MTXReorder PSMTXReorder
#define MTXROMultVecArray PSMTXROMultVecArray
#define MTXROSkin2VecArray PSMTXROSkin2VecArray
#define MTXROMultS16VecArray PSMTXROMultS16VecArray
#define MTXMultS16VecArray PSMTXMultS16VecArray

// mtx.c

// C functions
void C_MTXIdentity(Mtx m);
void C_MTXCopy(Mtx src, Mtx dst);
void C_MTXConcat(Mtx a, Mtx b, Mtx ab);
void C_MTXTranspose(Mtx src, Mtx xPose);
u32 C_MTXInverse(Mtx src, Mtx inv);
u32 C_MTXInvXpose(Mtx src, Mtx invX);
void C_MTXRotRad(Mtx m, char axis, f32 rad);
void C_MTXRotTrig(Mtx m, char axis, f32 sinA, f32 cosA);
void C_MTXRotAxisRad(Mtx m, Vec *axis, f32 rad);
void C_MTXTrans(Mtx m, f32 xT, f32 yT, f32 zT);
void C_MTXTransApply(Mtx src, Mtx dst, f32 xT, f32 yT, f32 zT);
void C_MTXScale(Mtx m, f32 xS, f32 yS, f32 zS);
void C_MTXScaleApply(Mtx src, Mtx dst, f32 xS, f32 yS, f32 zS);
void C_MTXQuat(Mtx m, QuaternionPtr q);
void C_MTXReflect(Mtx m, Vec *p, Vec *n);
void C_MTXLookAt(Mtx m, Vec *camPos, Vec *camUp, Vec *target);
void C_MTXLightFrustum(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 scaleS, f32 scaleT, f32 transS, f32 transT);
void C_MTXLightPerspective(Mtx m, f32 fovY, f32 aspect, f32 scaleS, f32 scaleT, f32 transS, f32 transT);
void C_MTXLightOrtho(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 scaleS, f32 scaleT, f32 transS, f32 transT);

// asm functions
void PSMTXIdentity(Mtx m);
void PSMTXCopy(Mtx src, Mtx dst);
void PSMTXConcat(Mtx mA, Mtx mB, Mtx mAB);
void PSMTXTranspose(Mtx src, Mtx xPose);
u32 PSMTXInverse(Mtx src, Mtx inv);
u32 PSMTXInvXpose(Mtx src, Mtx invX);
void PSMTXRotRad(Mtx m, char axis, f32 rad);
void PSMTXRotTrig(Mtx m, char axis, f32 sinA, f32 cosA);
void PSMTXRotAxisRad(Mtx m, Vec *axis, f32 rad);
void PSMTXTrans(Mtx m, f32 xT, f32 yT, f32 zT);
void PSMTXTransApply(Mtx src, Mtx dst, f32 xT, f32 yT, f32 zT);
void PSMTXScale(Mtx m, f32 xS, f32 yS, f32 zS);
void PSMTXScaleApply(Mtx src, Mtx dst, f32 xS, f32 yS, f32 zS);
void PSMTXQuat(Mtx m, QuaternionPtr q);
void PSMTXReflect(Mtx m, Vec *p, Vec *n);
void PSMTXLookAt(Mtx m, Vec *camPos, Vec *camUp, Vec *target);
void PSMTXLightFrustum(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 scaleS, f32 scaleT, f32 transS, f32 transT);
void PSMTXLightPerspective(Mtx m, f32 fovY, f32 aspect, f32 scaleS, f32 scaleT, f32 transS, f32 transT);
void PSMTXLightOrtho(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 scaleS, f32 scaleT, f32 transS, f32 transT);

// mtx44.c

// C functions
void C_MTX44Identity(Mtx44 m);
void C_MTX44Copy(Mtx44 src, Mtx44 dst);
void C_MTX44Concat(Mtx44 a, Mtx44 b, Mtx44 ab);
void C_MTX44Transpose(Mtx44 src, Mtx44 xPose);
void C_MTX44Trans(Mtx44 m, f32 xT, f32 yT, f32 zT);
void C_MTX44TransApply(Mtx44 src, Mtx44 dst, f32 xT, f32 yT , f32 zT);
void C_MTX44Scale(Mtx44 m, f32 xS, f32 yS, f32 zS);
void C_MTX44ScaleApply(Mtx44 src, Mtx44 dst, f32 xS, f32 yS, f32 zS);
void C_MTX44RotRad(Mtx44 m, char axis, f32 rad);
void C_MTX44RotTrig(Mtx44 m, char axis, f32 sinA, f32 cosA);
void C_MTX44RotAxisRad(Mtx44 m, Vec *axis, f32 rad);

// asm functions
void PSMTX44Identity(Mtx44 m);
void PSMTX44Copy(Mtx44 src, Mtx44 dst);
void PSMTX44Concat(Mtx44 a, Mtx44 b, Mtx44 ab);
void PSMTX44Transpose(Mtx44 src, Mtx44 xPose);
void PSMTX44Trans(Mtx44 m, f32 xT, f32 yT, f32 zT);
void PSMTX44TransApply(Mtx44 src, Mtx44 dst, f32 xT, f32 yT , f32 zT);
void PSMTX44Scale(Mtx44 m, f32 xS, f32 yS, f32 zS);
void PSMTX44ScaleApply(Mtx44 src, Mtx44 dst, f32 xS, f32 yS, f32 zS);
void PSMTX44RotRad(Mtx44 m, char axis, f32 rad);
void PSMTX44RotTrig(Mtx44 m, char axis, f32 sinA, f32 cosA);
void PSMTX44RotAxisRad(Mtx44 m, Vec *axis, f32 rad);

// mtxstack.c
typedef struct {
    u32 numMtx;
    Mtx *stackBase;
    Mtx *stackPtr;
} MTXStack;

void MTXInitStack(MTXStack *sPtr, u32 numMtx);
Mtx *MTXPush(MTXStack *sPtr, Mtx m);
Mtx *MTXPushFwd(MTXStack *sPtr, Mtx m);
Mtx *MTXPushInv(MTXStack *sPtr, Mtx m);
Mtx *MTXPushInvXpose(MTXStack *sPtr, Mtx m);
Mtx *MTXPop(MTXStack *sPtr);
Mtx *MTXGetStackPtr(MTXStack *sPtr);

// mtxvec.c
void MTXMultVecSR(Mtx44 m, Vec *src, Vec *dst);
void MTXMultVecArraySR(Mtx44 m, Vec *srcBase, Vec *dstBase, u32 count); 

// C functions
void C_MTXMultVec(Mtx44 m, Vec *src, Vec *dst);
void C_MTXMultVecArray(Mtx m, Vec *srcBase, Vec *dstBase, u32 count);

// asm functions
void PSMTXMultVec(Mtx44 m, Vec *src, Vec *dst);
void PSMTXMultVecArray(Mtx m, Vec *srcBase, Vec *dstBase, u32 count);

// psmtx.c
void PSMTXReorder(Mtx src, ROMtx dest);
void PSMTXROMultVecArray(ROMtx *m, Vec *srcBase, Vec *dstBase, u32 count);
void PSMTXROSkin2VecArray(ROMtx *m0, ROMtx *m1, f32 * wtBase, Vec *srcBase, Vec *dstBase, u32 count);
void PSMTXROMultS16VecArray(ROMtx *m, S16Vec *srcBase, Vec *dstBase, u32 count);
void PSMTXMultS16VecArray(Mtx44 *m, S16Vec *srcBase, Vec *dstBase, u32 count);

// vec.c
f32 VECMag(Vec *v);
void VECHalfAngle(Vec *a, Vec *b, Vec *half);
void VECReflect(Vec *src, Vec *normal, Vec *dst);
f32 VECDistance(Vec *a, Vec *b);

// C functions
void C_VECAdd(Vec *a, Vec *b, Vec *c);
void C_VECSubtract(Vec *a, Vec *b, Vec *c);
void C_VECScale(Vec *src, Vec *dst, f32 scale);
void C_VECNormalize(Vec *src, Vec *unit);
f32 C_VECSquareMag(Vec *v);
f32 C_VECDotProduct(Vec *a, Vec *b);
void C_VECCrossProduct(Vec *a, Vec *b, Vec *axb);
f32 C_VECSquareDistance(Vec *a, Vec *b);

// Asm functions
void PSVECAdd(Vec *a, Vec *b, Vec *c); 
void PSVECSubtract(Vec *a, Vec *b, Vec *c);
void PSVECScale(Vec *src, Vec *dst, f32 scale);
void PSVECNormalize(Vec *vec1, Vec *dst);
f32 PSVECSquareMag(Vec *vec1);
f32 PSVECDotProduct(Vec *vec1, Vec *vec2);
void PSVECCrossProduct(Vec *vec1, Vec *vec2, Vec *dst);
f32 PSVECSquareDistance(Vec *vec1, Vec *vec2);

#ifdef __cplusplus
}
#endif

#endif
