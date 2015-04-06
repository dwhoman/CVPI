;(define debug? (make-parameter #t))
;;; ffi for openvg
;#>
;#ifndef _OPENVG_H
;#include "openvg.h"
;#endif
;<#

(import-for-syntax bindings)
(use simple-exceptions bindings srfi-45)
(use miscmacros)			;begin0

;;; enable or disable error messages
(define vg-error-print-enable #t)

(foreign-declare "#include <VG/openvg.h>")
;;; Data Types and Number Representation [3]
(define-foreign-type VGbyte byte)
(define-foreign-type VGbyte-vector (c-pointer VGbyte))
(define-foreign-type VGubyte unsigned-byte)
(define-foreign-type VGubyte-vector (c-pointer VGubyte))
(define-foreign-type VGshort short)
(define-foreign-type VGshort-vector (c-pointer VGshort))
(define-foreign-type VGushort unsigned-short)
(define-foreign-type VGushort-vector (c-pointer VGushort))
(define-foreign-type VGint integer32)
(define-foreign-type VGint-vector (c-pointer VGint))
(define-foreign-type VGuint unsigned-integer32)
(define-foreign-type VGuint-vector (c-pointer VGuint))
(define-foreign-type VGbitfield unsigned-integer32)
(define-foreign-type VGbitfield-vector (c-pointer VGbitfield))
(define-foreign-type VGfloat float)
(define-foreign-type VGfloat-vector (c-pointer VGfloat))
(define-foreign-type VGHandle VGuint)
(define-foreign-type VGHandle-vector (c-pointer VGHandle-vector))
(define-foreign-type VGFont VGHandle)
(define-foreign-type VGFont-vector (c-pointer VGFont))
(define-foreign-type VGImage VGHandle)
(define-foreign-type VGImage-vector (c-pointer VGImage))
(define-foreign-type VGMaskLayer VGHandle)
(define-foreign-type VGMaskLayer-vector (c-pointer VGMaskLayer))
(define-foreign-type VGPaint VGHandle)
(define-foreign-type VGPaint-vector (c-pointer VGPaint))
(define-foreign-type VGPath VGHandle)
(define-foreign-type VGPath-vector (c-pointer VGPath))

;;; Constants
(define-foreign-variable OPENVG_VERSION_1_0 (const int) "OPENVG_VERSION_1_0")
(define-foreign-variable OPENVG_VERSION_1_1 (const int) "OPENVG_VERSION_1_1")
(define-foreign-variable VG_MAXSHORT (const VGshort) "VG_MAXSHORT")
(define-foreign-variable VG_MAXINT (const VGint) "VG_MAXINT")
(define-foreign-variable VG_MAX_FLOAT (const VGfloat) "VG_MAX_FLOAT")
(define-foreign-variable VG_INVALID_HANDLE (const VGHandle) "VG_INVALID_HANDLE")
(define-foreign-variable VG_PATH_FORMAT_STANDARD (const int) "VG_PATH_FORMAT_STANDARD")
;;; Enumerated Data Types [3.5]
;; VGboolean
(define-foreign-enum-type (VGboolean int VG_FALSE)
  (VGboolean->int int->VGboolean)
  VG_FALSE
  VG_TRUE)

;; VGBlendMode
(define-foreign-enum-type (VGBlendMode int VG_BLEND_SRC)
  (VGBlendMode->int int->VGBlendMode)
  VG_BLEND_SRC
  VG_BLEND_SRC_OVER
  VG_BLEND_DST_OVER
  VG_BLEND_SRC_IN
  VG_BLEND_DST_IN
  VG_BLEND_MULTIPLY
  VG_BLEND_SCREEN
  VG_BLEND_DARKEN
  VG_BLEND_LIGHTEN
  VG_BLEND_ADDITIVE)
;; VGCapStyle
(define-foreign-enum-type (VGCapStyle int VG_CAP_BUTT)
  (VGCapStyle->int int->VGCapStyle)
  VG_CAP_BUTT
  VG_CAP_ROUND
  VG_CAP_SQUARE)
;; VGColorRampSpreadMode
(define-foreign-enum-type (VGColorRampSpreadMode int VG_COLOR_RAMP_SPREAD_PAD)
  (VGColorRampSpreadMode->int int->VGColorRampSpreadMode)
  VG_COLOR_RAMP_SPREAD_PAD
  VG_COLOR_RAMP_SPREAD_REPEAT
  VG_COLOR_RAMP_SPREAD_REFLECT)
;; VGErrorCode
(define-foreign-enum-type (VGErrorCode int VG_ILLEGAL_ARGUMENT_ERROR)
  (VGErrorCode->int int->VGErrorCode)
  VG_NO_ERROR
  VG_BAD_HANDLE_ERROR
  VG_ILLEGAL_ARGUMENT_ERROR
  VG_OUT_OF_MEMORY_ERROR
  VG_NO_CONTEXT_ERROR
  VG_PATH_CAPABILITY_ERROR
  VG_UNSUPPORTED_IMAGE_FORMAT_ERROR
  VG_UNSUPPORTED_PATH_FORMAT_ERROR
  VG_IMAGE_IN_USE_ERROR)
;; VGFillRule
(define-foreign-enum-type (VGFillRule int VG_EVEN_ODD)
  (VGFillRule->int int->VGFillRule)
  VG_EVEN_ODD
  VG_NON_ZERO)
;; VGFontParamType
(define-foreign-enum-type (VGFontParamType int VG_FONT_NUM_GLYPHS)
  (VGFontParamType->int int->VGFontParamType)
  VG_FONT_NUM_GLYPHS)
;; VGHardwareQueryResult
(define-foreign-enum-type (VGHardwareQueryResult int VG_HARDWARE_UNACCELERATED)
  (VGHardwareQueryResult->int int->VGHardwareQueryResult)
  VG_HARDWARE_ACCELERATED
  VG_HARDWARE_UNACCELERATED)
;; VGHardwareQueryType
(define-foreign-enum-type (VGHardwareQueryType int VG_IMAGE_FORMAT_QUERY)
  (VGHardwareQueryType->int int->VGHardwareQueryType)
  VG_IMAGE_FORMAT_QUERY
  VG_PATH_DATATYPE_QUERY)
;; VGImageChannel
(define-foreign-enum-type (VGImageChannel int VG_RED)
  (VGImageChannel->int int->VGImageChannel)
  VG_RED
  VG_GREEN
  VG_BLUE
  VG_ALPHA)
;; VGImageFormat
(define-foreign-enum-type (VGImageFormat int VG_sRGBX_8888)
  (VGImageFormat->int int->VGImageFormat)
  ;; RGB{A,X} channel ordering
  VG_sRGBX_8888
  VG_sRGBA_8888
  VG_sRGBA_8888_PRE
  VG_sRGB_565
  VG_sRGBA_5551
  VG_sRGBA_4444
  VG_sL_8
  VG_lRGBX_8888
  VG_lRGBA_8888
  VG_lRGBA_8888_PRE
  VG_lL_8
  VG_A_8
  VG_BW_1
  VG_A_1
  VG_A_4
  ;; {A,X}RGB channel ordering
  VG_sXRGB_8888
  VG_sARGB_8888
  VG_sARGB_8888_PRE
  VG_sARGB_1555
  VG_sARGB_4444
  VG_lXRGB_8888
  VG_lARGB_8888
  VG_lARGB_8888_PRE
  ;; BGR{A,X} channel ordering
  VG_sBGRX_8888
  VG_sBGRA_8888
  VG_sBGRA_8888_PRE
  VG_sBGR_565
  VG_sBGRA_5551
  VG_sBGRA_4444
  VG_lBGRX_8888
  VG_lBGRA_8888
  VG_lBGRA_8888_PRE
  ;; {A,X}BGR channel ordering
  VG_sXBGR_8888
  VG_sABGR_8888
  VG_sABGR_8888_PRE
  VG_sABGR_1555
  VG_sABGR_4444
  VG_lXBGR_8888
  VG_lABGR_8888
  VG_lABGR_8888_PRE)
;; VGImageMode
(define-foreign-enum-type (VGImageMode int VG_DRAW_IMAGE_NORMAL)
  (VGImageMode->int int->VGImageMode)
  VG_DRAW_IMAGE_NORMAL
  VG_DRAW_IMAGE_MULTIPLY
  VG_DRAW_IMAGE_STENCIL)
;; VGImageParamType
(define-foreign-enum-type (VGImageParamType int VG_IMAGE_FORMAT)
  (VGImageParamType->int int->VGImageParamType)
  VG_IMAGE_FORMAT
  VG_IMAGE_WIDTH
  VG_IMAGE_HEIGHT)
;; VGImageQuality
(define-foreign-enum-type (VGImageQuality int VG_IMAGE_QUALITY_NONANTIALIASED)
  (VGImageQuality->int int->VGImageQuality)
  VG_IMAGE_QUALITY_NONANTIALIASED
  VG_IMAGE_QUALITY_FASTER
  VG_IMAGE_QUALITY_BETTER)
;; VGJoinStyle
(define-foreign-enum-type (VGJoinStyle int VG_JOIN_MITER)
  (VGJoinStyle->int int->VGJoinStyle)
  VG_JOIN_MITER
  VG_JOIN_ROUND
  VG_JOIN_BEVEL)
;; VGMaskOperation
(define-foreign-enum-type (VGMaskOperation int VG_CLEAR_MASK)
  (VGMaskOperation->int int->VGMaskOperation)
  VG_CLEAR_MASK
  VG_FILL_MASK
  VG_SET_MASK
  VG_UNION_MASK
  VG_INTERSECT_MASK
  VG_SUBTRACT_MASK)
;; VGMatrixMode
(define-foreign-enum-type (VGMatrixMode int VG_MATRIX_PATH_USER_TO_SURFACE)
  (VGMatrixMode->int int->VGMatrixMode)
  VG_MATRIX_PATH_USER_TO_SURFACE
  VG_MATRIX_IMAGE_USER_TO_SURFACE
  VG_MATRIX_FILL_PAINT_TO_USER
  VG_MATRIX_STROKE_PAINT_TO_USER
  VG_MATRIX_GLYPH_USER_TO_SURFACE)
;; VGPaintMode
(define-foreign-enum-type (VGPaintMode int VG_STROKE_PATH)
  (VGPaintMode->int int->VGPaintMode)
  VG_STROKE_PATH
  VG_FILL_PATH)
;; VGPaintParamType
(define-foreign-enum-type (VGPaintParamType int VG_PAINT_TYPE)
  (VGPaintParamType->int int->VGPaintParamType)
  ;; Color paint parameters
  VG_PAINT_TYPE
  VG_PAINT_COLOR
  VG_PAINT_COLOR_RAMP_SPREAD_MODE
  VG_PAINT_COLOR_RAMP_STOPS
  VG_PAINT_COLOR_RAMP_PREMULTIPLIED
  ;; Linear gradient paint parameters
  VG_PAINT_LINEAR_GRADIENT
  ;; Radial gradient paint parameters
  VG_PAINT_RADIAL_GRADIENT
  ;; Pattern paint parameters
  VG_PAINT_PATTERN_TILING_MODE)
;; VGPaintType
(define-foreign-enum-type (VGPaintType int VG_PAINT_TYPE_COLOR)
  (VGPaintType->int int->VGPaintType)
  VG_PAINT_TYPE_COLOR
  VG_PAINT_TYPE_LINEAR_GRADIENT
  VG_PAINT_TYPE_RADIAL_GRADIENT
  VG_PAINT_TYPE_PATTERN)
;; VGParamType
(define-foreign-enum-type (VGParamType int VG_MATRIX_MODE)
  (VGParamType->int int->VGParamType)
  ;; Mode settings
  VG_MATRIX_MODE
  VG_FILL_RULE
  VG_IMAGE_QUALITY
  VG_RENDERING_QUALITY
  VG_BLEND_MODE
  VG_IMAGE_MODE
  ;; scissoring rectangles
  VG_SCISSOR_RECTS
  ;; Color Transformation
  VG_COLOR_TRANSFORM
  VG_COLOR_TRANSFORM_VALUES
  ;; Stroke parameters
  VG_STROKE_LINE_WIDTH
  VG_STROKE_CAP_STYLE
  VG_STROKE_JOIN_STYLE
  VG_STROKE_MITER_LIMIT
  VG_STROKE_DASH_PATTERN
  VG_STROKE_DASH_PHASE
  VG_STROKE_DASH_PHASE_RESET
  ;; Edge fill color for VG_TILE_FILL tiling mode
  VG_TILE_FILL_COLOR
  ;; Color for vgClear
  VG_CLEAR_COLOR
  ;; Glyph origin
  VG_GLYPH_ORIGIN
  ;; Enable/disable masking and scissoring
  VG_MASKING
  VG_SCISSORING
  ;; Pixel layout information
  VG_PIXEL_LAYOUT
  VG_SCREEN_LAYOUT
  ;; Source format selection for image filters
  VG_FILTER_FORMAT_LINEAR
  VG_FILTER_FORMAT_PREMULTIPLIED
  ;; Destination write enable mask for image filters
  VG_FILTER_CHANNEL_MASK
  ;; Implementation limits (read-only)
  VG_MAX_SCISSOR_RECTS
  VG_MAX_DASH_COUNT
  VG_MAX_KERNEL_SIZE
  VG_MAX_SEPARABLE_KERNEL_SIZE
  VG_MAX_COLOR_RAMP_STOPS
  VG_MAX_IMAGE_WIDTH
  VG_MAX_IMAGE_HEIGHT
  VG_MAX_IMAGE_PIXELS
  VG_MAX_IMAGE_BYTES
  VG_MAX_FLOAT
  VG_MAX_GAUSSIAN_STD_DEVIATION)
;; VGPathAbsRel
(define-foreign-enum-type (VGPathAbsRel int VG_ABSOLUTE)
  (VGPathAbsRel->int int->VGPathAbsRel)
  VG_ABSOLUTE
  VG_RELATIVE)
;; VGPathCapabilities
(define-foreign-enum-type (VGPathCapabilities int VG_PATH_CAPABILITY_APPEND_FROM)
  (VGPathCapabilities->int int->VGPathCapabilities)
  VG_PATH_CAPABILITY_APPEND_FROM
  VG_PATH_CAPABILITY_APPEND_TO
  VG_PATH_CAPABILITY_MODIFY
  VG_PATH_CAPABILITY_TRANSFORM_FROM
  VG_PATH_CAPABILITY_TRANSFORM_TO
  VG_PATH_CAPABILITY_INTERPOLATE_FROM
  VG_PATH_CAPABILITY_INTERPOLATE_TO
  VG_PATH_CAPABILITY_PATH_LENGTH
  VG_PATH_CAPABILITY_POINT_ALONG_PATH
  VG_PATH_CAPABILITY_TANGENT_ALONG_PATH
  VG_PATH_CAPABILITY_PATH_BOUNDS
  VG_PATH_CAPABILITY_PATH_TRANSFORMED_BOUNDS
  VG_PATH_CAPABILITY_ALL)
;; VGPathCommand
(define-foreign-enum-type (VGPathCommand int VG_MOVE_TO_ABS)
  (VGPathCommand->int int->VGPathCommand)
  VG_MOVE_TO_ABS
  VG_MOVE_TO_REL
  VG_LINE_TO_ABS
  VG_LINE_TO_REL
  VG_HLINE_TO_ABS
  VG_HLINE_TO_REL
  VG_VLINE_TO_ABS
  VG_VLINE_TO_REL
  VG_QUAD_TO_ABS
  VG_QUAD_TO_REL
  VG_CUBIC_TO_ABS
  VG_CUBIC_TO_REL
  VG_SQUAD_TO_ABS
  VG_SQUAD_TO_REL
  VG_SCUBIC_TO_ABS
  VG_SCUBIC_TO_REL
  VG_SCCWARC_TO_ABS
  VG_SCCWARC_TO_REL
  VG_SCWARC_TO_ABS
  VG_SCWARC_TO_REL
  VG_LCCWARC_TO_ABS
  VG_LCCWARC_TO_REL
  VG_LCWARC_TO_ABS
  VG_LCWARC_TO_REL)
;; VGPathDatatype
(define-foreign-enum-type (VGPathDatatype int VG_PATH_DATATYPE_S_8)
  (VGPathDatatype->int int->VGPathDatatype)
  VG_PATH_DATATYPE_S_8
  VG_PATH_DATATYPE_S_16
  VG_PATH_DATATYPE_S_32
  VG_PATH_DATATYPE_F)

;; VGPathParamType
(define-foreign-enum-type (VGPathParamType int VG_PATH_FORMAT)
  (VGPathParamType->int int->VGPathParamType)
  VG_PATH_FORMAT
  VG_PATH_DATATYPE
  VG_PATH_SCALE
  VG_PATH_BIAS
  VG_PATH_NUM_SEGMENTS
  VG_PATH_NUM_COORDS)
;; VGPathSegment
(define-foreign-enum-type (VGPathSegment int VG_CLOSE_PATH)
  (VGPathSegment->int int->VGPathSegment)
  VG_CLOSE_PATH
  VG_MOVE_TO
  VG_LINE_TO
  VG_HLINE_TO
  VG_VLINE_TO
  VG_QUAD_TO
  VG_CUBIC_TO
  VG_SQUAD_TO
  VG_SCUBIC_TO
  VG_SCCWARC_TO
  VG_SCWARC_TO
  VG_LCCWARC_TO
  VG_LCWARC_TO)
;; VGPixelLayout
(define-foreign-enum-type (VGPixelLayout int VG_PIXEL_LAYOUT_UNKNOWN)
  (VGPixelLayout->int int->VGPixelLayout)
  VG_PIXEL_LAYOUT_UNKNOWN
  VG_PIXEL_LAYOUT_RGB_VERTICAL
  VG_PIXEL_LAYOUT_BGR_VERTICAL
  VG_PIXEL_LAYOUT_RGB_HORIZONTAL
  VG_PIXEL_LAYOUT_BGR_HORIZONTAL)
;; VGRenderingQuality
(define-foreign-enum-type (VGRenderingQuality int VG_RENDERING_QUALITY_NONANTIALIASED)
  (VGRenderingQuality->int int->VGRenderingQuality)
  VG_RENDERING_QUALITY_NONANTIALIASED
  VG_RENDERING_QUALITY_FASTER
  VG_RENDERING_QUALITY_BETTER)
;; VGStringID
(define-foreign-enum-type (VGStringID int VG_VENDOR)
  (VGStringID->int int->VGStringID)
  VG_VENDOR
  VG_RENDERER
  VG_VERSION
  VG_EXTENSIONS)
;; VGTilingMode
(define-foreign-enum-type (VGTilingMode int VG_TILE_FILL)
  (VGTilingMode->int int->VGTilingMode)
  VG_TILE_FILL
  VG_TILE_PAD
  VG_TILE_REPEAT
  VG_TILE_REFLECT)

(define vgGetError (lambda () (VGErrorCode->int (foreign-lambda VGErrorCode "vgGetError"))))
(define vgFlush (foreign-lambda void "vgFlush"))
(define vgFinish (foreign-lambda void "vgFinish"))

(define-syntax unwind-protect
  (syntax-rules ()
    ((_ body-form cleanup-forms ...)
     (dynamic-wind
       (lambda () #f)
       (lambda ()
         (condition-case (body-form)
           (else () #f)))
       (lambda () cleanup-forms ...)))))

(define vg-no-error-exception
  (make-exception "VG_NO_ERROR" 'vg-no-error))
(define vg-bad-handle-error-exception
  (make-exception "VG_BAD_HANDLE_ERROR" 'vg-bad-handle-error))
(define vg-illegal-argument-error-exception
  (make-exception "VG_ILLEGAL_ARGUMENT_ERROR"
		  'vg-illegal-argument-error))
(define vg-out-of-memory-error-exception
  (make-exception "VG_OUT_OF_MEMORY_ERROR" 'vg-out-of-memory-error))
(define vg-path-capability-error-exception
  (make-exception "VG_PATH_CAPABILITY_ERROR"
		  'vg-path-capability-error))
(define vg-unsupported-image-format-error-exception
  (make-exception "VG_UNSUPPORTED_IMAGE_FORMAT_ERROR"
		   'vg-unsupported-image-format-error))
(define vg-unsupported-path-format-error-exception
  (make-exception "VG_UNSUPPORTED_PATH_FORMAT_ERROR"
		  'vg-unsupported-path-format-error))
(define vg-image-in-use-error-exception
  (make-exception "VG_IMAGE_IN_USE_ERROR" 'vg-image-in-use-error))
(define vg-no-context-error-exception
  (make-exception "VG_NO_CONTEXT_ERROR" 'vg-no-context-error))
(define vg-undefined-error-exception 
  (make-exception "UNDEFINED_VG_ERROR" 'vg-undefined-error))

(define (vg-error-raise vg-error location arguments)
  (raise
   ((case vg-error
       ((VG_NO_ERROR) vg-no-error-exception)
       ((VG_BAD_HANDLE_ERROR) vg-bad-handle-error-exception)
       ((VG_ILLEGAL_ARGUMENT_ERROR) vg-illegal-argument-error-exception)
       ((VG_OUT_OF_MEMORY_ERROR) vg-out-of-memory-error-exception)
       ((VG_PATH_CAPABILITY_ERROR) vg-path-capability-error-exception)
       ((VG_UNSUPPORTED_IMAGE_FORMAT_ERROR)
	vg-unsupported-image-format-error-exception)
       ((VG_UNSUPPORTED_PATH_FORMAT_ERROR)
	vg-unsupported-path-format-error-exception)
       ((VG_IMAGE_IN_USE_ERROR) vg-image-in-use-error-exception)
       ((VG_NO_CONTEXT_ERROR) vg-no-context-error-exception)
       (else vg-undefined-error-exception))
    location arguments)))

(define (vg-error-string vg-error)
  (case vg-error
    ((VG_NO_ERROR) "VG_NO_ERROR")
    ((VG_BAD_HANDLE_ERROR) "VG_BAD_HANDLE_ERROR")
    ((VG_ILLEGAL_ARGUMENT_ERROR) "VG_ILLEGAL_ARGUMENT_ERROR")
    ((VG_OUT_OF_MEMORY_ERROR) "VG_OUT_OF_MEMORY_ERROR")
    ((VG_PATH_CAPABILITY_ERROR) "VG_PATH_CAPABILITY_ERROR")
    ((VG_UNSUPPORTED_IMAGE_FORMAT_ERROR)
     "VG_UNSUPPORTED_IMAGE_FORMAT_ERROR")
    ((VG_UNSUPPORTED_PATH_FORMAT_ERROR)
     "VG_UNSUPPORTED_PATH_FORMAT_ERROR")
    ((VG_IMAGE_IN_USE_ERROR) "VG_IMAGE_IN_USE_ERROR")
    ((VG_NO_CONTEXT_ERROR) "VG_NO_CONTEXT_ERROR")
    (else "VG-UNDEFINED-ERROR")))

;;; (vg-error-warn "warning" f-name f-args ...)
(define-syntax vg-error-warn
  (ir-macro-transformer
   (lambda (args inject compare)
     (let ((message (cadr args))
	   (f-name (caddr args))
	   (f-run (cddr args)))
       `(begin0
	  ,f-run
	  ,(if (vg-error-print-enable)
	       `(let ((error-value (vgGetError)))
		  (unless (eq VG_NO_ERROR (vg-error-signal error-value))
			  (fprintf (current-error-port) "~A: ~A: ~A~%" ,,f-name
				   (vg-error-string error-value) ,,message)))))))))

;;; (vg-error-warn-abort "warning" error-location error-arguments f-name f-args ...)
(define vg-error-warn-abort
  (ir-macro-transformer
   (lambda (args inject compare)
     (bind (message location arguments f-name . f-args) (cdr args)
       `(begin0
	 (,f-name ,@f-args) 
	 (let ((error-value (vgGetError)))
	   (unless (eq VG_NO_ERROR error-value)
		   ,(if (vg-error-print-enable)
			`(fprintf (current-error-port) "~A: ~A: ~A~%" ,,f-name
				  (vg-error-string error-value) ,,message))
		   (vg-error-raise error-value location arguments))))))))

;;; according to the VG 1.1 spec sec. 4.1, all functions can signal
;;; VG_OUT_OF_MEMORY_ERROR.

;;; Context Parameter Set/Get API [5.2]

;;; setters and getters should give the programmer the option whether
;;; to continue if an error is signaled. It is unlikely that such
;;; functions would cause a VG_OUT_OF_MEMORY_ERROR. The errors should
;;; be due to bad parameters. *-warn functions will print to
;;; stderr. *-abort functions will signal an error, and *-warn-abort
;;; will do both.

(define vgSetf
  (lambda (type value)
    (let ((t (int->VGParamType type)) (v value))
      ((foreign-lambda void "vgSetf" VGParamType VGfloat) t v))))

(define vgSetf-warn
  (lambda (type value)
    (vgSetf type value)
    (vg-error-warn "vgSetf returned %s\n")))

(define vgSeti 
  (lambda (type value)
    (let ((t (int->VGParamType type)) (v value))
      ((foreign-lambda void "vgSeti" VGParamType VGint) t v))))

(define vgSeti-warn)
(define vgSeti-abort)

(define vgSetfv 
  (lambda (type count values)
    (let ((t (int->VGParamType type)) (c count) (v values))
      ((foreign-lambda void "vgSetfv" VGParamType VGint (const VGfloat-vector)) t c v))))
(define vgSetiv
  (lambda (type count values)
    (let ((t (int->VGParamType type)) (c count) (v values))
      ((foreign-lambda void "vgSetiv" VGParamType VGint (const VGint-vector)) t c v))))
(define vgGetf 
  (lambda (type)
    (let ((t (int->VGParamType type))) 
      ((foreign-lambda VGfloat "vgGetf" VGParamType) t))))
(define vgGeti
  (lambda (type)
    (let ((t (int->VGParamType type)))
      ((foreign-lambda VGint "vgGeti" VGParamType) t))))
(define vgGetVectorSize 
  (lambda (type)
    (let ((ty (int->VGParamType type)))
      ((foreign-lambda VGint "vgGetVectorSize" VGParamType) ty))))
(define vgGetfv
  (lambda (type count values)
    (let ((t (int->VGParamType type)) (c count) (v values))
      ((foreign-lambda void "vgGetfv" VGParamType VGint VGfloat-vector) t c v))))
(define vgGetiv
  (lambda (type count values)
    (let ((t (int->VGParamType type)) (c count) (v values))
      ((foreign-lambda void "vgGetiv" VGParamType VGint VGint-vector) t c v))))

;;; Object Parameter Set/Get API [5.3]
(define vgSetParameterf
  (lambda (object paramType value)
    (let ((o object) (p paramType) (v value))
      ((foreign-lambda void "vgSetParameterf" VGHandle VGint VGfloat) o p v))))
(define vgSetParameteri 
  (lambda (object paramType value)
    (let ((o object) (p paramType) (v value))
      ((foreign-lambda void "vgSetParameteri" VGHandle VGint VGfloat) o p v))))
(define vgSetParameterfv 
  (lambda (object paramType count values)
    (let ((o object) (p paramType) (c count) (v values))
      ((foreign-lambda void "vgSetParameterfv" VGHandle VGint VGint (const VGfloat-vector)) o p c v))))
(define vgSetParameteriv
  (lambda (object paramType count values)
    (let ((o object) (p paramType) (c count) (v values))
      ((foreign-lambda void "vgSetParameteriv" VGHandle VGint VGint (const VGint-vector)) o p c v))))
(define vgGetParameterf
  (lambda (object paramType)
    (let ((o object) (p paramType))
      ((foreign-lambda VGfloat "vgGetParameterf" VGHandle VGint) o p))))
(define vgGetParameteri
  (lambda (object paramType)
    (let ((o object) (p paramType))
      ((foreign-lambda VGint "vgGetParameteri" VGHandle VGint) o p))))
(define vgGetParameterVectorSize
  (lambda (object paramType)
    (let ((o object) (p paramType))
      ((foreign-lambda VGint "vgGetParameterVectorSize" VGHandle VGint) o p))))
(define vgGetParameterfv
  (lambda (object paramType count values)
    (let ((o object) (p paramType) (c count) (v values))
      ((foreign-lambda void "vgGetParameterfv" VGHandle VGint VGint VGfloat-vector) o p c v))))
(define vgGetParameteriv
  (lambda (object paramType count values)
    (let ((o object) (p paramType) (c count) (v values))
      ((foreign-lambda void "vgGetParameteriv" VGHandle VGint VGint VGint-vector) o p c v))))

;;; Matrix Transformation [6.6]
(define vgLoadIdentity (foreign-lambda void "vgLoadIdentity"))
(define vgLoadMatrix (foreign-lambda void "vgLoadMatrix" (const VGfloat-vector)))
(define vgGetMatrix (foreign-lambda void "vgGetMatrix" VGfloat-vector))
(define vgMultMatrix (foreign-lambda void "vgMultMatrix" (const VGfloat-vector)))
(define vgTranslate (foreign-lambda void "vgTranslate" VGfloat VGfloat))
(define vgScale (foreign-lambda void "vgScale" VGfloat VGfloat))
(define vgShear (foreign-lambda void "vgShear" VGfloat VGfloat))
(define vgRotate (foreign-lambda void "vgRotate" VGfloat))

;;; Masking [7.2]
(define vgMask
  (lambda (mask operation x-zero y-zero width height)
    (let ((m mask) (o (int->VGMaskOperation operation)) (x x-zero) (y y-zero) (w width) (h height))
      ((foreign-lambda void "vgMask" VGHandle VGMaskOperation VGint VGint VGint VGint) m o x y w h))))
(define vgRenderToMask
  (lambda (path paintModes operation)
    (let ((p path) (pm paintModes) (o (int->VGMaskOperation operation)))
      ((foreign-lambda void "vgRenderToMask" VGPath VGbitfield VGMaskOperation) p pm o))))
(define vgCreateMaskLayer (foreign-lambda VGMaskLayer
					  "vgCreateMaskLayer" VGint
					  VGint))
(define vgDestroyMaskLayer (foreign-lambda void "vgDestroyMaskLayer"
					   VGMaskLayer))

;;; vgCreateMaskLayer-gc width height &optional destroyer
(define-syntax vgCreateMaskLayer-gc
  (ir-macro-transformer
   (lambda (args inject compare)
     (let ((width (car args))
	   (height (cadr args))
	   (body (cddr args)))
       `(let ((mask #f) (destroyer #f))
	  (dynamic-wind
	      (lambda () 
		(set! mask (vg-error-warn-abort "" 'vgCreateMaskLayer-gc 'vgCreateMaskLayer-gc
						vgCreateMaskLayer
						,width ,height))
		(set! destroyer (delay (vgDestroyMaskLayer mask))))
	      (lambda ()
		(condition-case (-> mask ,@body)
				(else () #f)))
	      (lambda () 
		(if (promise? destroyer)
		    (force destroyer)))))))))

(define vgFillMaskLayer (foreign-lambda void "vgFillMaskLayer" VGMaskLayer VGint VGint VGint VGint VGfloat))
(define vgCopyMask (foreign-lambda void "vgCopyMask" VGMaskLayer VGint VGint VGint VGint VGint VGint))

;;; Fast Clear [7.3]
(define vgClear (foreign-lambda void "vgClear" VGint VGint VGint VGint))

;;; Create and Destroy Path [8.6.2]
(define vgCreatePath
  (lambda (pathFormat datatype scale bias segmentCapacityHint coordCapacityHint capabilities)
    (let ((p pathFormat) (d (int->VGPathDatatype datatype)) (s scale) (b bias) (sch segmentCapacityHint) (cch coordCapacityHint) (c capabilities))
      ((foreign-lambda VGPath "vgCreatePath" VGint VGPathDatatype VGfloat VGfloat VGint VGint VGbitfield) p d s b sch cch c))))
(define vgClearPath (foreign-lambda void "vgClearPath" VGPath VGbitfield))
(define vgDestroyPath (foreign-lambda void "vgDestroyPath" VGPath))

;;; Query & Modify Path Capabilities [8.6.4]
(define vgGetPathCapabilities (foreign-lambda VGbitfield "vgGetPathCapabilities" VGPath))
(define vgRemovePathCapabilities (foreign-lambda void "vgRemovePathCapabilities" VGPath VGbitfield))

;;; Copy Data Between Paths [8.6.5-6]
(define vgAppendPath (foreign-lambda void "vgAppendPath" VGPath VGPath))
(define vgAppendPathData (foreign-lambda void "vgAppendPathData"
					 VGPath VGint (const VGubyte-vector) (const c-pointer)))

;;; Modify Path Data [8.6.7]
(define vgModifyPathCoords (foreign-lambda void "vgModifyPathCoords" VGPath VGint VGint (const c-pointer)))

;;; Transform Path [8.6.8]
(define vgTransformPath (foreign-lambda void "vgTransformPath" VGPath VGPath))

;;; Interpolate Between Paths [8.6.9]
(define vgInterpolatePath
  (lambda (dstPath startPath endPath amount)
    (VGboolean->int ((foreign-lambda VGboolean "vgInterpolatePath" VGPath VGPath VGPath VGfloat)
		     dstPath startPath endPath amount))))

;;; Length of Path [8.6.10]
(define vgPathLength (foreign-lambda VGfloat "vgPathLength" VGPath VGint VGint))

;;; Position & Tangent Along Path [8.6.11]
(define vgPointAlongPath (foreign-lambda void "vgPointAlongPath"
					 VGPath VGint VGint VGfloat VGfloat-vector
					 VGfloat-vector VGfloat-vector VGfloat-vector))

;;; Query Bounding Box [8.6.12]
(define vgPathBounds (foreign-lambda void "vgPathBounds"
				     VGPath VGfloat-vector VGfloat-vector VGfloat-vector VGfloat-vector))
(define vgPathTransformedBounds (foreign-lambda void "vgPathTransformedBounds"
						VGPath VGfloat-vector VGfloat-vector VGfloat-vector VGfloat-vector))

;;; Draw Path [8.8]
(define vgPathLength (foreign-lambda VGfloat "vgPathLength" VGPath VGint VGint))
(define vgDrawPath (foreign-lambda void "vgDrawPath" VGPath VGbitfield))

;;; Paint Definition [9.1]
;;; Create & Destroy Paint [9.1.1]
(define vgCreatePaint (foreign-lambda VGPaint "vgCreatePaint"))
(define vgDestroyPaint (foreign-lambda void "vgDestroyPaint" VGPaint))
;;; Set the Current Paint [9.1.2]
(define vgSetPaint (foreign-lambda void "vgSetPaint" VGPaint VGbitfield))
(define vgGetPaint
  (lambda (paintModes)
    (let ((pm (int->VGPaintMode paintModes)))
      ((foreign-lambda VGPaint "vgGetPaint" VGPaintMode) pm))))

;;;Color Paint [9.2]
(define vgSetParameterfv-paint-color
  (lambda (paint-object values)
    (vgSetParameterfv paint-object VG_PAINT_COLOR 4 values)))
(define vgSetColor (foreign-lambda void "vgSetColor" VGPaint VGuint))
(define vgGetColor (foreign-lambda VGuint "vgGetColor" VGPaint))

;;; Pattern Paint [9.4]
(define vgPaintPattern (foreign-lambda void "vgPaintPattern" VGPaint VGImage))

;;; Create & Destroy Image [10.3]
(define vgCreateImage
  (lambda (fmt width height quality)
    (let ((format (int->VGImageFormat fmt))) 
      ((foreign-lambda VGImage "vgCreateImage" VGImageFormat VGint VGint VGbitfield) 
       format width height quality))))
(define vgDestroyImage (foreign-lambda void "vgDestroyImage" VGImage))

;;; Read and Write Image Pixels [10.5]
(define vgClearImage (foreign-lambda void "vgClearImage" VGImage VGint VGint VGint VGint))
(define vgImageSubData
  (lambda (image data dataStride fmt x y width height)
    (let ((format (int->VGImageFormat fmt)))
      ((foreign-lambda void "vgImageSubData"
		       VGImage (const c-pointer) VGint VGImageFormat VGint VGint VGint VGint)
       image data dataStride format x y width height))))
(define vgGetImageSubData
  (lambda (image data dataStride fmt x y width height)
    (let ((format (int->VGImageFormat fmt)))
      ((foreign-lambda void "vgGetImageSubData"
		       VGImage c-pointer VGint VGImageFormat VGint VGint VGint VGint)
       image data dataStride format x y width height))))
;;; Child Images [10.6]
(define vgChildImage (foreign-lambda VGImage "vgChildImage" VGImage VGint VGint VGint VGint))
(define vgGetParent (foreign-lambda VGImage "vgGetParent" VGImage))
;;; Copy Between Images [10.7]
(define vgCopyImage
  (lambda (dst dx dy src sx sy width height dither)
    (let ((d (int->VGboolean dither)))
      ((foreign-lambda void "vgCopyImage"
			VGImage VGint VGint VGImage VGint VGint VGint VGint VGboolean)
       dst dx dy src sx sy width height d))))
;;; Draw Image [10.8]
(define vgDrawImage (foreign-lambda void "vgDrawImage" VGImage))
;;; Read and Write Drawing Surface Pixels [10.9]
(define vgSetPixels (foreign-lambda void "vgSetPixels" VGint VGint VGImage VGint VGint VGint VGint))
(define vgWritePixels 
  (lambda (data dataStride fmt dx dy width height)
    (let ((format (int->VGImageFormat fmt)))
      ((foreign-lambda void "vgWritePixels"
		       (const c-pointer) VGint VGImageFormat VGint VGint VGint VGint)
       data dataStride format dx dy width height))))
(define vgGetPixels (foreign-lambda void "vgGetPixels"
				    VGImage VGint VGint VGint VGint VGint VGint))
(define vgReadPixels
  (lambda (data dataStride fmt sx sy width height)
    (let ((format (int->VGImageFormat fmt)))
      ((foreign-lambda void "vgReadPixels" c-pointer VGint VGImageFormat VGint VGint VGint VGint)
       data dataStride format sx sy width height))))
(define vgCopyPixel (foreign-lambda void "vgCopyPixel" VGint VGint VGint VGint VGint VGint))

;;; Font Definition [11.4]
;;; Manage VGFont Object [11.4.2]
(define vgCreateFont (foreign-lambda VGFont "vgCreateFont" VGint))
(define vgDestroyFont (foreign-lambda void "vgDestroyFont" VGFont))

;;; Add/Modify Glyphs in Fonts [11.4.4]
(define vgSetGlyphToPath
  (lambda (font glyphIndex path inHinted origin escape)
    (let ((hint (int->VGboolean inHinted)))
      ((foreign-lambda void "vgSetGlyphToPath"
			VGFont VGuint VGPath VGboolean
			(const VGfloat-vector) (const VGfloat-vector))
       font glyphIndex path hint origin escape))))
(define vgSetGlyphToImage (foreign-lambda void "vgSetGlyphToImage"
					  VGFont VGuint VGImage (const VGfloat-vector) (const VGfloat-vector)))
(define vgClearGlyph (foreign-lambda void "vgClearGlyph" VGFont VGuint))

;;; Draw Text [11.5]
(define vgDrawGlyph
  (lambda (font glyphIndex paintModes allowAutoHinting)
    (let ((a (int->VGboolean allowAutoHinting)))
      ((foreign-lambda void "vgDrawGlyph" VGFont VGuint VGbitfield VGboolean)
       font glyphIndex paintModes a))))

(define vgDrawGlyphs
  (lambda (font glyphCount glyphIndices adjustments_x adjustments_y paintModes allowAutoHinting)
    (let ((a (int->VGboolean allowAutoHinting)))
      ((foreign-lambda void "vgDrawGlyphs"
			VGFont VGint (const VGuint-vector) (const VGfloat-vector)
			(const VGfloat-vector) VGbitfield VGboolean)
       font glyphCount glyphIndices adjustments_x adjustments_y paintModes a))))

;;; Color Combination [12.3]
(define vgColorMatrix (foreign-lambda void "vgColorMatrix" VGImage VGImage (const VGfloat-vector)))

;;; Convolution [12.4]
(define vgConvolve
  (lambda (dst src kernelW KernelH shiftX shiftY kernel scale bias tilingMode)
    (let ((tm (int->VGTilingMode tilingMode)))
      ((foreign-lambda void "vgConvolve"
		       VGImage VGImage VGint VGint VGint VGint (const VGshort-vector)
		       VGfloat VGfloat VGTilingMode)
       dst src kernelW KernelH shiftX shiftY kernel scale bias tm))))
(define vgSeparableConvolve
  (lambda (dst src kernelW KernelH shiftX shiftY kernelX kernelY scale bias tilingMode)
    (let ((tm (int->VGTilingMode tilingMode)))
      ((foreign-lambda void "vgSeparableConvolve"
		       VGImage VGImage VGint VGint VGint VGint (const VGshort-vector)
		       (const VGshort-vector) VGfloat VGfloat VGTilingMode) 
       dst src kernelW KernelH shiftX shiftY kernelX kernelY scale bias tm))))
(define vgGaussianBlur
  (lambda (dst src stdDevX stdDevY tilingMode)
    (let ((tm (int->VGTilingMode tilingMode)))
      ((foreign-lambda void "vgGaussianBlur" VGImage VGImage VGfloat VGfloat VGTilingMode)
       dst src stdDevX stdDevY tm))))

;;; Lookup Table [12.5]

(define vgLookup 
  (lambda (dst src redLUT greenLUT blueLUT alphaLUT outputLinear outputPremultiplied)
    (let ((ol (int->VGboolean outputLinear)) (op (int->VGboolean outputPremultiplied)))
      ((foreign-lambda void "vgLookup" VGImage VGImage (const VGubyte-vector) (const VGubyte-vector)
		       (const VGubyte-vector) (const VGubyte-vector) VGboolean VGboolean)
       dst src redLUT greenLUT blueLUT alphaLUT ol op))))
(define vgLookupSingle
  (lambda (dst src LUT sourceChannel outputLinear outputPremultiplied)
    (let ((sc (int->VGImageChannel sourceChannel))
	  (ol (int->VGboolean outputLinear)) (op (int->VGboolean outputPremultiplied)))
      ((foreign-lambda void "vgLookupSingle"
			VGImage VGImage (const VGuint-vector) VGImageChannel VGboolean VGboolean)
       dst src LUT sc ol op))))
