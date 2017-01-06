package com.hop.sdk.hop.impl;

import android.opengl.GLES20;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

class HopPyramid
{
    private FloatBuffer mPositionBuffer;
    private ShortBuffer mIndexBuffer;
    private FloatBuffer mTexCoordBuffer;

    public HopPyramid()
    {
        float position[] = {
                -1, 1, -1, -1, 0, 0,
                1, -1, 1, 1, 0, 0,
                -1, -1, 1, -1, 0, 0,
                1, 1, -1, 1, 0, 0
    };

        float texCoord[] = {
                1, 0, 0, 0, 0.5f, 1,
                1, 0, 0, 0, 0.5f, 1,
                1, 0, 0, 0, 0.5f, 1,
                1, 0, 0, 0, 0.5f, 1,
        };

        short indices[]	= {
                0, 1, 2,
                3, 4, 5,
                6, 7, 8,
                9, 10, 11
        };

        mPositionBuffer = ByteBuffer.allocateDirect(4 * position.length).order(ByteOrder.nativeOrder()).asFloatBuffer();
        mPositionBuffer.put(position);
        mPositionBuffer.position(0);

        mTexCoordBuffer = ByteBuffer.allocateDirect(4 * texCoord.length).order(ByteOrder.nativeOrder()).asFloatBuffer();
        mTexCoordBuffer.put(texCoord);
        mTexCoordBuffer.position(0);

        mIndexBuffer = ByteBuffer.allocateDirect(2 * indices.length).order(ByteOrder.nativeOrder()).asShortBuffer();
        mIndexBuffer.put(indices);
        mIndexBuffer.position(0);
    }

    public void draw(HopRenderTarget renderTarget[])
    {
        int oldFace[] = new int[1];
        int oldCullMode[] = new int[1];
        GLES20.glGetIntegerv(GLES20.GL_FRONT_FACE, oldFace, 0);
        GLES20.glGetIntegerv(GLES20.GL_CULL_FACE_MODE, oldCullMode, 0);

        GLES20.glFrontFace(GLES20.GL_CCW);
        GLES20.glCullFace(GLES20.GL_BACK);
        GLES20.glUseProgram(HopShaderMgr.program);

        int a_position = GLES20.glGetAttribLocation(HopShaderMgr.program, "a_position");
        int a_texCoord = GLES20.glGetAttribLocation(HopShaderMgr.program, "a_texCoord");
        int u_texture = GLES20.glGetUniformLocation(HopShaderMgr.program, "u_texture");

        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        GLES20.glUniform1i(u_texture, 0);

        mPositionBuffer.position(0);
        GLES20.glVertexAttribPointer(a_position, 2, GLES20.GL_FLOAT, false, 8, mPositionBuffer);
        GLES20.glEnableVertexAttribArray(a_position);

        mTexCoordBuffer.position(0);
        GLES20.glVertexAttribPointer(a_texCoord, 2, GLES20.GL_FLOAT, false, 8, mTexCoordBuffer);
        GLES20.glEnableVertexAttribArray(a_texCoord);

        for (int i = 0; i < 4; ++i)
        {
            renderTarget[i].applyTexture();
            mIndexBuffer.position(3 * i);
            GLES20.glDrawElements(GLES20.GL_TRIANGLES, 3, GLES20.GL_UNSIGNED_SHORT, mIndexBuffer);
        }

        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);

        GLES20.glFrontFace(oldFace[0]);
        GLES20.glCullFace(oldCullMode[0]);
    }
}

