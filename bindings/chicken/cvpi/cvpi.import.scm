;;;; cvpi.import.scm - GENERATED BY CHICKEN 4.8.0.5 -*- Scheme -*-

(eval '(import chicken scheme foreign foreigners srfi-4 fmt))
(##sys#register-compiled-module
  'cvpi
  (list)
  '((cvpi_image_rgba . cvpi#cvpi_image_rgba)
    (cvpi_avuy2argb . cvpi#cvpi_avuy2argb)
    (cvpi_image_polar_coordinate_table_destroy
      .
      cvpi#cvpi_image_polar_coordinate_table_destroy)
    (cvpi_image_polar_coordinate_table_lens_corrected
      .
      cvpi#cvpi_image_polar_coordinate_table_lens_corrected)
    (cvpi_image_polar_coordinate_table
      .
      cvpi#cvpi_image_polar_coordinate_table)
    (cvpi_precise_coordinate_table_destroy
      .
      cvpi#cvpi_precise_coordinate_table_destroy)
    (cvpi_coordinate_table_lens_correct
      .
      cvpi#cvpi_coordinate_table_lens_correct)
    (cvpi_image_coordinate_table_destroy
      .
      cvpi#cvpi_image_coordinate_table_destroy)
    (cvpi_image_coordinate_table . cvpi#cvpi_image_coordinate_table)
    (cvpi_magnitude . cvpi#cvpi_magnitude)
    (g5114 . cvpi#g5114)
    (g5113 . cvpi#g5113)
    (cvpi_integer_truncation->int . cvpi#cvpi_integer_truncation->int)
    (int->cvpi_integer_truncation . cvpi#int->cvpi_integer_truncation)
    (cvpi_image_cumulative_distribution
      .
      cvpi#cvpi_image_cumulative_distribution)
    (cvpi_color_channels_cumulative_distribution
      .
      cvpi#cvpi_color_channels_cumulative_distribution)
    (cvpi_channel_cumulative_distribution
      .
      cvpi#cvpi_channel_cumulative_distribution)
    (cvpi_image_histogram . cvpi#cvpi_image_histogram)
    (cvpi_color_channels_histogram . cvpi#cvpi_color_channels_histogram)
    (cvpi_channel_histogram . cvpi#cvpi_channel_histogram)
    (cvpi_channel_max_min . cvpi#cvpi_channel_max_min)
    (cvpi_channel_min . cvpi#cvpi_channel_min)
    (cvpi_channel_max . cvpi#cvpi_channel_max)
    (cvpi_image_thicken . cvpi#cvpi_image_thicken)
    (cvpi_image_thin . cvpi#cvpi_image_thin)
    (cvpi_image_hit_miss . cvpi#cvpi_image_hit_miss)
    (cvpi_image_erode . cvpi#cvpi_image_erode)
    (cvpi_image_dialate . cvpi#cvpi_image_dialate)
    (cvpi_image_logical_complement_inv
      .
      cvpi#cvpi_image_logical_complement_inv)
    (cvpi_image_logical_complement . cvpi#cvpi_image_logical_complement)
    (cvpi_image_logical_xnor . cvpi#cvpi_image_logical_xnor)
    (cvpi_image_logical_xor . cvpi#cvpi_image_logical_xor)
    (cvpi_image_logical_nor . cvpi#cvpi_image_logical_nor)
    (cvpi_image_logical_or . cvpi#cvpi_image_logical_or)
    (cvpi_image_logical_nand . cvpi#cvpi_image_logical_nand)
    (cvpi_image_logical_and . cvpi#cvpi_image_logical_and)
    (cvpi_image_rgba_to_bw . cvpi#cvpi_image_rgba_to_bw)
    (cvpi_image_mean_arithmetic . cvpi#cvpi_image_mean_arithmetic)
    (cvpi_image_mean_gpu . cvpi#cvpi_image_mean_gpu)
    (cvpi_image_channel_mask . cvpi#cvpi_image_channel_mask)
    (cvpi_image_threshold_sector . cvpi#cvpi_image_threshold_sector)
    (cvpi_image_threshold . cvpi#cvpi_image_threshold)
    (cvpi_channel_threshold . cvpi#cvpi_channel_threshold)
    (cvpi_combine_channelwise . cvpi#cvpi_combine_channelwise)
    (cvpi_add_channels_all . cvpi#cvpi_add_channels_all)
    (cvpi_add_channels_color . cvpi#cvpi_add_channels_color)
    (cvpi_add_channels . cvpi#cvpi_add_channels)
    (cvpi_add_images . cvpi#cvpi_add_images)
    (cvpi_yuyv2yuva . cvpi#cvpi_yuyv2yuva)
    (vgPixelBits . cvpi#vgPixelBits)
    (vgCreateImagePainted . cvpi#vgCreateImagePainted)
    (vgColorMatrixNormal . cvpi#vgColorMatrixNormal)
    (vgSeparableConvolveNormal . cvpi#vgSeparableConvolveNormal)
    (vgConvolveNormal . cvpi#vgConvolveNormal)
    (cvpi_pgm_header_write . cvpi#cvpi_pgm_header_write)
    (cvpi_pbm_header_write . cvpi#cvpi_pbm_header_write)
    (cvpi_bmp_header_write . cvpi#cvpi_bmp_header_write)
    (g4399 . cvpi#g4399)
    (g4398 . cvpi#g4398)
    (cvpi_bmp_pixel_format->int . cvpi#cvpi_bmp_pixel_format->int)
    (int->cvpi_bmp_pixel_format . cvpi#int->cvpi_bmp_pixel_format)
    (cvpi_egl_error_string . cvpi#cvpi_egl_error_string)
    (cvpi_egl_settings_transparent_blue_value
      .
      cvpi#cvpi_egl_settings_transparent_blue_value)
    (cvpi_egl_settings_transparent_blue_value_check
      .
      cvpi#cvpi_egl_settings_transparent_blue_value_check)
    (cvpi_egl_settings_transparent_green_value
      .
      cvpi#cvpi_egl_settings_transparent_green_value)
    (cvpi_egl_settings_transparent_green_value_check
      .
      cvpi#cvpi_egl_settings_transparent_green_value_check)
    (cvpi_egl_settings_transparent_red_value
      .
      cvpi#cvpi_egl_settings_transparent_red_value)
    (cvpi_egl_settings_transparent_red_value_check
      .
      cvpi#cvpi_egl_settings_transparent_red_value_check)
    (cvpi_egl_settings_surface_type . cvpi#cvpi_egl_settings_surface_type)
    (cvpi_egl_settings_surface_type_check
      .
      cvpi#cvpi_egl_settings_surface_type_check)
    (cvpi_egl_settings_stencil_size . cvpi#cvpi_egl_settings_stencil_size)
    (cvpi_egl_settings_stencil_size_check
      .
      cvpi#cvpi_egl_settings_stencil_size_check)
    (cvpi_egl_settings_renderable_type
      .
      cvpi#cvpi_egl_settings_renderable_type)
    (cvpi_egl_settings_min_swap_interval
      .
      cvpi#cvpi_egl_settings_min_swap_interval)
    (cvpi_egl_settings_min_swap_interval_check
      .
      cvpi#cvpi_egl_settings_min_swap_interval_check)
    (cvpi_egl_settings_max_swap_interval
      .
      cvpi#cvpi_egl_settings_max_swap_interval)
    (cvpi_egl_settings_max_swap_interval_check
      .
      cvpi#cvpi_egl_settings_max_swap_interval_check)
    (cvpi_egl_settings_match_native_pixmap_check
      .
      cvpi#cvpi_egl_settings_match_native_pixmap_check)
    (cvpi_egl_settings_max_pbuffer_pixels_check
      .
      cvpi#cvpi_egl_settings_max_pbuffer_pixels_check)
    (cvpi_egl_settings_max_pbuffer_height_check
      .
      cvpi#cvpi_egl_settings_max_pbuffer_height_check)
    (cvpi_egl_settings_max_pbuffer_width_check
      .
      cvpi#cvpi_egl_settings_max_pbuffer_width_check)
    (cvpi_egl_settings_depth_size . cvpi#cvpi_egl_settings_depth_size)
    (cvpi_egl_settings_depth_size_check
      .
      cvpi#cvpi_egl_settings_depth_size_check)
    (cvpi_egl_settings_conformant . cvpi#cvpi_egl_settings_conformant)
    (cvpi_egl_settings_conformant_check
      .
      cvpi#cvpi_egl_settings_conformant_check)
    (cvpi_egl_settings_config_id . cvpi#cvpi_egl_settings_config_id)
    (cvpi_egl_settings_config_id_check
      .
      cvpi#cvpi_egl_settings_config_id_check)
    (cvpi_egl_settings_alpha_mask_size
      .
      cvpi#cvpi_egl_settings_alpha_mask_size)
    (cvpi_egl_settings_alpha_mask_size_check
      .
      cvpi#cvpi_egl_settings_alpha_mask_size_check)
    (cvpi_egl_settings_alpha_size . cvpi#cvpi_egl_settings_alpha_size)
    (cvpi_egl_settings_alpha_size_check
      .
      cvpi#cvpi_egl_settings_alpha_size_check)
    (cvpi_egl_settings_blue_size . cvpi#cvpi_egl_settings_blue_size)
    (cvpi_egl_settings_blue_size_check
      .
      cvpi#cvpi_egl_settings_blue_size_check)
    (cvpi_egl_settings_green_size . cvpi#cvpi_egl_settings_green_size)
    (cvpi_egl_settings_green_size_check
      .
      cvpi#cvpi_egl_settings_green_size_check)
    (cvpi_egl_settings_red_size . cvpi#cvpi_egl_settings_red_size)
    (cvpi_egl_settings_red_size_check . cvpi#cvpi_egl_settings_red_size_check)
    (cvpi_egl_settings_buffer_size . cvpi#cvpi_egl_settings_buffer_size)
    (cvpi_egl_settings_buffer_size_check
      .
      cvpi#cvpi_egl_settings_buffer_size_check)
    (cvpi_egl_surface_functions_check . cvpi#cvpi_egl_surface_functions_check)
    (cvpi_egl_settings_height_check . cvpi#cvpi_egl_settings_height_check)
    (cvpi_egl_settings_height . cvpi#cvpi_egl_settings_height)
    (cvpi_egl_settings_width_check . cvpi#cvpi_egl_settings_width_check)
    (cvpi_egl_settings_width . cvpi#cvpi_egl_settings_width)
    (cvpi_egl_settings_check . cvpi#cvpi_egl_settings_check)
    (g4005 . cvpi#g4005)
    (g4004 . cvpi#g4004)
    (cvpi_egl_settings_change->int . cvpi#cvpi_egl_settings_change->int)
    (int->cvpi_egl_settings_change . cvpi#int->cvpi_egl_settings_change)
    (cvpi_egl_instance_takedown . cvpi#cvpi_egl_instance_takedown)
    (cvpi_egl_instance_setup . cvpi#cvpi_egl_instance_setup)
    (cvpi_egl_settings_create . cvpi#cvpi_egl_settings_create)
    (g3939 . cvpi#g3939)
    (g3938 . cvpi#g3938)
    (cvpi_egl_samples->int . cvpi#cvpi_egl_samples->int)
    (int->cvpi_egl_samples . cvpi#int->cvpi_egl_samples)
    (g3892 . cvpi#g3892)
    (g3891 . cvpi#g3891)
    (cvpi_egl_current_surface_type->int
      .
      cvpi#cvpi_egl_current_surface_type->int)
    (int->cvpi_egl_current_surface_type
      .
      cvpi#int->cvpi_egl_current_surface_type)
    (g3839 . cvpi#g3839)
    (g3838 . cvpi#g3838)
    (CVPI_EGLBoolean->int . cvpi#CVPI_EGLBoolean->int)
    (int->CVPI_EGLBoolean . cvpi#int->CVPI_EGLBoolean)
    (g3789 . cvpi#g3789)
    (g3788 . cvpi#g3788)
    (cvpi_egl_conformant_bits->int . cvpi#cvpi_egl_conformant_bits->int)
    (int->cvpi_egl_conformant_bits . cvpi#int->cvpi_egl_conformant_bits)
    (g3733 . cvpi#g3733)
    (g3732 . cvpi#g3732)
    (cvpi_egl_renderable_api->int . cvpi#cvpi_egl_renderable_api->int)
    (int->cvpi_egl_renderable_api . cvpi#int->cvpi_egl_renderable_api)
    (g3683 . cvpi#g3683)
    (g3682 . cvpi#g3682)
    (cvpi_egl_config_caveat->int . cvpi#cvpi_egl_config_caveat->int)
    (int->cvpi_egl_config_caveat . cvpi#int->cvpi_egl_config_caveat)
    (g3630 . cvpi#g3630)
    (g3629 . cvpi#g3629)
    (cvpi_egl_color_buffer_type->int . cvpi#cvpi_egl_color_buffer_type->int)
    (int->cvpi_egl_color_buffer_type . cvpi#int->cvpi_egl_color_buffer_type)
    (g3580 . cvpi#g3580)
    (g3579 . cvpi#g3579)
    (cvpi_egl_transparent_type->int . cvpi#cvpi_egl_transparent_type->int)
    (int->cvpi_egl_transparent_type . cvpi#int->cvpi_egl_transparent_type)
    (g3530 . cvpi#g3530)
    (g3529 . cvpi#g3529)
    (cvpi_egl_renderable_bits->int . cvpi#cvpi_egl_renderable_bits->int)
    (int->cvpi_egl_renderable_bits . cvpi#int->cvpi_egl_renderable_bits)
    (g3477 . cvpi#g3477)
    (g3476 . cvpi#g3476)
    (cvpi_egl_surface_bits->int . cvpi#cvpi_egl_surface_bits->int)
    (int->cvpi_egl_surface_bits . cvpi#int->cvpi_egl_surface_bits)
    (CVPI_NOT . cvpi#CVPI_NOT)
    (CVPI_FALSE_TEST . cvpi#CVPI_FALSE_TEST)
    (CVPI_TRUE_TEST . cvpi#CVPI_TRUE_TEST)
    (vgLookupSingle . cvpi#vgLookupSingle)
    (vgLookup . cvpi#vgLookup)
    (vgGaussianBlur . cvpi#vgGaussianBlur)
    (vgSeparableConvolve . cvpi#vgSeparableConvolve)
    (vgConvolve . cvpi#vgConvolve)
    (vgColorMatrix . cvpi#vgColorMatrix)
    (vgDrawGlyphs . cvpi#vgDrawGlyphs)
    (vgDrawGlyph . cvpi#vgDrawGlyph)
    (vgClearGlyph . cvpi#vgClearGlyph)
    (vgSetGlyphToImage . cvpi#vgSetGlyphToImage)
    (vgSetGlyphToPath . cvpi#vgSetGlyphToPath)
    (vgDestroyFont . cvpi#vgDestroyFont)
    (vgCreateFont . cvpi#vgCreateFont)
    (vgCopyPixel . cvpi#vgCopyPixel)
    (vgReadPixels . cvpi#vgReadPixels)
    (vgGetPixels . cvpi#vgGetPixels)
    (vgWritePixels . cvpi#vgWritePixels)
    (vgSetPixels . cvpi#vgSetPixels)
    (vgDrawImage . cvpi#vgDrawImage)
    (vgCopyImage . cvpi#vgCopyImage)
    (vgGetParent . cvpi#vgGetParent)
    (vgChildImage . cvpi#vgChildImage)
    (vgGetImageSubData . cvpi#vgGetImageSubData)
    (vgImageSubData . cvpi#vgImageSubData)
    (vgClearImage . cvpi#vgClearImage)
    (vgDestroyImage . cvpi#vgDestroyImage)
    (vgCreateImage . cvpi#vgCreateImage)
    (vgPaintPattern . cvpi#vgPaintPattern)
    (vgGetColor . cvpi#vgGetColor)
    (vgSetColor . cvpi#vgSetColor)
    (vgSetParameterfv-paint-color . cvpi#vgSetParameterfv-paint-color)
    (vgGetPaint . cvpi#vgGetPaint)
    (vgSetPaint . cvpi#vgSetPaint)
    (vgDestroyPaint . cvpi#vgDestroyPaint)
    (vgCreatePaint . cvpi#vgCreatePaint)
    (vgDrawPath . cvpi#vgDrawPath)
    (vgPathLength . cvpi#vgPathLength)
    (vgPathTransformedBounds . cvpi#vgPathTransformedBounds)
    (vgPathBounds . cvpi#vgPathBounds)
    (vgPointAlongPath . cvpi#vgPointAlongPath)
    (vgPathLength . cvpi#vgPathLength)
    (vgInterpolatePath . cvpi#vgInterpolatePath)
    (vgTransformPath . cvpi#vgTransformPath)
    (vgModifyPathCoords . cvpi#vgModifyPathCoords)
    (vgAppendPathData . cvpi#vgAppendPathData)
    (vgAppendPath . cvpi#vgAppendPath)
    (vgRemovePathCapabilities . cvpi#vgRemovePathCapabilities)
    (vgGetPathCapabilities . cvpi#vgGetPathCapabilities)
    (vgDestroyPath . cvpi#vgDestroyPath)
    (vgClearPath . cvpi#vgClearPath)
    (vgCreatePath . cvpi#vgCreatePath)
    (vgClear . cvpi#vgClear)
    (vgCopyMask . cvpi#vgCopyMask)
    (vgFillMaskLayer . cvpi#vgFillMaskLayer)
    (vgDestroyMaskLayer . cvpi#vgDestroyMaskLayer)
    (vgCreateMaskLayer . cvpi#vgCreateMaskLayer)
    (vgRenderToMask . cvpi#vgRenderToMask)
    (vgMask . cvpi#vgMask)
    (vgRotate . cvpi#vgRotate)
    (vgShear . cvpi#vgShear)
    (vgScale . cvpi#vgScale)
    (vgTranslate . cvpi#vgTranslate)
    (vgMultMatrix . cvpi#vgMultMatrix)
    (vgGetMatrix . cvpi#vgGetMatrix)
    (vgLoadMatrix . cvpi#vgLoadMatrix)
    (vgLoadIdentity . cvpi#vgLoadIdentity)
    (vgGetParameteriv . cvpi#vgGetParameteriv)
    (vgGetParameterfv . cvpi#vgGetParameterfv)
    (vgGetParameterVectorSize . cvpi#vgGetParameterVectorSize)
    (vgGetParameteri . cvpi#vgGetParameteri)
    (vgGetParameterf . cvpi#vgGetParameterf)
    (vgSetParameteriv . cvpi#vgSetParameteriv)
    (vgSetParameterfv . cvpi#vgSetParameterfv)
    (vgSetParameteri . cvpi#vgSetParameteri)
    (vgSetParameterf . cvpi#vgSetParameterf)
    (vgGetiv . cvpi#vgGetiv)
    (vgGetfv . cvpi#vgGetfv)
    (vgGetVectorSize . cvpi#vgGetVectorSize)
    (vgGeti . cvpi#vgGeti)
    (vgGetf . cvpi#vgGetf)
    (vgSetiv . cvpi#vgSetiv)
    (vgSetfv . cvpi#vgSetfv)
    (vgSeti . cvpi#vgSeti)
    (vgSetf . cvpi#vgSetf)
    (vgFinish . cvpi#vgFinish)
    (vgFlush . cvpi#vgFlush)
    (vgGetError . cvpi#vgGetError)
    (g1996 . cvpi#g1996)
    (g1995 . cvpi#g1995)
    (VGTilingMode->int . cvpi#VGTilingMode->int)
    (int->VGTilingMode . cvpi#int->VGTilingMode)
    (g1943 . cvpi#g1943)
    (g1942 . cvpi#g1942)
    (VGStringID->int . cvpi#VGStringID->int)
    (int->VGStringID . cvpi#int->VGStringID)
    (g1890 . cvpi#g1890)
    (g1889 . cvpi#g1889)
    (VGRenderingQuality->int . cvpi#VGRenderingQuality->int)
    (int->VGRenderingQuality . cvpi#int->VGRenderingQuality)
    (g1840 . cvpi#g1840)
    (g1839 . cvpi#g1839)
    (VGPixelLayout->int . cvpi#VGPixelLayout->int)
    (int->VGPixelLayout . cvpi#int->VGPixelLayout)
    (g1784 . cvpi#g1784)
    (g1783 . cvpi#g1783)
    (VGPathSegment->int . cvpi#VGPathSegment->int)
    (int->VGPathSegment . cvpi#int->VGPathSegment)
    (g1704 . cvpi#g1704)
    (g1703 . cvpi#g1703)
    (VGPathParamType->int . cvpi#VGPathParamType->int)
    (int->VGPathParamType . cvpi#int->VGPathParamType)
    (g1645 . cvpi#g1645)
    (g1644 . cvpi#g1644)
    (VGPathDatatype->int . cvpi#VGPathDatatype->int)
    (int->VGPathDatatype . cvpi#int->VGPathDatatype)
    (g1592 . cvpi#g1592)
    (g1591 . cvpi#g1591)
    (VGPathCommand->int . cvpi#VGPathCommand->int)
    (int->VGPathCommand . cvpi#int->VGPathCommand)
    (g1479 . cvpi#g1479)
    (g1478 . cvpi#g1478)
    (VGPathCapabilities->int . cvpi#VGPathCapabilities->int)
    (int->VGPathCapabilities . cvpi#int->VGPathCapabilities)
    (g1399 . cvpi#g1399)
    (g1398 . cvpi#g1398)
    (VGPathAbsRel->int . cvpi#VGPathAbsRel->int)
    (int->VGPathAbsRel . cvpi#int->VGPathAbsRel)
    (g1352 . cvpi#g1352)
    (g1351 . cvpi#g1351)
    (VGParamType->int . cvpi#VGParamType->int)
    (int->VGParamType . cvpi#int->VGParamType)
    (g1200 . cvpi#g1200)
    (g1199 . cvpi#g1199)
    (VGPaintType->int . cvpi#VGPaintType->int)
    (int->VGPaintType . cvpi#int->VGPaintType)
    (g1147 . cvpi#g1147)
    (g1146 . cvpi#g1146)
    (VGPaintParamType->int . cvpi#VGPaintParamType->int)
    (int->VGPaintParamType . cvpi#int->VGPaintParamType)
    (g1082 . cvpi#g1082)
    (g1081 . cvpi#g1081)
    (VGPaintMode->int . cvpi#VGPaintMode->int)
    (int->VGPaintMode . cvpi#int->VGPaintMode)
    (g1035 . cvpi#g1035)
    (g1034 . cvpi#g1034)
    (VGMatrixMode->int . cvpi#VGMatrixMode->int)
    (int->VGMatrixMode . cvpi#int->VGMatrixMode)
    (g979 . cvpi#g979)
    (g978 . cvpi#g978)
    (VGMaskOperation->int . cvpi#VGMaskOperation->int)
    (int->VGMaskOperation . cvpi#int->VGMaskOperation)
    (g920 . cvpi#g920)
    (g919 . cvpi#g919)
    (VGJoinStyle->int . cvpi#VGJoinStyle->int)
    (int->VGJoinStyle . cvpi#int->VGJoinStyle)
    (g870 . cvpi#g870)
    (g869 . cvpi#g869)
    (VGImageQuality->int . cvpi#VGImageQuality->int)
    (int->VGImageQuality . cvpi#int->VGImageQuality)
    (g820 . cvpi#g820)
    (g819 . cvpi#g819)
    (VGImageParamType->int . cvpi#VGImageParamType->int)
    (int->VGImageParamType . cvpi#int->VGImageParamType)
    (g770 . cvpi#g770)
    (g769 . cvpi#g769)
    (VGImageMode->int . cvpi#VGImageMode->int)
    (int->VGImageMode . cvpi#int->VGImageMode)
    (g720 . cvpi#g720)
    (g719 . cvpi#g719)
    (VGImageFormat->int . cvpi#VGImageFormat->int)
    (int->VGImageFormat . cvpi#int->VGImageFormat)
    (g559 . cvpi#g559)
    (g558 . cvpi#g558)
    (VGImageChannel->int . cvpi#VGImageChannel->int)
    (int->VGImageChannel . cvpi#int->VGImageChannel)
    (g506 . cvpi#g506)
    (g505 . cvpi#g505)
    (VGHardwareQueryType->int . cvpi#VGHardwareQueryType->int)
    (int->VGHardwareQueryType . cvpi#int->VGHardwareQueryType)
    (g459 . cvpi#g459)
    (g458 . cvpi#g458)
    (VGHardwareQueryResult->int . cvpi#VGHardwareQueryResult->int)
    (int->VGHardwareQueryResult . cvpi#int->VGHardwareQueryResult)
    (g412 . cvpi#g412)
    (g411 . cvpi#g411)
    (VGFontParamType->int . cvpi#VGFontParamType->int)
    (int->VGFontParamType . cvpi#int->VGFontParamType)
    (g368 . cvpi#g368)
    (g367 . cvpi#g367)
    (VGFillRule->int . cvpi#VGFillRule->int)
    (int->VGFillRule . cvpi#int->VGFillRule)
    (g321 . cvpi#g321)
    (g320 . cvpi#g320)
    (VGErrorCode->int . cvpi#VGErrorCode->int)
    (int->VGErrorCode . cvpi#int->VGErrorCode)
    (g253 . cvpi#g253)
    (g252 . cvpi#g252)
    (VGColorRampSpreadMode->int . cvpi#VGColorRampSpreadMode->int)
    (int->VGColorRampSpreadMode . cvpi#int->VGColorRampSpreadMode)
    (g203 . cvpi#g203)
    (g202 . cvpi#g202)
    (VGCapStyle->int . cvpi#VGCapStyle->int)
    (int->VGCapStyle . cvpi#int->VGCapStyle)
    (g153 . cvpi#g153)
    (g152 . cvpi#g152)
    (VGBlendMode->int . cvpi#VGBlendMode->int)
    (int->VGBlendMode . cvpi#int->VGBlendMode)
    (g82 . cvpi#g82)
    (g81 . cvpi#g81)
    (VGboolean->int . cvpi#VGboolean->int)
    (int->VGboolean . cvpi#int->VGboolean))
  (list)
  (list))

;; END OF FILE
