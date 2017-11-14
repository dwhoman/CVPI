import subprocess
import numpy as np
import hypothesis as h
import hypothesis.strategies as st
import hypothesis.extra.numpy as hnp

#h.settings(buffer_size = 819200000)

min_img_width = 1
min_img_height = 1
max_img_width = 10
max_img_height = 10
max_uint32 = 2**32 - 1
max_int32 = 2**31 - 1
min_int32 = -(2**31)
max_short = 2**15 - 1
min_short = -(2**15)

def thirty2to8s(np_num):
    return [int(i) for i in int(np_num).to_bytes(4, byteorder='big', signed=False)]

def twoDto3d(np_array):
    return np.array([[[z for z in thirty2to8s(y)] for y in x] for x in np_array], dtype=np.uint64)

def image_hex(np_array):
    return ''.join(["%02x" % (x) for x in np_array.flatten('C')])

@st.composite
def np_images(draw,
              number,
              width=st.integers(min_img_width, max_img_width).example(),
              height=st.integers(min_img_height, max_img_height).example()):
    return draw(st.lists(hnp.arrays(np.uint32, (width,height),
                                    elements=st.integers(0,max_uint32)),
                         min_size=number, max_size=number))

@h.given(np_images(2),
         st.integers(1, 5),
         st.integers(1, 5),
         st.floats(1.0, 1.0),
         st.floats(0, 0))
def test_add_images(images, a, b, scale, bias):
    assert len(images) == 2
    assert images[0].shape == images[1].shape
    
    image_1 = twoDto3d(images[0])
    image_2 = twoDto3d(images[1])
                
    image_sum = np.clip(np.ceil(scale * (a * image_1 + b * image_2) + bias), 0, 255)

    compl_proc = subprocess.check_output([
        "./cvpi_tests_hyp",
        "cvpi_image_add",
        image_hex(image_1),
        image_hex(image_2),
        str(images[0].shape[0]),
        str(images[0].shape[1]),
        str(a), str(b), format(scale, 'f'), format(bias, 'f')])

    compl_proc_str = ''.join(map(chr, compl_proc))
    numpy_image_str = image_hex(image_sum) + "\n"

    h.note(str(images[0].shape[0]) + " " + str(images[0].shape[1]))
    h.note(image_hex(image_1))
    h.note(image_hex(image_2))
    h.note("cvpi: " + compl_proc_str)
    h.note("numpy: " + numpy_image_str)

    assert numpy_image_str == compl_proc_str

if __name__ == '__main__':
    test_add_images()
