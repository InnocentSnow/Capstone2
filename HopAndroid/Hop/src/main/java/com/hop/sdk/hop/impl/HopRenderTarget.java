package com.hop.sdk.hop.impl;

import android.opengl.GLES20;
import android.util.Log;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;

class HopRenderTarget
{
    private static int RENDER_TARGET_WIDTH = 1024;
    private static int RENDER_TARGET_HEIGHT = 1024;

    private int mFrameBuffer;
    private int mDepthBuffer;
    private int mTexture;

    public void applyTexture()
    {
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTexture);
    }

    public void applyRenderTarget()
    {
        GLES20.glViewport(0, 0, RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT);
        GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, mFrameBuffer);
        GLES20.glFramebufferTexture2D(GLES20.GL_FRAMEBUFFER, GLES20.GL_COLOR_ATTACHMENT0, GLES20.GL_TEXTURE_2D, mTexture, 0);
        GLES20.glFramebufferRenderbuffer(GLES20.GL_FRAMEBUFFER, GLES20.GL_DEPTH_ATTACHMENT, GLES20.GL_RENDERBUFFER, mDepthBuffer);
    }

    public static void cancelRenderTarget(int width, int height)
    {
        GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, 0);
        GLES20.glBindRenderbuffer(GLES20.GL_RENDERBUFFER, 0);
        GLES20.glViewport(0, 0, width, height);
    }

    public HopRenderTarget()
    {
        int frameBuffer[] = new int[1];
        int renderedTexture[] = new int[1];
        int depthBuffer[] = new int[1];

        GLES20.glGenFramebuffers(1, frameBuffer, 0);
        GLES20.glGenTextures(1, renderedTexture, 0);
        GLES20.glGenRenderbuffers(1, depthBuffer, 0);

        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, renderedTexture[0]);

        GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
        GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);

        IntBuffer texBuffer = ByteBuffer.allocateDirect(RENDER_TARGET_WIDTH * RENDER_TARGET_HEIGHT * 4).order(ByteOrder.nativeOrder()).asIntBuffer();

        GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGBA, RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT, 0, GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE, texBuffer);
        GLES20.glBindRenderbuffer(GLES20.GL_RENDERBUFFER, depthBuffer[0]);
        GLES20.glRenderbufferStorage(GLES20.GL_RENDERBUFFER, GLES20.GL_DEPTH_COMPONENT16, RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT);

        mFrameBuffer = frameBuffer[0];
        mDepthBuffer = depthBuffer[0];
        mTexture = renderedTexture[0];

        GLES20.glEnable(GLES20.GL_DEPTH_TEST);
        GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, 0);
        GLES20.glBindRenderbuffer(GLES20.GL_RENDERBUFFER, 0);

        int status = GLES20.glCheckFramebufferStatus(GLES20.GL_FRAMEBUFFER);
        if(status != GLES20.GL_FRAMEBUFFER_COMPLETE) Log.e("Hop", "Failed to create render target");

        applyRenderTarget();
        GLES20.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);
        cancelRenderTarget(RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT);
    }
}
