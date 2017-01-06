package com.hop.sdk.hop.base;

import android.opengl.Matrix;
import com.hop.sdk.hop.api.HopAPI;

public class HopSide
{
    private static final float EPSILON = 0.1f;
    private static final float FOV_Y = 90.0f;
    private static final float ASPECT = 2.0f;

    private float mEye[];
    private float mAt[];

    public HopSide(float eyeX, float eyeY, float eyeZ, float atX, float atY, float atZ)
    {
        mEye = new float[] {eyeX, eyeY, eyeZ};
        mAt = new float[] {atX, atY, atZ};
    }

    public float[] getView()
    {
        float matView[] = new float[16];
        Matrix.setLookAtM(matView, 0, mEye[0], mEye[1], mEye[2], mAt[0], mAt[1], mAt[2], 0, 1, 0);

        return matView;
    }

    public float[] getProjection()
    {
        float size = HopAPI.getSize() / 2;
        float zNear = Math.min(EPSILON, EPSILON * size);

        float matProj[] = new float[16];
        Matrix.perspectiveM(matProj, 0, FOV_Y, ASPECT, zNear, zNear + 1);

        matProj[10] = -1;
        matProj[14] = -2 * zNear;

        return matProj;
    }

    public float[] getViewProjection()
    {
        float matViewProj[] = new float[16];
        Matrix.multiplyMM(matViewProj, 0, getProjection(), 0, getView(), 0);

        return matViewProj;
    }
}
