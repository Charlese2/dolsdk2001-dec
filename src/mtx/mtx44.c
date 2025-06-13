#include <dolphin.h>
#include <dolphin/mtx.h>
#include "fake_tgmath.h"

#define qr0 0

static float mtxUnit[4] = {0, 1, 0.5, 3};

// functions
void C_MTXFrustum(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f);
void C_MTXPerspective(Mtx m, f32 fovY, f32 aspect, f32 n, f32 f);
void C_MTXOrtho(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f);
u32 C_MTX44Inverse(Mtx44 src, Mtx44 inv);

void C_MTXFrustum(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f) {
    f32 tmp;

    ASSERTMSGLINE(0x63, m, "MTXFrustum():  NULL Mtx44Ptr 'm' ");
    ASSERTMSGLINE(0x64, t != b, "MTXFrustum():  't' and 'b' clipping planes are equal ");
    ASSERTMSGLINE(0x65, l != r, "MTXFrustum():  'l' and 'r' clipping planes are equal ");
    ASSERTMSGLINE(0x66, n != f, "MTXFrustum():  'n' and 'f' clipping planes are equal ");
    tmp = 1 / (r - l);
    m[0][0] = (2 * n * tmp);
    m[0][1] = 0;
    m[0][2] = (tmp * (r + l));
    m[0][3] = 0;
    tmp = 1 / (t - b);
    m[1][0] = 0;
    m[1][1] = (2 * n * tmp);
    m[1][2] = (tmp * (t + b));
    m[1][3] = 0;
    m[2][0] = 0;
    m[2][1] = 0;
    tmp = 1 / (f - n);
    m[2][2] = (-n * tmp);
    m[2][3] = (tmp * -(f * n));
    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = -1;
    m[3][3] = 0;
}

void C_MTXPerspective(Mtx m, f32 fovY, f32 aspect, f32 n, f32 f) {
    f32 angle;
    f32 cot;
    f32 tmp;

    ASSERTMSGLINE(0xAD, m, "MTXPerspective():  NULL Mtx44Ptr 'm' ");
    ASSERTMSGLINE(0xAE, (fovY > 0.0) && (fovY < 180.0), "MTXPerspective():  'fovY' out of range ");
    ASSERTMSGLINE(0xAF, 0.0f != aspect, "MTXPerspective():  'aspect' is 0 ");

    angle = (0.5f * fovY);
    angle = angle * 0.017453293f;
    cot = 1 / tanf(angle);
    m[0][0] = (cot / aspect);
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = 0;
    m[1][0] = 0;
    m[1][1] = (cot);
    m[1][2] = 0;
    m[1][3] = 0;
    m[2][0] = 0;
    m[2][1] = 0;
    tmp = 1 / (f - n);
    m[2][2] = (-n * tmp);
    m[2][3] = (tmp * -(f * n));
    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = -1;
    m[3][3] = 0;
}

void C_MTXOrtho(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f) {
    f32 tmp;

    ASSERTMSGLINE(0xF8, m, "MTXOrtho():  NULL Mtx44Ptr 'm' ");
    ASSERTMSGLINE(0xF9, t != b, "MTXOrtho():  't' and 'b' clipping planes are equal ");
    ASSERTMSGLINE(0xFA, l != r, "MTXOrtho():  'l' and 'r' clipping planes are equal ");
    ASSERTMSGLINE(0xFB, n != f, "MTXOrtho():  'n' and 'f' clipping planes are equal ");
    tmp = 1 / (r - l);
    m[0][0] = 2 * tmp;
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = (tmp * -(r + l));
    tmp = 1 / (t - b);
    m[1][0] = 0;
    m[1][1] = 2 * tmp;
    m[1][2] = 0;
    m[1][3] = (tmp * -(t + b));
    m[2][0] = 0;
    m[2][1] = 0;
    tmp = 1 / (f - n);
    m[2][2] = (-1 * tmp);
    m[2][3] = (-f * tmp);
    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;
}

void C_MTX44Identity(Mtx m) {
    ASSERTMSGLINE(0x13E, m, "MTX44Identity():  NULL Mtx44 'm' ");
    m[0][0] = 1;
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = 0;
    m[1][0] = 0;
    m[1][1] = 1;
    m[1][2] = 0;
    m[1][3] = 0;
    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = 1;
    m[2][3] = 0;
    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;
}

void PSMTX44Identity(register Mtx m) {
    register f32 c1 = 1.0f;
    register f32 c0 = 0.0f;
    asm {
        stfs c1, 0(m)
        psq_st c0, 4(m), 0, qr0
        psq_st c0, 12(m), 0, qr0
        stfs c1, 20(m)
        psq_st c0, 24(m), 0, qr0
        psq_st c0, 32(m), 0, qr0
        stfs c1, 40(m)
        psq_st c0, 44(m), 0, qr0
        psq_st c0, 52(m), 0, qr0
        stfs c1, 60(m)
    }
}

void C_MTX44Copy(Mtx src, Mtx dst) {
    ASSERTMSGLINE(0x178, src, "MTX44Copy():  NULL Mtx44Ptr 'src' ");
    ASSERTMSGLINE(0x179, dst, "MTX44Copy():  NULL Mtx44Ptr 'dst' ");
    if (src == dst) {
        return;
    }
    dst[0][0] = src[0][0];
    dst[0][1] = src[0][1];
    dst[0][2] = src[0][2];
    dst[0][3] = src[0][3];
    dst[1][0] = src[1][0];
    dst[1][1] = src[1][1];
    dst[1][2] = src[1][2];
    dst[1][3] = src[1][3];
    dst[2][0] = src[2][0];
    dst[2][1] = src[2][1];
    dst[2][2] = src[2][2];
    dst[2][3] = src[2][3];
    dst[3][0] = src[3][0];
    dst[3][1] = src[3][1];
    dst[3][2] = src[3][2];
    dst[3][3] = src[3][3];
}

asm void PSMTX44Copy(register Mtx src, register Mtx dst) {
    psq_l f1, 0(src), 0, qr0
    psq_st f1, 0(dst), 0, qr0
    psq_l f1, 8(src), 0, qr0
    psq_st f1, 8(dst), 0, qr0
    psq_l f1, 16(src), 0, qr0
    psq_st f1, 16(dst), 0, qr0
    psq_l f1, 24(src), 0, qr0
    psq_st f1, 24(dst), 0, qr0
    psq_l f1, 32(src), 0, qr0
    psq_st f1, 32(dst), 0, qr0
    psq_l f1, 40(src), 0, qr0
    psq_st f1, 40(dst), 0, qr0
    psq_l f1, 48(src), 0, qr0
    psq_st f1, 48(dst), 0, qr0
    psq_l f1, 56(src), 0, qr0
    psq_st f1, 56(dst), 0, qr0
}

void C_MTX44Concat(Mtx44 a, Mtx44 b, Mtx44 ab) {
    Mtx44 mTmp;
    f32 (*m)[4];

    ASSERTMSGLINE(0x1C0, a, "MTX44Concat():  NULL Mtx44Ptr 'a'  ");
    ASSERTMSGLINE(0x1C1, b, "MTX44Concat():  NULL Mtx44Ptr 'b'  ");
    ASSERTMSGLINE(0x1C2, ab, "MTX44Concat():  NULL Mtx44Ptr 'ab' ");

    if (ab == a || ab == b) {
        m = mTmp;
    } else {
        m = ab;
    }

    m[0][0] = a[0][3] * b[3][0] + (a[0][2] * b[2][0] + ((a[0][0] * b[0][0]) + (a[0][1] * b[1][0])));
    m[0][1] = a[0][3] * b[3][1] + (a[0][2] * b[2][1] + ((a[0][0] * b[0][1]) + (a[0][1] * b[1][1])));
    m[0][2] = a[0][3] * b[3][2] + (a[0][2] * b[2][2] + ((a[0][0] * b[0][2]) + (a[0][1] * b[1][2])));
    m[0][3] = a[0][3] * b[3][3] + (a[0][2] * b[2][3] + ((a[0][0] * b[0][3]) + (a[0][1] * b[1][3])));

    m[1][0] = a[1][3] * b[3][0] + (a[1][2] * b[2][0] + ((a[1][0] * b[0][0]) + (a[1][1] * b[1][0])));
    m[1][1] = a[1][3] * b[3][1] + (a[1][2] * b[2][1] + ((a[1][0] * b[0][1]) + (a[1][1] * b[1][1])));
    m[1][2] = a[1][3] * b[3][2] + (a[1][2] * b[2][2] + ((a[1][0] * b[0][2]) + (a[1][1] * b[1][2])));
    m[1][3] = a[1][3] * b[3][3] + (a[1][2] * b[2][3] + ((a[1][0] * b[0][3]) + (a[1][1] * b[1][3])));

    m[2][0] = a[2][3] * b[3][0] + (a[2][2] * b[2][0] + ((a[2][0] * b[0][0]) + (a[2][1] * b[1][0])));
    m[2][1] = a[2][3] * b[3][1] + (a[2][2] * b[2][1] + ((a[2][0] * b[0][1]) + (a[2][1] * b[1][1])));
    m[2][2] = a[2][3] * b[3][2] + (a[2][2] * b[2][2] + ((a[2][0] * b[0][2]) + (a[2][1] * b[1][2])));
    m[2][3] = a[2][3] * b[3][3] + (a[2][2] * b[2][3] + ((a[2][0] * b[0][3]) + (a[2][1] * b[1][3])));

    m[3][0] = a[3][3] * b[3][0] + (a[3][2] * b[2][0] + ((a[3][0] * b[0][0]) + (a[3][1] * b[1][0])));
    m[3][1] = a[3][3] * b[3][1] + (a[3][2] * b[2][1] + ((a[3][0] * b[0][1]) + (a[3][1] * b[1][1])));
    m[3][2] = a[3][3] * b[3][2] + (a[3][2] * b[2][2] + ((a[3][0] * b[0][2]) + (a[3][1] * b[1][2])));
    m[3][3] = a[3][3] * b[3][3] + (a[3][2] * b[2][3] + ((a[3][0] * b[0][3]) + (a[3][1] * b[1][3])));

    if (m == mTmp) {
        C_MTX44Copy(mTmp, ab);
    }
}

asm void PSMTX44Concat(register Mtx a, register Mtx b, register Mtx ab) {
    psq_l f0, 0(a), 0, qr0
    psq_l f2, 0(b), 0, qr0
    ps_muls0 f6, f2, f0
    psq_l f3, 16(b), 0, qr0
    psq_l f4, 32(b), 0, qr0
    ps_madds1 f6, f3, f0, f6
    psq_l f1, 8(a), 0, qr0
    psq_l f5, 48(b), 0, qr0
    ps_madds0 f6, f4, f1, f6
    psq_l f0, 16(a), 0, qr0
    ps_madds1 f6, f5, f1, f6
    psq_l f1, 24(a), 0, qr0
    ps_muls0 f8, f2, f0
    ps_madds1 f8, f3, f0, f8
    psq_l f0, 32(a), 0, qr0
    ps_madds0 f8, f4, f1, f8
    ps_madds1 f8, f5, f1, f8
    psq_l f1, 40(a), 0, qr0
    ps_muls0 f10, f2, f0
    ps_madds1 f10, f3, f0, f10
    psq_l f0, 48(a), 0, qr0
    ps_madds0 f10, f4, f1, f10
    ps_madds1 f10, f5, f1, f10
    psq_l f1, 56(a), 0, qr0
    ps_muls0 f12, f2, f0
    psq_l f2, 8(b), 0, qr0
    ps_madds1 f12, f3, f0, f12
    psq_l f0, 0(a), 0, qr0
    ps_madds0 f12, f4, f1, f12
    psq_l f3, 24(b), 0, qr0
    ps_madds1 f12, f5, f1, f12
    psq_l f1, 8(a), 0, qr0
    ps_muls0 f7, f2, f0
    psq_l f4, 40(b), 0, qr0
    ps_madds1 f7, f3, f0, f7
    psq_l f5, 56(b), 0, qr0
    ps_madds0 f7, f4, f1, f7
    psq_l f0, 16(a), 0, qr0
    ps_madds1 f7, f5, f1, f7
    psq_l f1, 24(a), 0, qr0
    ps_muls0 f9, f2, f0
    psq_st f6, 0(ab), 0, qr0
    ps_madds1 f9, f3, f0, f9
    psq_l f0, 32(a), 0, qr0
    ps_madds0 f9, f4, f1, f9
    psq_st f8, 16(ab), 0, qr0
    ps_madds1 f9, f5, f1, f9
    psq_l f1, 40(a), 0, qr0
    ps_muls0 f11, f2, f0
    psq_st f10, 32(ab), 0, qr0
    ps_madds1 f11, f3, f0, f11
    psq_l f0, 48(a), 0, qr0
    ps_madds0 f11, f4, f1, f11
    psq_st f12, 48(ab), 0, qr0
    ps_madds1 f11, f5, f1, f11
    psq_l f1, 56(a), 0, qr0
    ps_muls0 f13, f2, f0
    psq_st f7, 8(ab), 0, qr0
    ps_madds1 f13, f3, f0, f13
    psq_st f9, 24(ab), 0, qr0
    ps_madds0 f13, f4, f1, f13
    psq_st f11, 40(ab), 0, qr0
    ps_madds1 f13, f5, f1, f13
    psq_st f13, 56(ab), 0, qr0
}

void C_MTX44Transpose(Mtx44 src, Mtx44 xPose) {
    Mtx44 mTmp;
    f32 (*m)[4];

    ASSERTMSGLINE(0x277, src, "MTX44Transpose():  NULL Mtx44Ptr 'src' ");
    ASSERTMSGLINE(0x278, xPose, "MTX44Transpose():  NULL Mtx44Ptr 'xPose' ");

    if (src == xPose) {
        m = mTmp;
    } else {
        m = xPose;
    }

    m[0][0] = src[0][0];
    m[0][1] = src[1][0];
    m[0][2] = src[2][0];
    m[0][3] = src[3][0];
    m[1][0] = src[0][1];
    m[1][1] = src[1][1];
    m[1][2] = src[2][1];
    m[1][3] = src[3][1];
    m[2][0] = src[0][2];
    m[2][1] = src[1][2];
    m[2][2] = src[2][2];
    m[2][3] = src[3][2];
    m[3][0] = src[0][3];
    m[3][1] = src[1][3];
    m[3][2] = src[2][3];
    m[3][3] = src[3][3];

    if (m == mTmp) {
        MTX44Copy(mTmp, xPose);
    }
}

asm void PSMTX44Transpose(register Mtx44 src, register Mtx44 xPose) {
    psq_l f0, 0(src), 0, qr0
    psq_l f1, 16(src), 0, qr0
    ps_merge00 f4, f0, f1
    psq_l f2, 8(src), 0, qr0
    psq_st f4, 0(xPose), 0, qr0
    ps_merge11 f5, f0, f1
    psq_l f3, 24(src), 0, qr0
    psq_st f5, 16(xPose), 0, qr0
    ps_merge00 f4, f2, f3
    psq_l f0, 32(src), 0, qr0
    psq_st f4, 32(xPose), 0, qr0
    ps_merge11 f5, f2, f3
    psq_l f1, 48(src), 0, qr0
    psq_st f5, 48(xPose), 0, qr0
    ps_merge00 f4, f0, f1
    psq_l f2, 40(src), 0, qr0
    psq_st f4, 8(xPose), 0, qr0
    ps_merge11 f5, f0, f1
    psq_l f3, 56(src), 0, qr0
    psq_st f5, 24(xPose), 0, qr0
    ps_merge00 f4, f2, f3
    psq_st f4, 40(xPose), 0, qr0
    ps_merge11 f5, f2, f3
    psq_st f5, 56(xPose), 0, qr0
}

u32 C_MTX44Inverse(Mtx44 src, Mtx44 inv) {
    Mtx44 gjm;
    s32 i;
    s32 j;
    s32 k;
    f32 w;
    f32 max;
    s32 swp;
    f32 ftmp;
    //float tmp; // f27
    //float tmp; // f28

    ASSERTMSGLINE(0x2D8, src, "MTX44Inverse():  NULL Mtx44Ptr 'src' ");
    ASSERTMSGLINE(0x2d9, inv, "MTX44Inverse():  NULL Mtx44Ptr 'inv' ");

    MTX44Copy(src, gjm);
    MTXIdentity(inv);

    for (;;) {
        max = 0;
        swp = i;
        k = i;


        return 0;
    }

    return 1;
}

void C_MTX44Trans(Mtx44 m, f32 xT, f32 yT, f32 zT) {
    ASSERTMSGLINE(0x33D, m, "MTX44Trans():  NULL Mtx44Ptr 'm' ");

    m[0][0] = 1;
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = xT;
    m[1][0] = 0;
    m[1][1] = 1;
    m[1][2] = 0;
    m[1][3] = yT;
    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = 1;
    m[2][3] = zT;
    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;
}

void PSMTX44Trans(register Mtx44 m, register f32 xT, register f32 yT, register f32 zT) {
    register f32 c_zero = 0;
    register f32 c_one = 1;
    register f32 c_01;

    asm {
        stfs xT, 12(m)
        stfs yT, 28(m)
        ps_merge00 c_01, c_zero, c_one
        stfs zT, 44(m)
        psq_st c_one, 0(m), 1, qr0
        psq_st c_zero, 4(m), 0, qr0
        psq_st c_01, 16(m), 0, qr0
        psq_st c_zero, 24(m), 1, qr0
        psq_st c_zero, 32(m), 0, qr0
        psq_st c_one, 40(m), 1, qr0
        psq_st c_zero, 48(m), 0, qr0
        psq_st c_01, 56(m), 0, qr0
    }
}

void C_MTX44TransApply(Mtx44 src, Mtx44 dst, f32 xT, f32 yT , f32 zT) {
    ASSERTMSGLINE(0x37D, src, "MTX44TransApply(): NULL Mtx44Ptr 'src' ");
    ASSERTMSGLINE(0x37E, dst, "MTX44TransApply(): NULL Mtx44Ptr 'src' ");

    if (src != dst) {
        dst[0][0] = src[0][0];
        dst[0][1] = src[0][1];
        dst[0][2] = src[0][2];
        dst[1][0] = src[1][0];
        dst[1][1] = src[1][1];
        dst[1][2] = src[1][2];
        dst[2][0] = src[2][0];
        dst[2][1] = src[2][1];
        dst[2][2] = src[2][2];
        dst[3][0] = src[3][0];
        dst[3][1] = src[3][1];
        dst[3][2] = src[3][2];
        dst[3][3] = src[3][3];
    }
    dst[0][3] = src[0][3] + xT;
    dst[1][3] = src[1][3] + yT;
    dst[2][3] = src[2][3] + zT;
}

asm void PSMTX44TransApply(register Mtx44 src, register Mtx44 dst, register f32 xT, register f32 yT, register f32 zT) {
    psq_l f4, 0(src), 0, qr0
    ps_merge11 f9, xT, xT
    psq_l f5, 8(src), 0, qr0
    ps_merge11 f10, yT, yT
    psq_st f4, 0(dst), 0, qr0
    ps_merge11 f11, zT, zT
    psq_l f6, 16(src), 0, qr0
    ps_sum1 f5, f9, f5, f5
    psq_l f7, 24(src), 0, qr0
    ps_sum1 f7, f10, f7, f7
    psq_l f8, 32(src), 0, qr0
    psq_st f5, 8(dst), 0, qr0
    psq_l f4, 40(src), 0, qr0
    psq_st f6, 16(dst), 0, qr0
    ps_sum1 f4, f11, f4, f4
    psq_st f7, 24(dst), 0, qr0
    psq_l f5, 48(src), 0, qr0
    psq_st f8, 32(dst), 0, qr0
    psq_l f6, 56(src), 0, qr0
    psq_st f4, 40(dst), 0, qr0
    psq_st f5, 48(dst), 0, qr0
    psq_st f6, 56(dst), 0, qr0
}

void C_MTX44Scale(Mtx44 m, f32 xS, f32 yS, f32 zS) {
    ASSERTMSGLINE(0x3CA, m, "MTX44Scale():  NULL Mtx44Ptr 'm' ");

    m[0][0] = xS;
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = 0;
    m[1][0] = 0;
    m[1][1] = yS;
    m[1][2] = 0;
    m[1][3] = 0;
    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = zS;
    m[2][3] = 0;
    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;
}

void PSMTX44Scale(register Mtx44 m, register f32 xS, register f32 yS, register f32 zS) {
    register f32 c_zero = 0;
    register f32 c_one = 1;

    asm {
        stfs xS, 0(m)
        psq_st c_zero, 4(m), 0, qr0
        psq_st c_zero, 12(m), 0, qr0
        stfs yS, 20(m)
        psq_st c_zero, 24(m), 0, qr0
        psq_st c_zero, 32(m), 0, qr0
        stfs zS, 40(m)
        psq_st c_zero, 44(m), 0, qr0
        psq_st c_zero, 52(m), 0, qr0
        stfs c_one, 60(m)
    }
}

void C_MTX44ScaleApply(Mtx44 src, Mtx44 dst, f32 xS, f32 yS, f32 zS) {
    ASSERTMSGLINE(0x406, src, "MTX44ScaleApply(): NULL Mtx44Ptr 'src' ");
    ASSERTMSGLINE(0x407, dst, "MTX44ScaleApply(): NULL Mtx44Ptr 'dst' ");

    dst[0][0] = src[0][0] * xS;
    dst[0][1] = src[0][1] * xS;
    dst[0][2] = src[0][2] * xS;
    dst[0][3] = src[0][3] * xS;
    dst[1][0] = src[1][0] * yS;
    dst[1][1] = src[1][1] * yS;
    dst[1][2] = src[1][2] * yS;
    dst[1][3] = src[1][3] * yS;
    dst[2][0] = src[2][0] * zS;
    dst[2][1] = src[2][1] * zS;
    dst[2][2] = src[2][2] * zS;
    dst[2][3] = src[2][3] * zS;
    dst[3][0] = src[3][0];
    dst[3][1] = src[3][1];
    dst[3][2] = src[3][2];
    dst[3][3] = src[3][3];
}

asm void PSMTX44ScaleApply(register Mtx44 src, register Mtx44 dst, register f32 xS, register f32 yS, register f32 zS) {
    psq_l f4, 0(src), 0, qr0
    psq_l f5, 8(src), 0, qr0
    psq_l f6, 16(src), 0, qr0
    ps_muls0 f4, f4, xS
    psq_l f7, 24(src), 0, qr0
    ps_muls0 f5, f5, xS
    psq_l f8, 32(src), 0, qr0
    psq_st f4, 0(dst), 0, qr0
    ps_muls0 f6, f6, yS
    psq_l f9, 40(src), 0, qr0
    psq_st f5, 8(dst), 0, qr0
    ps_muls0 f7, f7, yS
    psq_l f10, 48(src), 0, qr0
    psq_st f6, 16(dst), 0, qr0
    ps_muls0 f8, f8, zS
    psq_l f11, 56(src), 0, qr0
    psq_st f7, 24(dst), 0, qr0
    ps_muls0 f9, f9, zS
    psq_st f8, 32(dst), 0, qr0
    psq_st f9, 40(dst), 0, qr0
    psq_st f10, 48(dst), 0, qr0
    psq_st f11, 56(dst), 0, qr0
}

void C_MTX44RotRad(Mtx44 m, char axis, f32 rad) {
    f32 sinA;
    f32 cosA;

    ASSERTMSGLINE(0x455, m, "MTX44RotRad():  NULL Mtx44Ptr 'm' ");

    sinA = sinf(rad);
    cosA = cosf(rad);
    C_MTX44RotTrig(m, axis, sinA, cosA);
}

void PSMTX44RotRad(Mtx44 m, char axis, f32 rad) {
    f32 sinA;
    f32 cosA;

    sinA = sinf(rad);
    cosA = cosf(rad);
    PSMTX44RotTrig(m, axis, sinA, cosA);
}

void C_MTX44RotTrig(Mtx44 m, char axis, f32 sinA, f32 cosA) {
    ASSERTMSGLINE(0x482, m, "MTX44RotTrig():  NULL Mtx44Ptr 'm' ");

    axis |= ' ';

    switch(axis) {
        case 'x':
            m[0][0] = 1;
            m[0][1] = 0;
            m[0][2] = 0;
            m[0][3] = 0;
            m[1][0] = 0;
            m[1][1] = cosA;
            m[1][2] = -sinA;
            m[1][3] = 0;
            m[2][0] = 0;
            m[2][1] = sinA;
            m[2][2] = cosA;
            m[2][3] = 0;
            m[3][0] = 0;
            m[3][1] = 0;
            m[3][2] = 0;
            m[3][3] = 1;
            break;
        case 'y':
            m[0][0] = cosA;
            m[0][1] = 0;
            m[0][2] = sinA;
            m[0][3] = 0;
            m[1][0] = 0;
            m[1][1] = 1;
            m[1][2] = 0;
            m[1][3] = 0;
            m[2][0] = -sinA;
            m[2][1] = 0;
            m[2][2] = cosA;
            m[2][3] = 0;
            m[3][0] = 0;
            m[3][1] = 0;
            m[3][2] = 0;
            m[3][3] = 1;
            break;
        case 'z':
            m[0][0] = cosA;
            m[0][1] = -sinA;
            m[0][2] = 0;
            m[0][3] = 0;
            m[1][0] = sinA;
            m[1][1] = cosA;
            m[1][2] = 0;
            m[1][3] = 0;
            m[2][0] = 0;
            m[2][1] = 0;
            m[2][2] = 1;
            m[2][3] = 0;
            m[3][0] = 0;
            m[3][1] = 0;
            m[3][2] = 0;
            m[3][3] = 1;
            break;
        default:
            ASSERTMSGLINE(0x49E, FALSE, "MTX44RotTrig():  invalid 'axis' value ");
            break;
    }
}

void PSMTX44RotTrig(register Mtx44 m, register char axis, register f32 sinA, register f32 cosA) {
    register f32 ftmp0;
    register f32 ftmp1;
    register f32 ftmp2;
    register f32 ftmp3;
    register f32 ftmp4;
    register f32 c_zero = 0;
    register f32 c_one = 1;
    asm {
        ori axis, axis, ' '
        cmplwi axis, 'x'
        beq case_x
        cmplwi axis, 'y'
        beq case_y
        cmplwi axis, 'z'
        beq case_z
        b end
    case_x:
        psq_st c_one, 0(m), 1, qr0
        psq_st c_zero, 4(m), 0, qr0
        ps_neg ftmp0, sinA
        psq_st c_zero, 12(m), 0, qr0
        ps_merge00 ftmp1, sinA, cosA
        psq_st c_zero, 28(m), 0, qr0
        ps_merge00 ftmp0, cosA, ftmp0
        psq_st c_zero, 44(m), 0, qr0
        psq_st c_zero, 52(m), 0, qr0
        psq_st ftmp1, 36(m), 0, qr0
        psq_st ftmp0, 20(m), 0, qr0
        psq_st c_one, 60(m), 1, qr0
        b end
    case_y:
        ps_merge00 ftmp1, cosA, c_zero
        psq_st c_zero, 48(m), 0, qr0
        ps_neg ftmp0, sinA
        psq_st c_zero, 24(m), 0, qr0
        ps_merge00 ftmp3, c_zero, c_one
        psq_st ftmp1, 0(m), 0, qr0
        ps_merge00 ftmp4, ftmp0, c_zero
        ps_merge00 ftmp2, sinA, c_zero
        psq_st ftmp3, 16(m), 0, qr0
        psq_st ftmp2, 8(m), 0, qr0
        psq_st ftmp4, 32(m), 0, qr0
        psq_st ftmp1, 40(m), 0, qr0
        psq_st ftmp3, 56(m), 0, qr0
        b end
    case_z:
        psq_st c_zero, 8(m), 0, qr0
        ps_neg ftmp0, sinA
        psq_st c_zero, 24(m), 0, qr0
        ps_merge00 ftmp1, sinA, cosA
        psq_st c_zero, 32(m), 0, qr0
        ps_merge00 ftmp2, c_one, c_zero
        psq_st c_zero, 48(m), 0, qr0
        ps_merge00 ftmp3, c_zero, c_one
        psq_st ftmp1, 16(m), 0, qr0
        ps_merge00 ftmp4, cosA, ftmp0
        psq_st ftmp2, 40(m), 0, qr0
        psq_st ftmp3, 56(m), 0, qr0
        psq_st ftmp4, 0(m), 0, qr0
    end:
    }
}

void C_MTX44RotAxisRad(Mtx44 m, Vec *axis, f32 rad) {
    Vec vN;
    f32 s;
    f32 c;
    f32 t;
    f32 x;
    f32 y;
    f32 z;
    f32 xSq;
    f32 ySq;
    f32 zSq;

    ASSERTMSGLINE(0x505, m, "MTX44RotAxisRad():  NULL MtxPtr 'm' ");
    ASSERTMSGLINE(0x506, axis, "MTX44RotAxisRad():  NULL VecPtr 'axis' ");

    s = sinf(rad);
    c = cosf(rad);
    t = 1 - c;
    C_VECNormalize(axis, &vN);
    x = vN.x;
    y = vN.y;
    z = vN.z;
    xSq = (x * x);
    ySq = (y * y);
    zSq = (z * z);
    m[0][0] = (c + (t * xSq));
    m[0][1] = (y * (t * x)) - (s * z);
    m[0][2] = (z * (t * x)) + (s * y);
    m[0][3] = 0;
    m[1][0] = ((y * (t * x)) + (s * z));
    m[1][1] = (c + (t * ySq));
    m[1][2] = ((z * (t * y)) - (s * x));
    m[1][3] = 0;
    m[2][0] = ((z * (t * x)) - (s * y));
    m[2][1] = ((z * (t * y)) + (s * x));
    m[2][2] = (c + (t * zSq));
    m[2][3] = 0;
    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;
}

asm void PSMTX44RotAxisRad(register Mtx44 m, register Vec *axis, register f32 rad) {
    nofralloc
    mflr r0
    stw r0, 4(r1)
    stwu r1, -80(r1)
    stfd f31, 32(r1)
    stfd f30, 24(r1)
    stw r31, 20(r1)
    stw r30, 16(r1)
    fmr f31, rad
    mr r31, axis
    mr r30, m
    bl sinf
    fmr f30, f1
    fmr f1, f31
    bl cosf
    mr r4, r31
    ps_merge00 f31, f1, f1
    mr m, r30
    psq_l f1, 0(axis), 0, qr0
    lis r5, mtxUnit@ha
    ps_mul f3, f1, f1
    lfs f2, 8(axis)
    ps_madd f4, f2, f2, f3
    lfs f7, mtxUnit+8@l(r5)
    ps_sum0 f3, f4, f2, f3
    lfs f8, mtxUnit+12@l(r5)
    frsqrte f4, f3
    lfs f12, mtxUnit@l(r5)
    fmuls f5, f4, f4
    lfs f13, mtxUnit+4@l(r5)
    fmuls f6, f7, f4
    stfs f12, 12(m)
    fnmsubs f5, f5, f3, f8
    stfs f13, 60(m)
    fmuls f4, f6, f5
    stfs f12, 28(m)
    ps_muls0 f1, f1, f4
    psq_st f12, 44(m), 0, qr0
    fmuls f2, f2, f4
    ps_mul f3, f1, f1
    psq_st f12, 52(m), 0, qr0
    ps_mul f4, f2, f2
    fsubs f13, f13, f31
    ps_madd f5, f3, f13, f31
    ps_madd f6, f4, f13, f31
    psq_st f5, 0(m), 1, qr0
    ps_merge10 f8, f5, f5
    ps_muls0 f7, f1, f2
    psq_st f6, 40(m), 1, qr0
    ps_merge00 f13, f13, f13
    ps_muls0 f5, f1, f30
    ps_merge10 f7, f7, f7
    psq_st f8, 20(m), 1, qr0
    ps_madd f3, f7, f13, f5
    ps_msub f4, f7, f13, f5
    psq_st f3, 36(m), 1, qr0
    ps_merge10 f6, f3, f3
    psq_st f4, 24(m), 1, qr0
    ps_muls1 f7, f1, f1
    ps_merge10 f5, f4, f4
    psq_st f6, 8(m), 1, qr0
    ps_muls0 f8, f2, f30
    psq_st f5, 32(m), 1, qr0
    ps_madd f3, f7, f13, f8
    psq_st f3, 16(m), 1, qr0
    ps_msub f4, f7, f13, f8
    psq_st f4, 4(m), 1, qr0
    lwz r0, 84(r1)
    lfd f31, 32(r1)
    lfd f30, 24(r1)
    lwz r31, 20(r1)
    lwz r30, 16(r1)
    addi r1, r1, 80
    mtlr r0
    blr
}