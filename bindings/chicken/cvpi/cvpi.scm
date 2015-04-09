;; This file is part of CVPI.
;; 
;; Copyright (C) 2015
;; 
;; This program is free software: you can redistribute it and/or
;; modify it under the terms of the GNU Lesser General Public License
;; as published by the Free Software Foundation, either version 3 of
;; the License, or (at your option) any later version.
;; 
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU Lesser General Public License for more details.
;; 
;; You should have received a copy of the GNU Lesser General Public License
;; along with this program.  If not, see <http://www.gnu.org/licenses/>.

#>
#if HAVE_BCM_HOST == 1
int have_bcm_host = 1
#else
int have_bcm_host = 0
#endif
<#

(define-foreign-variable HAVE_BCM_HOST int "have_bcm_host")
(define-syntax macro-if
  (ir-macro-transformer
   (lambda (form inject compare)
       (if (cadr form)
	   `(begin ,(caddr form))
	   `(begin ,@(cdddr form))))))

;;; ffi for cvpi
(foreign-declare "#include <VG/openvg.h>")
(foreign-declare "#include <cvpi.h>")
;; (foreign-declare "#include <cvpi_base.h>") 
;; (foreign-declare "#include <cvpi_egl_config.h>") 
;; (foreign-declare "#include <cvpi_image_functions.h>")
;; (foreign-declare "#include <cvpi_image_headers.h>")
;; (foreign-declare "#include <cvpi_vg_ext.h>")
;;; cvpi_base.h
(define-foreign-type EGLint int)

(define-foreign-type CVPI_BOOL int)
(define CVPI_TRUE_TEST (foreign-lambda int "CVPI_TRUE_TEST" CVPI_BOOL))
(define CVPI_FALSE_TEST (foreign-lambda int "CVPI_FALSE_TEST" CVPI_BOOL))
(define-foreign-variable CVPI_TRUE (const CVPI_BOOL) "CVPI_TRUE")
(define-foreign-variable CVPI_FALSE (const CVPI_BOOL) "CVPI_FALSE")
(define CVPI_NOT (foreign-lambda CVPI_BOOL "CVPI_NOT" CVPI_BOOL))

;;; cvpi_egl_config.h
(define-foreign-variable EGL_CONFIG_MAX_WIDTH (const int) "EGL_CONFIG_MAX_WIDTH")
(define-foreign-variable EGL_CONFIG_MAX_HEIGHT (const int) "EGL_CONFIG_MAX_HEIGHT")

(conditional-code (eq? 1 HAVE_BCM_HOST)
    (define-foreign-variable CVPI_EGL_DEFAULT_PIXEL_FORMAT (const int) "CVPI_EGL_DEFAULT_PIXEL_FORMAT")
    (define-foreign-enum-type (cvpi_egl_pixel_format int cvpi_egl_ARGB_8888)
      (cvpi_egl_pixel_format->int int->cvpi_egl_pixel_format)
      cvpi_egl_ARGB_8888_PRE
      cvpi_egl_ARGB_8888    
      cvpi_egl_XRGB_8888    
      cvpi_egl_RGB_565      
      cvpi_egl_A_8)
      
    (define-foreign-enum-type (cvpi_egl_pixel_format_brcm int cvpi_egl_pixel_format_brcm_none)
      (cvpi_egl_pixel_format_brcm->int int->cvpi_egl_pixel_format_brcm)
      cvpi_egl_pixel_format_gles2_texture_brcm
      cvpi_egl_pixel_format_gles_texture_brcm
      cvpi_egl_pixel_format_render_gles2_brcm
      cvpi_egl_pixel_format_render_gles_brcm
      cvpi_egl_pixel_format_render_gl_brcm
      cvpi_egl_pixel_format_render_mask_brcm
      cvpi_egl_pixel_format_render_vg_brcm  
      cvpi_egl_pixel_format_texture_mask_brcm
      cvpi_egl_pixel_format_usage_mask_brcm  
      cvpi_egl_pixel_format_vg_image_brcm    
      cvpi_egl_pixel_format_brcm_none))

(define-foreign-enum-type (cvpi_egl_surface_bits int cvpi_egl_surface_bits_dc)
  (cvpi_egl_surface_bits->int int->cvpi_egl_surface_bits)
  cvpi_egl_surface_bits_pbuffer
  cvpi_egl_surface_bits_pixmap 
  cvpi_egl_surface_bits_window 
  cvpi_egl_surface_bits_colorspace
  cvpi_egl_surface_bits_alpha 
  cvpi_egl_surface_bits_multisample
  cvpi_egl_surface_bits_swap
  cvpi_egl_surface_bits_dc)

	 (define-foreign-enum-type (cvpi_egl_renderable_bits int cvpi_egl_renderable_bits_openvg)
	   (cvpi_egl_renderable_bits->int int->cvpi_egl_renderable_bits)
	   cvpi_egl_renderable_bits_opengl_es
	   cvpi_egl_renderable_bits_openvg
	   cvpi_egl_renderable_bits_opengl_es2
	   cvpi_egl_renderable_bits_opengl)

(define-foreign-enum-type (cvpi_egl_transparent_type int cvpi_egl_transparent_type_none)
  (cvpi_egl_transparent_type->int int->cvpi_egl_transparent_type)
  cvpi_egl_transparent_type_dc
  cvpi_egl_transparent_type_none
  cvpi_egl_transparent_type_rgb)

(define-foreign-enum-type (cvpi_egl_color_buffer_type int cvpi_egl_color_buffer_type_rgb)
  (cvpi_egl_color_buffer_type->int int->cvpi_egl_color_buffer_type)
  cvpi_egl_color_buffer_type_dc
  cvpi_egl_color_buffer_type_rgb
  cvpi_egl_color_buffer_type_luminance)

(define-foreign-enum-type (cvpi_egl_config_caveat int cvpi_egl_config_caveat_none)
  (cvpi_egl_config_caveat->int int->cvpi_egl_config_caveat)
  cvpi_egl_config_caveat_dc
  cvpi_egl_config_caveat_none
  cvpi_egl_config_caveat_slow
  cvpi_egl_config_caveat_nonconformant)

(define-foreign-enum-type (cvpi_egl_renderable_api int cvpi_egl_renderable_api_openvg)
  (cvpi_egl_renderable_api->int int->cvpi_egl_renderable_api)
  cvpi_egl_renderable_api_openvg
  cvpi_egl_renderable_api_opengl
  cvpi_egl_renderable_api_current)

(define-foreign-enum-type (cvpi_egl_conformant_bits int cvpi_egl_conformant_bit_openvg)
  (cvpi_egl_conformant_bits->int int->cvpi_egl_conformant_bits)
  cvpi_egl_conformant_bit_dc
  cvpi_egl_conformant_bit_opengl
  cvpi_egl_conformant_bit_opengl_es
  cvpi_egl_conformant_bit_opengl_es2
  cvpi_egl_conformant_bit_openvg)

(define-foreign-enum-type (CVPI_EGLBoolean int CVPI_EGL_DONT_CARE)
  (CVPI_EGLBoolean->int int->CVPI_EGLBoolean)
  CVPI_EGL_TRUE
  CVPI_EGL_FALSE
  CVPI_EGL_DONT_CARE)

(define-foreign-enum-type (cvpi_egl_current_surface_type int cvpi_egl_surface_type_none)
  (cvpi_egl_current_surface_type->int int->cvpi_egl_current_surface_type)
  cvpi_egl_surface_type_none
  cvpi_egl_surface_type_pixmap
  cvpi_egl_surface_type_pbuffer
  cvpi_egl_surface_type_window)

(define-foreign-enum-type (cvpi_egl_samples int cvpi_egl_samples_zero)
  (cvpi_egl_samples->int int->cvpi_egl_samples)
  cvpi_egl_samples_zero
  cvpi_egl_samples_one)


(define-foreign-type cvpi_egl_instance (c-pointer (struct "cvpi_egl_instance_s")))
(define-foreign-type cvpi_egl_settings (c-pointer (struct "cvpi_egl_settings_s")))

(define cvpi_egl_settings_create (foreign-lambda cvpi_egl_settings "cvpi_egl_settings_create"))
(define cvpi_egl_instance_setup (foreign-lambda cvpi_egl_instance "cvpi_egl_instance_setup" cvpi_egl_settings))
(define cvpi_egl_instance_takedown (foreign-lambda void "cvpi_egl_instance_takedown" cvpi_egl_instance))

(define-foreign-enum-type (cvpi_egl_settings_change int cvpi_egl_settings_add)
  (cvpi_egl_settings_change->int int->cvpi_egl_settings_change)
  cvpi_egl_settings_add cvpi_egl_settings_remove)

(define cvpi_egl_settings_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_check" cvpi_egl_settings))
(define cvpi_egl_settings_width (foreign-lambda CVPI_BOOL "cvpi_egl_settings_width" cvpi_egl_settings unsigned-long))
(define cvpi_egl_settings_width_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_width_check" unsigned-long))
(define cvpi_egl_settings_height (foreign-lambda CVPI_BOOL "cvpi_egl_settings_height" cvpi_egl_settings unsigned-long))
(define cvpi_egl_settings_height_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_height_check" unsigned-long))
;; cpp-if
(conditional-code (eq? 1 HAVE_BCM_HOST)
		  (define cvpi_egl_settings_pixel_format_brcm_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_pixel_format_brcm_check" cvpi_egl_settings))
		  (define cvpi_egl_settings_pixel_format_brcm 
		    (lambda (settings pixel_format_brcm change)
		      (let ((pf (int->cvpi_egl_pixel_format_brcm pixel_format_brcm pixel_format_brcm))
			    (c (int->cvpi_egl_settings_change change)))
			((foreign-lambda CVPI_BOOL "cvpi_egl_settings_pixel_format_brcm"
					 cvpi_egl_settings cvpi_egl_pixel_format_brcm cvpi_egl_settings_change)
			 settings pf c))))
		  (define cvpi_egl_bytes_per_pixel
		    (lambda (format)
		      (let ((f (int->cvpi_egl_pixel_format format)))
			((foreign-lambda EGLint "cvpi_egl_bytes_per_pixel" cvpi_egl_pixel_format) f))))
		  (define cvpi_egl_settings_renderable_type_check
		    (foreign-lambda CVPI_BOOL "cvpi_egl_settings_renderable_type_check" cvpi_egl_settings)))
(define cvpi_egl_surface_functions_check (foreign-lambda CVPI_BOOL "cvpi_egl_surface_functions_check" cvpi_egl_settings))
(define cvpi_egl_settings_buffer_size_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_buffer_size_check" EGLint))
(define cvpi_egl_settings_buffer_size (foreign-lambda CVPI_BOOL "cvpi_egl_settings_buffer_size" cvpi_egl_settings EGLint))
(define cvpi_egl_settings_red_size_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_red_size_check" EGLint))
(define cvpi_egl_settings_red_size (foreign-lambda CVPI_BOOL "cvpi_egl_settings_red_size" cvpi_egl_settings EGLint))
(define cvpi_egl_settings_green_size_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_green_size_check" EGLint))
(define cvpi_egl_settings_green_size (foreign-lambda CVPI_BOOL "cvpi_egl_settings_green_size" cvpi_egl_settings EGLint))
(define cvpi_egl_settings_blue_size_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_blue_size_check" EGLint))
(define cvpi_egl_settings_blue_size (foreign-lambda CVPI_BOOL "cvpi_egl_settings_blue_size" cvpi_egl_settings EGLint))
(define cvpi_egl_settings_alpha_size_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_alpha_size_check" EGLint))
(define cvpi_egl_settings_alpha_size (foreign-lambda CVPI_BOOL "cvpi_egl_settings_alpha_size" cvpi_egl_settings EGLint))
(define cvpi_egl_settings_alpha_mask_size_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_alpha_mask_size_check" EGLint))
(define cvpi_egl_settings_alpha_mask_size (foreign-lambda CVPI_BOOL "cvpi_egl_settings_alpha_mask_size" cvpi_egl_settings EGLint))
(define cvpi_egl_settings_config_id_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_config_id_check" EGLint))
(define cvpi_egl_settings_config_id (foreign-lambda CVPI_BOOL "cvpi_egl_settings_config_id" cvpi_egl_settings EGLint))
(define cvpi_egl_settings_conformant_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_conformant_check" cvpi_egl_settings))
(define cvpi_egl_settings_conformant
  (lambda (settings bit change)
    (let ((b (int->cvpi_egl_conformant_bits bit)) (c (int->cvpi_egl_settings_change change)))
      ((foreign-lambda CVPI_BOOL "cvpi_egl_settings_conformant" cvpi_egl_settings cvpi_egl_conformant_bits cvpi_egl_settings_change)
       settings b c))))
(define cvpi_egl_settings_depth_size_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_depth_size_check" EGLint))
(define cvpi_egl_settings_depth_size (foreign-lambda CVPI_BOOL "cvpi_egl_settings_depth_size" cvpi_egl_settings EGLint))
(define cvpi_egl_settings_max_pbuffer_width_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_max_pbuffer_width_check" EGLint))
(define cvpi_egl_settings_max_pbuffer_height_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_max_pbuffer_height_check" EGLint))
(define cvpi_egl_settings_max_pbuffer_pixels_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_max_pbuffer_pixels_check" EGLint))
(define cvpi_egl_settings_match_native_pixmap_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_match_native_pixmap_check" EGLint))
(define cvpi_egl_settings_max_swap_interval_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_max_swap_interval_check" EGLint))
(define cvpi_egl_settings_max_swap_interval (foreign-lambda CVPI_BOOL "cvpi_egl_settings_max_swap_interval" cvpi_egl_settings EGLint))
(define cvpi_egl_settings_min_swap_interval_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_min_swap_interval_check" EGLint))
(define cvpi_egl_settings_min_swap_interval (foreign-lambda CVPI_BOOL "cvpi_egl_settings_min_swap_interval" cvpi_egl_settings EGLint))
(define cvpi_egl_settings_renderable_type 
  (lambda (settings bit change)
    (let ((b (int->cvpi_egl_renderable_bits bit))
	  (c (int->cvpi_egl_settings_change change)))
      ((foreign-lambda CVPI_BOOL "cvpi_egl_settings_renderable_type"
		       cvpi_egl_settings cvpi_egl_renderable_bits cvpi_egl_settings_change)
       settings b c))))
(define cvpi_egl_settings_stencil_size_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_stencil_size_check" EGLint))
(define cvpi_egl_settings_stencil_size (foreign-lambda CVPI_BOOL "cvpi_egl_settings_stencil_size" cvpi_egl_settings EGLint))
(define cvpi_egl_settings_surface_type_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_surface_type_check" cvpi_egl_settings))
(define cvpi_egl_settings_surface_type 
  (lambda (settings bit change)
    (let ((b (int->cvpi_egl_surface_bits bit))
	  (c (int->cvpi_egl_settings_change change)))
      ((foreign-lambda CVPI_BOOL "cvpi_egl_settings_surface_type" 
		       cvpi_egl_settings cvpi_egl_surface_bits cvpi_egl_settings_change)
       settings b c))))
(define cvpi_egl_settings_transparent_red_value_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_transparent_red_value_check" EGLint))
(define cvpi_egl_settings_transparent_red_value (foreign-lambda CVPI_BOOL "cvpi_egl_settings_transparent_red_value" cvpi_egl_settings EGLint))
(define cvpi_egl_settings_transparent_green_value_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_transparent_green_value_check" EGLint))
(define cvpi_egl_settings_transparent_green_value (foreign-lambda CVPI_BOOL "cvpi_egl_settings_transparent_green_value" cvpi_egl_settings EGLint))
(define cvpi_egl_settings_transparent_blue_value_check (foreign-lambda CVPI_BOOL "cvpi_egl_settings_transparent_blue_value_check" EGLint))
(define cvpi_egl_settings_transparent_blue_value (foreign-lambda CVPI_BOOL "cvpi_egl_settings_transparent_blue_value" cvpi_egl_settings EGLint))
(define cvpi_egl_error_string (foreign-lambda c-string "cvpi_egl_error_string" EGLint))

;;; cvpi_image_headers.h
(define-foreign-type cvpi_dword unsigned-integer32)
(define-foreign-type cvpi_word unsigned-short)
(define-foreign-type cvpi_long integer32)

(define-foreign-enum-type (cvpi_bmp_pixel_format int cvpi_bmp_sARGB_8888)
  (cvpi_bmp_pixel_format->int int->cvpi_bmp_pixel_format)
  cvpi_bmp_sRGB_565
  cvpi_bmp_sBGR_565
  cvpi_bmp_sXRGB_8888
  cvpi_bmp_sXBGR_8888
  cvpi_bmp_sARGB_8888
  cvpi_bmp_sBGRA_8888
  cvpi_bmp_sRGBA_8888)

(define cvpi_bmp_header_write
  (lambda (file-handle width height ppm pixel-format)
    (let ((pf (int->cvpi_bmp_pixel_format pixel-format))) 
      ((foreign-lambda void "cvpi_bmp_header_write"
		       c-pointer cvpi_long cvpi_long cvpi_dword cvpi_bmp_pixel_format)
       file-handle width height ppm pf))))

(define cvpi_pbm_header_write (foreign-lambda void "cvpi_pbm_header_write" c-pointer unsigned-long unsigned-long))
(define cvpi_pgm_header_write (foreign-lambda void "cvpi_pgm_header_write" c-pointer unsigned-long unsigned-long unsigned-char))

;;; cvpi_vg_ext.h
(define vgConvolveNormal
  (lambda (dst src kernelW KernelH shiftX shiftY kernel scale bias tilingMode)
    (let ((tm (int->VGTilingMode tilingMode)))
      ((foreign-lambda void "vgConvolveNormal"
		       VGImage VGImage VGint VGint VGint VGint (const VGshort-vector)
		       VGfloat unsigned-char VGTilingMode)
       dst src kernelW KernelH shiftX shiftY kernel scale bias tm))))
(define vgSeparableConvolveNormal
  (lambda (dst src kernelW KernelH shiftX shiftY kernelX kernelY scale bias tilingMode)
    (let ((tm (int->VGTilingMode tilingMode)))
      ((foreign-lambda void "vgSeparableConvolveNormal"
		       VGImage VGImage VGint VGint VGint VGint (const VGshort-vector)
		       (const VGshort-vector) VGfloat unsigned-char VGTilingMode) 
       dst src kernelW KernelH shiftX shiftY kernelX kernelY scale bias tm))))
(define vgColorMatrixNormal (foreign-lambda void "vgColorMatrixNormal" VGImage VGImage (const VGfloat-vector)))
(define vgCreateImagePainted 
  (lambda (fmt width height quality red green blue alpha)
    (let ((format (int->VGImageFormat fmt)))
      ((foreign-lambda VGImage "vgCreateImagePainted" 
		       VGImageFormat VGint VGint VGbitfield unsigned-char unsigned-char unsigned-char unsigned-char)
       format width height quality red green blue alpha))))
(define vgPixelBits
  (lambda (format)
    (let ((fmt (int->VGImageFormat format)))
      ((foreign-lambda VGint "vgPixelBits" VGImageFormat)
       fmt))))

;;; cvpi_image_functions.h
(define-foreign-type cvpi_pixel (union "cvpi_pixel"))

(define-foreign-variable CVPI_COLOR_SPACE (const int) "CVPI_COLOR_SPACE")
(define-foreign-variable CVPI_PIXEL_BYTES (const int) "CVPI_PIXEL_BYTES")
(define-foreign-variable cvpi_pixel_red (const int) "cvpi_pixel_red")
(define-foreign-variable cvpi_pixel_green (const int) "cvpi_pixel_green")
(define-foreign-variable cvpi_pixel_blue (const int) "cvpi_pixel_blue")
(define-foreign-variable cvpi_pixel_alpha (const int) "cvpi_pixel_alpha")
(define-foreign-variable cvpi_pixel_v (const int) "cvpi_pixel_v")
(define-foreign-variable cvpi_pixel_u (const int) "cvpi_pixel_u")
(define-foreign-variable cvpi_pixel_y (const int) "cvpi_pixel_y")
(define-foreign-variable cvpi_invert_colors (const VGfloat-vector) "cvpi_invert_colors")
(define-foreign-variable cvpi_avuy2ayuv (const VGfloat-vector) "cvpi_avuy2ayuv")
(define-foreign-variable cvpi_pixel_average (const VGfloat-vector) "cvpi_pixel_average")
(define-foreign-variable cvpi_pixel_color_average (const VGfloat-vector) "cvpi_pixel_color_average")
(define-foreign-variable cvpi_channel_red (const VGfloat-vector) "cvpi_channel_red")
(define-foreign-variable cvpi_channel_green (const VGfloat-vector) "cvpi_channel_green")
(define-foreign-variable cvpi_channel_blue (const VGfloat-vector) "cvpi_channel_blue")
(define-foreign-variable cvpi_channel_alpha (const VGfloat-vector) "cvpi_channel_alpha")
(define-foreign-variable cvpi_sqrt_array_floor (const VGubyte-vector) "cvpi_sqrt_array_floor")
(define-foreign-variable cvpi_sqrt_array_ceil (const VGubyte-vector) "cvpi_sqrt_array_ceil")
(define-foreign-variable cvpi_sqrt_array_round (const VGubyte-vector) "cvpi_sqrt_array_round")
(define-foreign-variable cvpi_binary_array (const VGuint-vector) "cvpi_binary_array")
(define-foreign-variable cvpi_binary_array_inverted (const VGuint-vector) "cvpi_binary_array_inverted")
(define-foreign-variable cvpi_zeros_array (const VGubyte-vector) "cvpi_zeros_array")
(define-foreign-variable cvpi_255_array (const VGubyte-vector) "cvpi_255_array")
(define-foreign-variable cvpi_identity_array (const VGubyte-vector) "cvpi_identity_array")
(define-foreign-variable cvpi_inversion_array (const VGubyte-vector) "cvpi_inversion_array")
(define-foreign-variable cvpi_filter_sobel_x (const VGshort-vector) "cvpi_filter_sobel_x")
(define-foreign-variable cvpi_filter_sobel_y (const VGshort-vector) "cvpi_filter_sobel_y")
(define-foreign-variable cvpi_filter_scharr_x (const VGshort-vector) "cvpi_filter_scharr_x")
(define-foreign-variable cvpi_filter_scharr_y (const VGshort-vector) "cvpi_filter_scharr_y")
(define-foreign-variable cvpi_filter_prewitt_x (const VGshort-vector) "cvpi_filter_prewitt_x")
(define-foreign-variable cvpi_filter_prewitt_y (const VGshort-vector) "cvpi_filter_prewitt_y")
(define-foreign-variable cvpi_filter_roberts_cross_x (const VGshort-vector) "cvpi_filter_roberts_cross_x")
(define-foreign-variable cvpi_filter_roberts_cross_y (const VGshort-vector) "cvpi_filter_roberts_cross_y")

(define cvpi_yuyv2yuva (foreign-lambda VGImage "cvpi_yuyv2yuva" (const VGImage)))
(define cvpi_add_images (foreign-lambda VGImage "cvpi_add_images"
			 (const VGImage) (const VGImage) VGshort VGshort VGfloat VGfloat))
(define cvpi_add_channels
  (lambda (image channel1 channel2 a b scale bias output_channels)
    (let ((c1 (int->VGImageChannel channel1)) (c2 (int->VGImageChannel channel2)))
      ((foreign-lambda VGImage "cvpi_add_channels"
		       (const VGImage) VGImageChannel VGImageChannel VGshort VGshort VGfloat VGfloat VGbitfield)
       image c1 c2 a b scale bias output_channels))))
;
(define cvpi_add_channels_color (foreign-lambda VGImage "cvpi_add_channels_color"
						(const VGImage) VGfloat VGfloat VGfloat VGfloat VGbitfield))
(define cvpi_add_channels_all (foreign-lambda VGImage "cvpi_add_channels_all"
					      (const VGImage) VGfloat VGfloat VGfloat VGfloat VGfloat VGbitfield))
(define cvpi_combine_channelwise (foreign-lambda VGImage "cvpi_combine_channelwise"
						 (const VGImage) (const VGImage) VGbitfield))
(define cvpi_channel_threshold
  (lambda (image channel bound1 bound2 fill invert)
    (let ((c (int->VGImageChannel channel)))
      ((foreign-lambda VGImage "cvpi_channel_threshold"
		       (const VGImage) VGImageChannel VGubyte VGubyte VGubyte CVPI_BOOL)
       image c bound1 bound2 fill invert))))
(define cvpi_image_threshold
  (foreign-lambda VGImage "cvpi_image_threshold"
		  (const VGImage) (const VGubyte-vector) (const VGubyte-vector)
		  (const VGubyte-vector) (const (c-pointer CVPI_BOOL)) CVPI_BOOL))
(define cvpi_image_threshold_sector
  (foreign-lambda VGImage "cvpi_image_threshold_sector"
		  (const VGImage) unsigned-int unsigned-int
		  (const VGubyte-vector) (const (c-pointer CVPI_BOOL)) CVPI_BOOL
		  (const (function VGuint ((const VGImage))))))


(define cvpi_image_channel_mask 
  (lambda (image maskChannel add)
    (let ((mc (int->VGImageChannel maskChannel)))
      ((foreign-lambda VGImage "cvpi_image_channel_mask"
		       (const VGImage) VGImageChannel CVPI_BOOL)
       image mc add))))
(define cvpi_image_mean_gpu (foreign-lambda VGImage "cvpi_image_mean_gpu"(const VGImage)))
(define cvpi_image_mean_arithmetic (foreign-lambda VGImage "cvpi_image_mean_arithmetic" (const VGImage)))
(define cvpi_image_rgba_to_bw
  (lambda (color_image sourceChannel nonzero_true)
    (let ((sc (int->VGImageChannel sourceChannel)))
      ((foreign-lambda VGImage "cvpi_image_rgba_to_bw"
		       (const VGImage) VGImageChannel CVPI_BOOL)
       color_image sc nonzero_true))))

(define cvpi_image_logical_and (foreign-lambda VGImage "cvpi_image_logical_and"
					       (const VGImage) (const VGImage) VGubyte VGubyte CVPI_BOOL))
(define cvpi_image_logical_nand (foreign-lambda VGImage "cvpi_image_logical_nand"
						(const VGImage) (const VGImage) VGubyte VGubyte CVPI_BOOL))
(define cvpi_image_logical_or (foreign-lambda VGImage "cvpi_image_logical_or"
					      (const VGImage) (const VGImage) VGubyte VGubyte CVPI_BOOL))
(define cvpi_image_logical_nor (foreign-lambda VGImage "cvpi_image_logical_nor"
					       (const VGImage) (const VGImage) VGubyte VGubyte CVPI_BOOL))
(define cvpi_image_logical_xor (foreign-lambda VGImage "cvpi_image_logical_xor"
					       (const VGImage) (const VGImage) VGubyte VGubyte CVPI_BOOL))
(define cvpi_image_logical_xnor (foreign-lambda VGImage "cvpi_image_logical_xnor"
						(const VGImage) (const VGImage) VGubyte VGubyte CVPI_BOOL))
(define cvpi_image_logical_complement (foreign-lambda VGImage "cvpi_image_logical_complement"
						      (const VGImage) (const VGImage) VGubyte VGubyte CVPI_BOOL))
(define cvpi_image_logical_complement_inv (foreign-lambda VGImage "cvpi_image_logical_complement_inv"
							  (const VGImage) (const VGImage) VGubyte VGubyte CVPI_BOOL))
(define cvpi_image_dialate (foreign-lambda VGImage "cvpi_image_dialate"
					   (const VGImage) VGubyte VGubyte CVPI_BOOL))
(define cvpi_image_erode (foreign-lambda VGImage "cvpi_image_erode"
					 (const VGImage) VGubyte VGubyte CVPI_BOOL))
(define cvpi_image_hit_miss (foreign-lambda VGImage "cvpi_image_hit_miss"
					    (const VGImage) VGubyte VGubyte CVPI_BOOL))
(define cvpi_image_thin (foreign-lambda VGImage "cvpi_image_thin"
					(const VGImage) VGubyte VGubyte CVPI_BOOL))
(define cvpi_image_thicken (foreign-lambda VGImage "cvpi_image_thicken"
					   (const VGImage) VGubyte VGubyte CVPI_BOOL))
(define cvpi_channel_max
  (lambda (image channel)
    (let ((c (int->VGImageChannel channel)))
      ((foreign-lambda VGubyte "cvpi_channel_max"
		       (const VGImage) VGImageChannel)
       image c))))
(define cvpi_channel_min
  (lambda (image channel)
    (let ((c (int->VGImageChannel channel)))
      ((foreign-lambda VGubyte "cvpi_channel_min" (const VGImage) VGImageChannel)
       image c))))
(define cvpi_channel_max_min
  (lambda (image channel)
    (let ((c (int->VGImageChannel channel)))
      ((foreign-lambda VGubyte-vector "cvpi_channel_max_min" (const VGImage) VGImageChannel)
       image c))))


(define cvpi_channel_histogram
  (lambda (image channel)
    (let ((c (int->VGImageChannel channel)))
      ((foreign-lambda (c-pointer uint32_t) "cvpi_channel_histogram" (const VGImage) VGImageChannel)
       image c))))
(define cvpi_color_channels_histogram
  (foreign-lambda (c-pointer uint32_t) "cvpi_color_channels_histogram" (const VGImage)))
(define cvpi_image_histogram
  (foreign-lambda (c-pointer uint32_t) cvpi_image_histogram (const VGImage)))
(define cvpi_channel_cumulative_distribution
  (foreign-lambda (c-pointer double) "cvpi_channel_cumulative_distribution" (const u32vector) VGint VGint))
(define cvpi_color_channels_cumulative_distribution
  (foreign-lambda (c-pointer double) "cvpi_color_channels_cumulative_distribution" (const u32vector) VGint VGint))
(define cvpi_image_cumulative_distribution
  (foreign-lambda (c-pointer double) "cvpi_image_cumulative_distribution" (const u32vector) VGint VGint))


(define-foreign-enum-type (cvpi_integer_truncation int cvpi_floor)
  (cvpi_integer_truncation->int int->cvpi_integer_truncation)
  cvpi_floor cvpi_ceil cvpi_round)
(define cvpi_magnitude
  (lambda (image1 image2 int-truncation)
    (let ((trunc (int->cvpi_integer_truncation int-truncation)))
      ((foreign-lambda VGImage "cvpi_magnitude" (const VGImage) (const VGImage) cvpi_integer_truncation)
       image1 image2 trunc))))

;; typedef union {
;;   uint32_t all
;;   uint16_t xy[2]
;; } cvpi_coordinate

(define-foreign-type cvpi_coordinate (union "cvpi_coordinate"))

;; typedef struct {
;;   cvpi_coordinate* coordinates
;;   unsigned long length
;; } cvpi_coordinate_table
(define-foreign-type cvpi_coordinate_table (struct "cvpi_coordinate_table"))

;; cvpi_coordinate_table* cvpi_image_coordinate_table(const VGImage image, VGbitfield channels)
(define cvpi_image_coordinate_table
  (foreign-lambda (c-pointer cvpi_coordinate_table) "cvpi_image_coordinate_table"
		  (const VGImage) VGbitfield))

;; void cvpi_image_coordinate_table_destroy(cvpi_coordinate_table *coordinate_table)
(define cvpi_image_coordinate_table_destroy
  (foreign-lambda void "cvpi_image_coordinate_table_destroy" (c-pointer cvpi_coordinate_table)))

;; typedef struct {
;;   double x
;;   double y
;; } cvpi_precise_coordinate
(define-foreign-type cvpi_precise_coordinate (struct "cvpi_precise_coordinate"))

;; typedef struct {
;;   cvpi_precise_coordinate* coordinates
;;   unsigned long length
;; } cvpi_precise_coordinate_table
(define-foreign-type cvpi_precise_coordinate_table (struct "cvpi_precise_coordinate_table"))

;; cvpi_precise_coordinate_table* cvpi_coordinate_table_lens_correct(const cvpi_coordinate_table *coordinate_table,
;; 								  double k1, double k2, double p1, double p2, double k3)
(define cvpi_coordinate_table_lens_correct
  (foreign-lambda (c-pointer cvpi_precise_coordinate_table) "cvpi_coordinate_table_lens_correct"
		  (const (c-pointer cvpi_coordinate_table))
		  double double double double double))

;; void cvpi_precise_coordinate_table_destroy(cvpi_precise_coordinate_table *coordinate_table)
(define cvpi_precise_coordinate_table_destroy
  (foreign-lambda void "cvpi_precise_coordinate_table_destroy"
		  (c-pointer cvpi_precise_coordinate_table)))

;; typedef struct {
;;   double radius		
;;   double theta
;; } cvpi_polar_coordinate
(define-foreign-type cvpi_polar_coordinate (struct "cvpi_polar_coordinate"))

;; typedef struct {
;;   cvpi_polar_coordinate* coordinates
;;   unsigned long length
;; } cvpi_polar_coordinate_table
(define-foreign-type cvpi_polar_coordinate_table (struct "cvpi_polar_coordinate_table"))

;; cvpi_polar_coordinate_table* cvpi_image_polar_coordinate_table(const cvpi_coordinate_table *ct)
(define cvpi_image_polar_coordinate_table
  (foreign-lambda (c-pointer cvpi_polar_coordinate_table) "cvpi_image_polar_coordinate_table"
		  (const (c-pointer cvpi_coordinate_table))))

;; cvpi_polar_coordinate_table* cvpi_image_polar_coordinate_table_lens_corrected(const cvpi_precise_coordinate_table *ct)
(define cvpi_image_polar_coordinate_table_lens_corrected
  (foreign-lambda (c-pointer cvpi_polar_coordinate_table)
		  "cvpi_image_polar_coordinate_table_lens_corrected"
		  (const (c-pointer cvpi_precise_coordinate_table))))

;; void cvpi_image_polar_coordinate_table_destroy(cvpi_polar_coordinate_table *ct)
(define cvpi_image_polar_coordinate_table_destroy
  (foreign-lambda void "cvpi_image_polar_coordinate_table_destroy"
		  (c-pointer cvpi_polar_coordinate_table)))

;; void cvpi_avuy2argb(cvpi_pixel* yuva, cvpi_pixel* rgba, unsigned long width, unsigned long height)
(define cvpi_avuy2argb
  (foreign-lambda void "cvpi_avuy2argb"
		  (c-pointer cvpi_pixel) (c-pointer cvpi_pixel) unsigned-long unsigned-long))

;; cvpi_pixel* cvpi_image_rgba(const VGImage image)
(define cvpi_image_rgba
  (foreign-lambda (c-pointer cvpi_pixel) "cvpi_image_rgba" (const VGImage)))
