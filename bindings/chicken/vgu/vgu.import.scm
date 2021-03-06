;;;; vgu.import.scm - GENERATED BY CHICKEN 4.8.0.5 -*- Scheme -*-

(eval '(import chicken scheme foreign foreigners))
(##sys#register-compiled-module
  'vgu
  (list)
  '((vguComputeWarpQuadToQuad . vgu#vguComputeWarpQuadToQuad)
    (vguComputeWarpSquareToQuad . vgu#vguComputeWarpSquareToQuad)
    (vguComputeWarpQuadToSquare . vgu#vguComputeWarpQuadToSquare)
    (vguArc . vgu#vguArc)
    (vguEllipse . vgu#vguEllipse)
    (vguRoundRect . vgu#vguRoundRect)
    (vguRect . vgu#vguRect)
    (vguPolygon . vgu#vguPolygon)
    (vguLine . vgu#vguLine)
    (g3476 . vgu#g3476)
    (g3475 . vgu#g3475)
    (VGUErrorCode->int . vgu#VGUErrorCode->int)
    (int->VGUErrorCode . vgu#int->VGUErrorCode)
    (g3417 . vgu#g3417)
    (g3416 . vgu#g3416)
    (VGUArcType->int . vgu#VGUArcType->int)
    (int->VGUArcType . vgu#int->VGUArcType)
    (vgLookupSingle . vgu#vgLookupSingle)
    (vgLookup . vgu#vgLookup)
    (vgGaussianBlur . vgu#vgGaussianBlur)
    (vgSeparableConvolve . vgu#vgSeparableConvolve)
    (vgConvolve . vgu#vgConvolve)
    (vgColorMatrix . vgu#vgColorMatrix)
    (vgDrawGlyphs . vgu#vgDrawGlyphs)
    (vgDrawGlyph . vgu#vgDrawGlyph)
    (vgClearGlyph . vgu#vgClearGlyph)
    (vgSetGlyphToImage . vgu#vgSetGlyphToImage)
    (vgSetGlyphToPath . vgu#vgSetGlyphToPath)
    (vgDestroyFont . vgu#vgDestroyFont)
    (vgCreateFont . vgu#vgCreateFont)
    (vgCopyPixel . vgu#vgCopyPixel)
    (vgReadPixels . vgu#vgReadPixels)
    (vgGetPixels . vgu#vgGetPixels)
    (vgWritePixels . vgu#vgWritePixels)
    (vgSetPixels . vgu#vgSetPixels)
    (vgDrawImage . vgu#vgDrawImage)
    (vgCopyImage . vgu#vgCopyImage)
    (vgGetParent . vgu#vgGetParent)
    (vgChildImage . vgu#vgChildImage)
    (vgGetImageSubData . vgu#vgGetImageSubData)
    (vgImageSubData . vgu#vgImageSubData)
    (vgClearImage . vgu#vgClearImage)
    (vgDestroyImage . vgu#vgDestroyImage)
    (vgCreateImage . vgu#vgCreateImage)
    (vgPaintPattern . vgu#vgPaintPattern)
    (vgGetColor . vgu#vgGetColor)
    (vgSetColor . vgu#vgSetColor)
    (vgSetParameterfv-paint-color . vgu#vgSetParameterfv-paint-color)
    (vgGetPaint . vgu#vgGetPaint)
    (vgSetPaint . vgu#vgSetPaint)
    (vgDestroyPaint . vgu#vgDestroyPaint)
    (vgCreatePaint . vgu#vgCreatePaint)
    (vgDrawPath . vgu#vgDrawPath)
    (vgPathLength . vgu#vgPathLength)
    (vgPathTransformedBounds . vgu#vgPathTransformedBounds)
    (vgPathBounds . vgu#vgPathBounds)
    (vgPointAlongPath . vgu#vgPointAlongPath)
    (vgPathLength . vgu#vgPathLength)
    (vgInterpolatePath . vgu#vgInterpolatePath)
    (vgTransformPath . vgu#vgTransformPath)
    (vgModifyPathCoords . vgu#vgModifyPathCoords)
    (vgAppendPathData . vgu#vgAppendPathData)
    (vgAppendPath . vgu#vgAppendPath)
    (vgRemovePathCapabilities . vgu#vgRemovePathCapabilities)
    (vgGetPathCapabilities . vgu#vgGetPathCapabilities)
    (vgDestroyPath . vgu#vgDestroyPath)
    (vgClearPath . vgu#vgClearPath)
    (vgCreatePath . vgu#vgCreatePath)
    (vgClear . vgu#vgClear)
    (vgCopyMask . vgu#vgCopyMask)
    (vgFillMaskLayer . vgu#vgFillMaskLayer)
    (vgDestroyMaskLayer . vgu#vgDestroyMaskLayer)
    (vgCreateMaskLayer . vgu#vgCreateMaskLayer)
    (vgRenderToMask . vgu#vgRenderToMask)
    (vgMask . vgu#vgMask)
    (vgRotate . vgu#vgRotate)
    (vgShear . vgu#vgShear)
    (vgScale . vgu#vgScale)
    (vgTranslate . vgu#vgTranslate)
    (vgMultMatrix . vgu#vgMultMatrix)
    (vgGetMatrix . vgu#vgGetMatrix)
    (vgLoadMatrix . vgu#vgLoadMatrix)
    (vgLoadIdentity . vgu#vgLoadIdentity)
    (vgGetParameteriv . vgu#vgGetParameteriv)
    (vgGetParameterfv . vgu#vgGetParameterfv)
    (vgGetParameterVectorSize . vgu#vgGetParameterVectorSize)
    (vgGetParameteri . vgu#vgGetParameteri)
    (vgGetParameterf . vgu#vgGetParameterf)
    (vgSetParameteriv . vgu#vgSetParameteriv)
    (vgSetParameterfv . vgu#vgSetParameterfv)
    (vgSetParameteri . vgu#vgSetParameteri)
    (vgSetParameterf . vgu#vgSetParameterf)
    (vgGetiv . vgu#vgGetiv)
    (vgGetfv . vgu#vgGetfv)
    (vgGetVectorSize . vgu#vgGetVectorSize)
    (vgGeti . vgu#vgGeti)
    (vgGetf . vgu#vgGetf)
    (vgSetiv . vgu#vgSetiv)
    (vgSetfv . vgu#vgSetfv)
    (vgSeti . vgu#vgSeti)
    (vgSetf . vgu#vgSetf)
    (vgFinish . vgu#vgFinish)
    (vgFlush . vgu#vgFlush)
    (vgGetError . vgu#vgGetError)
    (g1966 . vgu#g1966)
    (g1965 . vgu#g1965)
    (VGTilingMode->int . vgu#VGTilingMode->int)
    (int->VGTilingMode . vgu#int->VGTilingMode)
    (g1913 . vgu#g1913)
    (g1912 . vgu#g1912)
    (VGStringID->int . vgu#VGStringID->int)
    (int->VGStringID . vgu#int->VGStringID)
    (g1860 . vgu#g1860)
    (g1859 . vgu#g1859)
    (VGRenderingQuality->int . vgu#VGRenderingQuality->int)
    (int->VGRenderingQuality . vgu#int->VGRenderingQuality)
    (g1810 . vgu#g1810)
    (g1809 . vgu#g1809)
    (VGPixelLayout->int . vgu#VGPixelLayout->int)
    (int->VGPixelLayout . vgu#int->VGPixelLayout)
    (g1754 . vgu#g1754)
    (g1753 . vgu#g1753)
    (VGPathSegment->int . vgu#VGPathSegment->int)
    (int->VGPathSegment . vgu#int->VGPathSegment)
    (g1674 . vgu#g1674)
    (g1673 . vgu#g1673)
    (VGPathParamType->int . vgu#VGPathParamType->int)
    (int->VGPathParamType . vgu#int->VGPathParamType)
    (g1615 . vgu#g1615)
    (g1614 . vgu#g1614)
    (VGPathDatatype->int . vgu#VGPathDatatype->int)
    (int->VGPathDatatype . vgu#int->VGPathDatatype)
    (g1562 . vgu#g1562)
    (g1561 . vgu#g1561)
    (VGPathCommand->int . vgu#VGPathCommand->int)
    (int->VGPathCommand . vgu#int->VGPathCommand)
    (g1449 . vgu#g1449)
    (g1448 . vgu#g1448)
    (VGPathCapabilities->int . vgu#VGPathCapabilities->int)
    (int->VGPathCapabilities . vgu#int->VGPathCapabilities)
    (g1369 . vgu#g1369)
    (g1368 . vgu#g1368)
    (VGPathAbsRel->int . vgu#VGPathAbsRel->int)
    (int->VGPathAbsRel . vgu#int->VGPathAbsRel)
    (g1322 . vgu#g1322)
    (g1321 . vgu#g1321)
    (VGParamType->int . vgu#VGParamType->int)
    (int->VGParamType . vgu#int->VGParamType)
    (g1170 . vgu#g1170)
    (g1169 . vgu#g1169)
    (VGPaintType->int . vgu#VGPaintType->int)
    (int->VGPaintType . vgu#int->VGPaintType)
    (g1117 . vgu#g1117)
    (g1116 . vgu#g1116)
    (VGPaintParamType->int . vgu#VGPaintParamType->int)
    (int->VGPaintParamType . vgu#int->VGPaintParamType)
    (g1052 . vgu#g1052)
    (g1051 . vgu#g1051)
    (VGPaintMode->int . vgu#VGPaintMode->int)
    (int->VGPaintMode . vgu#int->VGPaintMode)
    (g1005 . vgu#g1005)
    (g1004 . vgu#g1004)
    (VGMatrixMode->int . vgu#VGMatrixMode->int)
    (int->VGMatrixMode . vgu#int->VGMatrixMode)
    (g949 . vgu#g949)
    (g948 . vgu#g948)
    (VGMaskOperation->int . vgu#VGMaskOperation->int)
    (int->VGMaskOperation . vgu#int->VGMaskOperation)
    (g890 . vgu#g890)
    (g889 . vgu#g889)
    (VGJoinStyle->int . vgu#VGJoinStyle->int)
    (int->VGJoinStyle . vgu#int->VGJoinStyle)
    (g840 . vgu#g840)
    (g839 . vgu#g839)
    (VGImageQuality->int . vgu#VGImageQuality->int)
    (int->VGImageQuality . vgu#int->VGImageQuality)
    (g790 . vgu#g790)
    (g789 . vgu#g789)
    (VGImageParamType->int . vgu#VGImageParamType->int)
    (int->VGImageParamType . vgu#int->VGImageParamType)
    (g740 . vgu#g740)
    (g739 . vgu#g739)
    (VGImageMode->int . vgu#VGImageMode->int)
    (int->VGImageMode . vgu#int->VGImageMode)
    (g690 . vgu#g690)
    (g689 . vgu#g689)
    (VGImageFormat->int . vgu#VGImageFormat->int)
    (int->VGImageFormat . vgu#int->VGImageFormat)
    (g529 . vgu#g529)
    (g528 . vgu#g528)
    (VGImageChannel->int . vgu#VGImageChannel->int)
    (int->VGImageChannel . vgu#int->VGImageChannel)
    (g476 . vgu#g476)
    (g475 . vgu#g475)
    (VGHardwareQueryType->int . vgu#VGHardwareQueryType->int)
    (int->VGHardwareQueryType . vgu#int->VGHardwareQueryType)
    (g429 . vgu#g429)
    (g428 . vgu#g428)
    (VGHardwareQueryResult->int . vgu#VGHardwareQueryResult->int)
    (int->VGHardwareQueryResult . vgu#int->VGHardwareQueryResult)
    (g382 . vgu#g382)
    (g381 . vgu#g381)
    (VGFontParamType->int . vgu#VGFontParamType->int)
    (int->VGFontParamType . vgu#int->VGFontParamType)
    (g338 . vgu#g338)
    (g337 . vgu#g337)
    (VGFillRule->int . vgu#VGFillRule->int)
    (int->VGFillRule . vgu#int->VGFillRule)
    (g291 . vgu#g291)
    (g290 . vgu#g290)
    (VGErrorCode->int . vgu#VGErrorCode->int)
    (int->VGErrorCode . vgu#int->VGErrorCode)
    (g223 . vgu#g223)
    (g222 . vgu#g222)
    (VGColorRampSpreadMode->int . vgu#VGColorRampSpreadMode->int)
    (int->VGColorRampSpreadMode . vgu#int->VGColorRampSpreadMode)
    (g173 . vgu#g173)
    (g172 . vgu#g172)
    (VGCapStyle->int . vgu#VGCapStyle->int)
    (int->VGCapStyle . vgu#int->VGCapStyle)
    (g123 . vgu#g123)
    (g122 . vgu#g122)
    (VGBlendMode->int . vgu#VGBlendMode->int)
    (int->VGBlendMode . vgu#int->VGBlendMode)
    (g52 . vgu#g52)
    (g51 . vgu#g51)
    (VGboolean->int . vgu#VGboolean->int)
    (int->VGboolean . vgu#int->VGboolean))
  (list)
  (list))

;; END OF FILE
