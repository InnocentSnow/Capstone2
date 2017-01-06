package com.hop.sdk.hop.impl;

import android.content.Context;
import android.opengl.GLES20;
import android.util.Log;

import com.hop.sdk.hop.R;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

abstract class HopShaderMgr
{
    public static int program;

    public static void load(Context context)
    {
        program = createProgram(context, R.raw.hop_vs,	R.raw.hop_fs);
    }

    private static int createProgram(Context context, int vertexShaderId, int fragmentShaderId)
    {
        int linked[] = new int[1];
        int vertexShader = loadShader(context, GLES20.GL_VERTEX_SHADER, vertexShaderId);
        int fragmentShader = loadShader(context, GLES20.GL_FRAGMENT_SHADER, fragmentShaderId);

        if(vertexShader == 0 || fragmentShader == 0) return 0;

        int program = GLES20.glCreateProgram();
        if(program == 0) return 0;

        GLES20.glAttachShader(program, vertexShader);
        GLES20.glAttachShader(program, fragmentShader);
        GLES20.glLinkProgram(program);

        GLES20.glGetProgramiv(program, GLES20.GL_LINK_STATUS, linked, 0);

        if(linked[0] == 0)
        {
            Log.e("Hop", "Error linking program <" + Integer.toString(vertexShaderId) + ", " + Integer.toString(fragmentShaderId) + "> :");
            Log.e("Hop", GLES20.glGetProgramInfoLog(program));
            GLES20.glDeleteProgram(program);

            return 0;
        }

        return program;
    }

    private static int loadShader(Context context, int type, int shaderId)
    {
        String shaderSource = readShaderSource(context, shaderId);

        int shader = GLES20.glCreateShader(type);
        int compiled[] = new int[1];

        if(shader == 0) return 0;

        GLES20.glShaderSource(shader, shaderSource);
        GLES20.glCompileShader(shader);
        GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compiled, 0);

        if(compiled[0] == 0)
        {
            Log.e("Hop", "<" + Integer.toString(shaderId) + "> " + GLES20.glGetShaderInfoLog(shader));
            GLES20.glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    private static String readShaderSource(Context context, int shaderID)
    {
        StringBuilder shaderBuilder = new StringBuilder();

        try
        {
            InputStream inputStream = context.getResources().openRawResource(shaderID);
            BufferedReader in = new BufferedReader(new InputStreamReader(inputStream));

            String read = in.readLine();
            while (read != null)
            {
                shaderBuilder.append(read).append("\n");
                read = in.readLine();
            }

            shaderBuilder.deleteCharAt(shaderBuilder.length() - 1);
            inputStream.close();
        }
        catch (Exception e)
        {
            Log.e("Hop", "Could not read shader <" + Integer.toString(shaderID) + "> : " + e.getLocalizedMessage());
            e.printStackTrace();
        }

        return shaderBuilder.toString();
    }
}
