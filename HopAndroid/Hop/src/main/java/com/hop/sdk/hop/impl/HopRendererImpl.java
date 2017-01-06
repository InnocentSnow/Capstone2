package com.hop.sdk.hop.impl;

import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;

import com.hop.sdk.hop.api.HopAPI;
import com.hop.sdk.hop.base.HopSide;
import com.hop.sdk.hop.base.HopView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class HopRendererImpl implements GLSurfaceView.Renderer
{
    private HopView.Renderer mRenderer;
    private HopRenderTarget mRenderTarget[];
    private HopPyramid mPyramid;
    private Context mContext;

    private int mWidth;
    private int mHeight;

    public HopRendererImpl(Context context, HopView.Renderer renderer)
    {
        mContext = context;
        mRenderer = renderer;
        mRenderTarget = new HopRenderTarget[4];
        mPyramid = new HopPyramid();
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
        HopShaderMgr.load(mContext);
        mRenderer.onSurfaceCreated();

        for(int i = 0; i < mRenderTarget.length; ++i)
        {
            mRenderTarget[i] = new HopRenderTarget();
        }
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height)
    {
        int size = Math.min(width, height);
        mRenderer.onSurfaceChanged(size, size);

        mWidth = size;
        mHeight = size;
    }

    @Override
    public void onDrawFrame(GL10 gl)
    {
        final float Y_RATE = 0.4f;

        //Left Right Front Back
        float dX[] = {1, -1, 0, 0}; //mirrored
        float dZ[] = {0, 0, 1, -1};

        float x = HopAPI.getX();
        float y = HopAPI.getY();
        float z = HopAPI.getZ();
        float size = HopAPI.getSize() / 2;
        float cy = y + size * Y_RATE;

        mRenderer.onBeforeDrawFrame();

        for(int i = 0; i < mRenderTarget.length; ++i)
        {
            mRenderTarget[i].applyRenderTarget();
            mRenderer.onDrawSide(new HopSide(x + dX[i] * size, cy, z + dZ[i] * size, x, cy, z));
        }

        HopRenderTarget.cancelRenderTarget(mWidth, mHeight);

        GLES20.glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);

        mPyramid.draw(mRenderTarget);
        mRenderer.onAfterDrawFrame();
    }
}
