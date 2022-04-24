#pragma once


namespace Display
{
    void Init();

    void Update();
}


struct Region
{
    int width;
    int height;

    Region(int w, int h) : width(w), height(h) { }

    void Fill(int x, int y);
};
