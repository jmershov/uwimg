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
    float sum = 0;
    for(int i = 0; i < im.w * im.h; i++) {
        sum += im.data[i];
    }

    for (int i = 0; i < im.w * im.h; i++) {
        im.data[i] /= sum;
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
                                sum += get_pixel(filter, c ,fh, fw) * get_pixel(im, c, h-heightOffset+fh, w-widthOffset+fw);
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
                                sum += get_pixel(filter, c ,fh, fw) * get_pixel(im, c, h-heightOffset+fh, w-widthOffset+fw);
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
    filter.data[3] = -1;
    filter.data[4] = 1;
    filter.data[5] = 1;
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
    int size = ceil(6*sigma);
    if (size % 2 == 0) {
        size++;
    }

    int half = floor(size/2);

    image filter = make_image(1, size, size);
    for (float h = 0; h < size; h++) {
        for (float w = 0; w < size; w++) {
            float value = 1/(TWOPI*sigma*sigma) * exp(-((w-half)*(w-half)+(h-half)*(h-half))/(2*sigma*sigma));
            set_pixel(filter, 0, h, w, value);
        }
    }
    l1_normalize(filter);
    return filter;
}

image add_image(image a, image b)
{
    // TODO
    assert(a.c == b.c && a.w == b.w && a.h == b.h);

    image result = make_image(a.c, a.h, a.w);
    for (int i = 0; i < a.c * a.h * a.w; i++) {
        result.data[i] = a.data[i] + b.data[i];
    }

    return result;
}

image sub_image(image a, image b)
{
    // TODO
    assert(a.c == b.c && a.w == b.w && a.h == b.h);

    image result = make_image(a.c, a.h, a.w);
    for (int i = 0; i < a.c * a.h * a.w; i++) {
        result.data[i] = a.data[i] - b.data[i];
    }
    
    return result;
}

image make_gx_filter()
{
    // TODO
    image filter = make_image(1, 3, 3);
    filter.data[0] = -1;
    filter.data[2] = 1;
    filter.data[3] = -2;
    filter.data[5] = 2;
    filter.data[6] = -1;
    filter.data[8] = 1;
    return filter;
}

image make_gy_filter()
{
    // TODO
    image filter = make_image(1, 3, 3);
    filter.data[0] = -1;
    filter.data[1] = -2;
    filter.data[2] = -1;
    filter.data[6] = 1;
    filter.data[7] = 2;
    filter.data[8] = 1;
    return filter;
}

void feature_normalize(image im)
{
    // TODO
    float max = im.data[0];
    float min = im.data[0];
    float range;
    for (int i = 0; i < im.c * im.w * im.h; i++) {
        float curr = im.data[i];
        if (curr < min) {
            min = curr;
        }

        if (curr > max) {
            max = curr;
        }
    }
    range = max - min;
    for (int i = 0; i < im.c * im.w * im.h; i++) {
        im.data[i] = range == 0 ? 0 : (im.data[i] - min)/range;
    }
}

image *sobel_image(image im)
{
    // TODO
    image* imgs = calloc(2, sizeof(image));
    image gxf = make_gx_filter();
    image gyf = make_gy_filter();
    image gx = convolve_image(im, gxf, 0);
    image gy = convolve_image(im, gyf, 0);

    imgs[0] = make_image(1, im.h, im.w);
    imgs[1] = make_image(1, im.h, im.w);

    for (int i = 0; i < im.h * im.w; i++) {
        imgs[0].data[i] = sqrt(pow(gx.data[i], 2) + pow(gy.data[i], 2));
        imgs[1].data[i] = atan2(gy.data[i], gx.data[i]);
    }
    return imgs;
}

image colorize_sobel(image im)
{
    // TODO
    image filter = make_gaussian_filter(3);
    im = convolve_image(im, filter, 1);
    image* imgs = sobel_image(im);
    image mag = imgs[0];
    image theta = imgs[1];
    feature_normalize(mag);
    feature_normalize(theta);

    image result = make_image(3, im.h, im.w);
    for (int h = 0; h < im.h; h++) {
        for (int w = 0; w < im.w; w++) {
            set_pixel(result, 2, h, w, get_pixel(mag, 0, h, w));
            set_pixel(result, 1, h, w, get_pixel(mag, 0, h, w));
            set_pixel(result, 0, h, w, get_pixel(theta, 0, h, w)); 
        }
    }
    hsv_to_rgb(result);
    clamp_image(result);
    return result;
}
