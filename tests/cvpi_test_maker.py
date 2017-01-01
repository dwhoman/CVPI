import numpy as np

def scale(value, old_min, old_max, new_min, new_max):
    """Scale a `value' existing within the range `old_min' and `old_max'
       to the range new_min and new_max.
    """
    if new_min > new_max:
        new_min, new_max = new_max, new_min

    if old_min > old_max:
        old_min, old_max = old_max, old_min

    return ((value - old_min) * (new_max - new_min)) / (old_max - old_min) + new_min

v_scale = np.vectorize(scale)

def scale_1_to_255(value):
    """Scale `value' existing within the range 0 to 1, to the range 0 to
       255. The result is then floored.
    """
    return int(scale(value, 0.0, 1.0, 0.0, 255.0))

v_scale_1_to_255 = np.vectorize(scale_1_to_255)

def scale_255_to_1(value):
    """Scale `value' existing within the range 0 to 255, to the range 0 to
       1.
    """
    return scale(value, 0.0, 255.0, 0.0, 1.0)

v_scale_255_to_1 = np.vectorize(scale_255_to_1)

def saturate(value, minimum, maximum):
    """Clamp `value' between `minimum' and `maximum', returning a new value."""
    if value < minimum:
        return minimum
    elif value > maximum:
        return maximum
    else:
        return value
v_saturate = np.vectorize(saturate)

def saturate_1(value):
    """Clamp `value' between 0 and 1."""
    return float(saturate(value, 0, 1))

v_saturate_1 = np.vectorize(saturate_1)

def saturate_255(value):
    """Clamp `value' between 0 and 255."""
    return int(saturate(value, 0, 255))

v_saturate_255 = np.vectorize(saturate_255)

def vgColorMatrix(rgba_image, multiplicand_matrix, bias_array):
    """Numpy implementation of OpenVG's vgColorMatrix.  `rgba_image' is an
       (n,m,4) dimensional array of values ranging from 0 to 255. The
       `multiplicand_matrix' is a 4x4 matrix of floats, and the
       `bias_array' is a length 4 array of floats.  The values of
       rgba_image get scaled from 0 to 1.  The vgColorMatrix operation
       gets applied.  The result is clamped between 0 and 1, and is
       then scaled from 0 to 255.
    """
    output = np.empty_like(rgba_image)
    rows, columns, p = rgba_image.shape
    i = 0
    while i < rows:
        j = 0
        while j < columns:
            pixel = rgba_image[i,j]
            pixel_scaled = v_scale_255_to_1(pixel)
            output[i,j] = v_scale_1_to_255(v_saturate_1(multiplicand_matrix * pixel_scaled + bias_array))
            j += 1
        i += 1
    return output

def vgConvolve(rgba_image, kernel, shift_x, shift_y, scale, bias, tiling_mode=np.array[0.,0.,0.,0.]):
    """Numpy implementation of OpenVG's vgConvolve.  `rgba_image' is an
       (n,m,4) dimensional array of values ranging from 0 to 255.
       `tiling_mode' can be a number between 0 and 1 used as a fill
       value, or one of three strings: "pad", "repeat", "reflect".
       See the OpenVG 1.1 standard 9.4.1 Pattern Tiling.
    """
    output = np.empty_like(rgba_image)
    rows, columns, p = rgba_image.shape
    # vgConvolve indexes images from the lower left and is row
    # major. The kernel is indexed from the upper right and is column
    # major. Flip and transpose the image so that it uses numpy's
    # ordering.
    rgba_linear_alg = np.transpose(np.flipud(rgba_image), (1,0,2))

    k_height, k_width = kernel.shape
    
    y = 0
    while y < rows:
        x = 0
        while x < columns:
            weighted_sum, i = 0, 0
            while i < k_width:
                j = 0
                while j < k_height:
                    pixel_row = y + j - shift_y
                    pixel_column = x + i - shift_x
                    pixel = None
                    if 0 <= pixel_row < rows and 0 <= pixel_column < columns:
                        pixel = v_scale_255_to_1(rgba_linear_alg[pixel_row, pixel_column])
                    else:
                        if isinstance(tiling_mode,str):
                            if tiling_mode == "pad":
                                pixel = v_scale_255_to_1(rgba_linear_alg[max(0, min(y, rows - 1)), max(0, min(x, columns - 1))])
                            elif tiling_mode == "repeat":
                                pixel = v_scale_255_to_1(rgba_linear_alg[y % rows, x % columns])
                            elif tiling_mode == "reflect":
                                y_p = y % rows if int(y/rows) % 2 == 0 else rows - 1 - y % rows
                                x_p = x % columns if int(x/columns) % 2 == 0 else columns - 1 - x % columns
                                pixel = v_scale_255_to_1(rgba_linear_alg[y_p, x_p])
                            else:
                                # default to filling with zeros
                                pixel = np.array[0.,0.,0.,0.]
                        else:   # tiling mode: fill
                            if isinstance(tiling_mode, np.ndarray):
                                pixel = v_saturate_1(tiling_mode)
                            else:
                                pixel = np.array[0.,0.,0.,0.]
                    weighted_sum += kernel[k_height - i - 1, k_width - j - 1] * pixel
                    j += 1
                i += 1
            
            output[y,x] = v_scale_1_to_255(v_saturate_1(weighted_sum * scale + bias))
            x += 1
        y += 1
    return np.flipud(np.transpose(output))

def vgSeparableConvolve(rgba_image, kernel_x, kernel_y, shift_x, shift_y, scale, bias, tiling_mode):
    pass
