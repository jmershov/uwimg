#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    // TODO
    for(int i = 0; i < im.w * im.h; i++) {
        im.data[i] /= (float) im.w * im.h;
    }
}

image make_box_filter(int w)
{
    // TODO
    image im = make_image(1,w,w);
    for (int i = 0; i < w*w; i++) {
        im.data[i] = 1;
    }
    l1_normalize(im);
    return im;
}

image convolve_image(image im, image filter, int preserve)
{
    // TODO
    assert(filter.c == 1 || filter.c == im.c);
    image newIm;
    float heightOffset = floor(filter.h / 2.0);
    float widthOffset = floor(filter.w / 2.0);
    if (filter.c == im.c) {
        if (preserve == 1) {
            newIm = make_image(im.c, im.h, im.w);
            for (int c = 0; c < im.c; c++) {
                for (int h = 0; h < im.h; h++) {
                    for (int w = 0; w < im.w; w++) {
                        float sum = 0;
                        for (int fh = 0; fh < filter.h; fh++) {
                            for (int fw = 0; fw < filter.w; fw++) {
                                if ((h - heightOffset + fh >= 0 && w - widthOffset + fw >= 0) &&
                                    (h - heightOffset + fh < im.h && w - widthOffset + fw < im.w)) {
                                        sum += get_pixel(filter, c ,fh, fw) * get_pixel(im, c, h-heightOffset+fh, w-widthOffset+fw);
                                } else if (!(h - heightOffset + fh >= 0 && h - heightOffset + fh < im.h) && 
                                    !(w - widthOffset + fw >= 0 && w - widthOffset + fw < im.w)){
                                        sum += get_pixel(filter, c ,fh, fw) * get_pixel(im, c, h, w);
                                } else if (!(h - heightOffset + fh >= 0 && h - heightOffset + fh < im.h)) {
                                    sum += get_pixel(filter, c ,fh, fw) * get_pixel(im, c, h, w - widthOffset + fw);
                                } else {
                                    sum += get_pixel(filter, c ,fh, fw) * get_pixel(im, c, h - heightOffset + fh, w);
                                }
                            }
                        }
                        set_pixel(newIm, c, h, w, sum);
                    }
                }
            }
        } else {
            newIm = make_image(1, im.h, im.w);
            for (int h = 0; h < im.h; h++) {
                for (int w = 0; w < im.w; w++) {
                    float sum = 0;
                    for (int c = 0; c < im.c; c++) {
                        for (int fh = 0; fh < filter.h; fh++) {
                            for (int fw = 0; fw < filter.w; fw++) {
                                if ((h - heightOffset + fh >= 0 && w - widthOffset + fw >= 0) &&
                                    (h - heightOffset + fh < im.h && w - widthOffset + fw < im.w)) {
                                        sum += get_pixel(filter, c ,fh, fw) * get_pixel(im, c, h-heightOffset+fh, w-widthOffset+fw);
                                } else if (!(h - heightOffset + fh >= 0 && h - heightOffset + fh < im.h) && 
                                    !(w - widthOffset + fw >= 0 && w - widthOffset + fw < im.w)){
                                        sum += get_pixel(filter, c ,fh, fw) * get_pixel(im, c, h, w);
                                } else if (!(h - heightOffset + fh >= 0 && h - heightOffset + fh < im.h)) {
                                    sum += get_pixel(filter, c ,fh, fw) * get_pixel(im, c, h, w - widthOffset + fw);
                                } else {
                                    sum += get_pixel(filter, c ,fh, fw) * get_pixel(im, c, h - heightOffset + fh, w);
                                }
                            }
                        }
                    }
                    set_pixel(newIm, 0, h, w, sum);
                }
            }
        }
    } else {
        image newFilter = make_image(im.c, filter.h, filter.w);
        for (int i = 0; i < im.c; i++) {
            for (int j = 0; j < filter.h; j++) {
                for (int k = 0; k < filter.w; k++) {
                    set_pixel(newFilter, i, j, k, get_pixel(filter, 0, j, k));
                }
            }
        }
        newIm = convolve_image(im, newFilter, preserve);
    }
    return newIm;
}

image make_highpass_filter()
{
    // TODO
    image filter = make_image(1, 3, 3);
    filter.data[1] = -1;
    filter.data[3] = -1;
    filter.data[4] = 4;
    filter.data[5] = -1;
    filter.data[7] = -1;
    return filter;
}

image make_sharpen_filter()
{
    // TODO
    image filter = make_image(1, 3, 3);
    filter.data[1] = -1;
    filter.data[3] = -1;
    filter.data[4] = 5;
    filter.data[5] = -1;
    filter.data[7] = -1;
    return filter;
}

image make_emboss_filter()
{
    // TODO
    image filter = make_image(1, 3, 3);
    filter.data[0] = -2;
    filter.data[1] = -1;
    filter.data[2] = 0;
    filter.data[3] = -1;
    filter.data[4] = 1;
    filter.data[5] = 1;
    filter.data[6] = 0;
    filter.data[7] = 1;
    filter.data[8] = 2;
    return filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: We should use preserve on the ones where the sum of all of the numbers in the filter sums up to 1. These would be for the
// emboss and sharpen filters.

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: Yes. We have to clamp the image, since there may be cases where the image data ends up with values greater than one, which we
// would have to fix.

image make_gaussian_filter(float sigma)
{
    // TODO
    return make_image(1,1,1);
}

image add_image(image a, image b)
{
    // TODO
    return make_image(1,1,1);
}

image sub_image(image a, image b)
{
    // TODO
    return make_image(1,1,1);
}

image make_gx_filter()
{
    // TODO
    return make_image(1,1,1);
}

image make_gy_filter()
{
    // TODO
    return make_image(1,1,1);
}

void feature_normalize(image im)
{
    // TODO
}

image *sobel_image(image im)
{
    // TODO
    return calloc(2, sizeof(image));
}

image colorize_sobel(image im)
{
    // TODO
    return make_image(1,1,1);
}
