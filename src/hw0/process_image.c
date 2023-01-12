#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int c, int h, int w)
{
    // Bounds Checking
    if (c >= im.c) {
        c = im.c - 1;
    } else if (c < 0) {
        c = 0;
    }
    
    if (h >= im.h) {
        h = im.h - 1;
    } else if (h < 0) {
        h = 0;
    }
    
    if (w >= im.w) {
        w = im.w - 1;
    } else if (w < 0) {
        w = 0;
    }

    return im.data[w + h * im.w + c * im.w * im.h];
}

void set_pixel(image im, int c, int h, int w, float v)
{
    // Bounds checking
    if ((c >= im.c || c < 0) || (h >= im.h || h < 0) || (w >= im.w || w < 0)) {
        return;
    }

    im.data[w + h * im.w + c * im.w * im.h] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.c, im.h, im.w);
    for (int i = 0; i < im.w * im.h * im.c; i++) {
        copy.data[i] = im.data[i];
    }
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(1, im.h, im.w);
    // TODO Fill this in
    for (int h = 0; h < im.h; h++) {
        for (int w = 0; w < im.w; w++) {
            float pixelR = get_pixel(im, 0, h, w);
            float pixelG = get_pixel(im, 1, h, w);
            float pixelB = get_pixel(im, 2, h, w);
            float pixNew = pixelR * 0.299 + pixelG * 0.587 + pixelB * .114;
            set_pixel(gray, 0, h, w, pixNew);
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    for (int h = 0; h < im.h; h++) {
        for (int w = 0; w < im.w; w++) {
            float pixel = get_pixel(im, c, h, w);
            pixel += v;
            set_pixel(im, c, h, w, pixel);
        }
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    for (int i = 0; i < im.h * im.w * im.c; i++) {
        float curr = im.data[i];
        if (curr > 1) {
            im.data[i] = 1;
        }

        if (curr < 0) {
            im.data[i] = 0;
        }
    }
}

// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    for (int h = 0; h < im.h; h++) {
        for (int w = 0; w < im.w; w++) {
            // Get RGB pixel values
            float pixR = get_pixel(im, 0, h, w);
            float pixG = get_pixel(im, 1, h, w);
            float pixB = get_pixel(im, 2, h, w);

            // Get value
            float val = three_way_max(pixR, pixG, pixB);
            float min = three_way_min(pixR, pixB, pixG);

            // Calculate saturation
            float range = val - min;
            float sat = 0;
            if (pixR != 0 || pixG != 0 || pixB != 0) {
                sat = range / val;
            }

            // Calculate hue
            float temp = 0;
            if (range == 0) {
                temp = 0;
            } else if (val == pixR) {
                temp = (pixG - pixB) / range;
            } else if (val == pixG) {
                temp = (pixB - pixR) / range + 2;
            } else if (val == pixB) {
                temp = (pixR - pixG) / range + 4;
            }

            float hue;
            if (temp < 0) {
                hue = temp / 6 + 1;
            } else {
                hue = temp / 6;
            }

            // Modify Image
            set_pixel(im, 0, h, w, hue);
            set_pixel(im, 1, h, w, sat);
            set_pixel(im, 2, h, w, val);
        }
    }
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    for (int h = 0; h < im.h; h++) {
        for (int w = 0; w < im.w; w++) {
            float pixH = get_pixel(im, 0, h, w);
            float pixS = get_pixel(im, 1, h, w);
            float pixV = get_pixel(im, 2, h, w);

            float red = 0;
            float green = 0;
            float blue = 0; 

            float range = pixS * pixV;
            float min = pixV - range;

            float hue = pixH * 6;
            if (hue < 1) {
                red = pixV;
                blue = min;
                green = blue + range * hue;
            } else if (hue < 2) {
                green = pixV;
                blue = min;
                red = blue + 2 * range - hue * range;
            } else if (hue < 3) {
                green = pixV;
                red = min;
                blue = red + hue * range - 2 * range;
            } else if (hue < 4) {
                blue = pixV;
                red = min;
                green = red + 4 * range - hue * range;
            } else if (hue < 5) {
                blue = pixV;
                green = min;
                red = green + hue * range - 4 * range;
            } else {
                hue -= 6;
                red = pixV;
                green = min;
                blue = green - hue * range;
            }

            set_pixel(im, 0, h, w, red);
            set_pixel(im, 1, h, w, green);
            set_pixel(im, 2, h, w, blue);
        }
    }
}
