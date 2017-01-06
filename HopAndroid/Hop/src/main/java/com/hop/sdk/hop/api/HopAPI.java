package com.hop.sdk.hop.api;

public class HopAPI
{
    private static float mSize = 1;
    private static float mX = 0;
    private static float mY = 0;
    private static float mZ = 0;

    public static float getSize()
    {
        return mSize;
    }

    public static float getX()
    {
        return mX;
    }

    public static float getY()
    {
        return mY;
    }

    public static float getZ()
    {
        return mZ;
    }

    public static void setSize(float size)
    {
        mSize = size;
    }

    public static void setPosition(float x, float y, float z)
    {
        mX = x;
        mY = y;
        mZ = z;
    }
}
