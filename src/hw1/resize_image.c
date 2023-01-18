#include <math.h>
#include "image.h"

float nn_interpolate(image im, int c, float h, float w)
{
    // TODO
    return get_pixel(im, c, round(h), round(w));
}

image nn_resize(image im, int h, int w)
{
    image newIm = make_image(im.c, h, w);
    float aw = (float) im.w / w;
    float bw = -0.5 + aw * 0.5;

    float ah = (float) im.h / h;
    float bh = -0.5 + ah * 0.5;

    for (int c = 0; c < im.c; c++) {
        for (int H = 0; H < h; H++) {
            for (int W = 0; W < w; W++) {
                set_pixel(newIm, c, H, W, nn_interpolate(im, c, H * ah + bh, W * aw + bw));
            }
        }
    }
    return newIm;
}

float bilinear_interpolate(image im, int c, float h, float w)
{
    // TODO
    float v1, v2, v3, v4, q1, q2, q;
    v1 = get_pixel(im, c, floor(h), floor(w));
    v2 = get_pixel(im, c, floor(h), ceil(w));
    v3 = get_pixel(im, c, ceil(h), floor(w));
    v4 = get_pixel(im, c, ceil(h), ceil(w));

    q1 = v2 * (w - floor(w)) + v1 * (ceil(w) - w);
    q2 = v3 * (ceil(w) - w) + v4 * (w - floor(w));
    q = q1 * (ceil(h) - h) + q2 * (h - floor(h));
    return q;
}

image bilinear_resize(image im, int h, int w)
{
    // TODO
    image newIm = make_image(im.c, h, w);
    float aw = (float) im.w / w;
    float bw = -0.5 + aw * 0.5;

    float ah = (float) im.h / h;
    float bh = -0.5 + ah * 0.5;

    for (int c = 0; c < im.c; c++) {
        for (int H = 0; H < h; H++) {
            for (int W = 0; W < w; W++) {
                set_pixel(newIm, c, H, W, bilinear_interpolate(im, c, H * ah + bh, W * aw + bw));
            }
        }
    }
    return newIm;
}

