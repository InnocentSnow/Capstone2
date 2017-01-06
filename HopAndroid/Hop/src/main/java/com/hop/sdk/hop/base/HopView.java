package com.hop.sdk.hop.base;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.widget.FrameLayout;

import com.hop.sdk.hop.impl.HopRendererImpl;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class HopView extends FrameLayout
{
    private Context mContext;

    private GLSurfaceView mGLSurfaceView;
    private HopView.Renderer mRenderer;
    private HopRendererImpl mRendererImpl;

    public HopView(Context context)
    {
        super(context);
        init(context);
    }

    public HopView(Context context, AttributeSet attrs)
    {
        super(context, attrs);
        init(context);
    }

    private void init(Context context)
    {
        if(!isInEditMode())
        {
            mGLSurfaceView = new GLSurfaceView(context);
            mGLSurfaceView.setEGLContextClientVersion(2);
            mGLSurfaceView.setPreserveEGLContextOnPause(true);

            addView(mGLSurfaceView);
        }

        mContext = context;
    }

    public void setRenderer(HopView.Renderer renderer)
    {
        if(renderer == null)
        {
            throw new IllegalArgumentException("Null Renderer");
        }
        else
        {
            mRenderer = renderer;
            mRendererImpl = new HopRendererImpl(mContext, mRenderer);

            mGLSurfaceView.setRenderer(mRendererImpl);
        }
    }

    public interface Renderer
    {
        void onBeforeDrawFrame();
        void onAfterDrawFrame();
        void onDrawSide(HopSide side);

        void onSurfaceCreated();
        void onSurfaceChanged(int width, int height);
    }
}
