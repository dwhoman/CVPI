;;; ffi for vgu
;#>
;#ifndef _VGU_H
;#include "vgu.h"
;#endif

;#ifndef _OPENVG_H
;#include "openvg.h"
;#endif
;<#
(foreign-declare "#include <VG/openvg.h>")
(foreign-declare "#include <VG/vgu.h>")
;;; Constants
(define-foreign-variable VGU_VERSION_1_0 (const int) "VGU_VERSION_1_0")
(define-foreign-variable VGU_VERSION_1_1 (const int) "VGU_VERSION_1_1")

;; VGUArcType
(define-foreign-enum-type (VGUArcType int VGU_ARC_OPEN)
  (VGUArcType->int int->VGUArcType)
  VGU_ARC_OPEN
  VGU_ARC_CHORD
  VGU_ARC_PIE)
;; VGUErrorCode
(define-foreign-enum-type (VGUErrorCode int VGU_NO_ERROR)
  (VGUErrorCode->int int->VGUErrorCode)
  VGU_NO_ERROR
  VGU_BAD_HANDLE_ERROR
  VGU_ILLEGAL_ARGUMENT_ERROR
  VGU_OUT_OF_MEMORY_ERROR
  VGU_PATH_CAPABILITY_ERROR
  VGU_BAD_WARP_ERROR)

(define vguLine 
  (lambda (path x0 y0 x1 y1)
    (VGUErrorCode->int ((foreign-lambda VGUErrorCode "vguLine" VGPath VGfloat VGfloat VGfloat VGfloat)
			path x0 y0 x1 y1))))

(define vguPolygon
  (lambda (path points count closed)
    (VGUErrorCode->int ((foreign-lambda VGUErrorCode "vguPolygon" 
					VGPath (const VGfloat-vector) VGint VGboolean)
			path points count closed))))

(define vguRect
  (lambda (path x y width height)
    (VGUErrorCode->int ((foreign-lambda VGUErrorCode "vguRect"
					VGPath VGfloat VGfloat VGfloat VGfloat)
			path x y width height))))

(define vguRoundRect
  (lambda (path x y width height arcWidth arcHeight)
    (VGUErrorCode->int ((foreign-lambda VGUErrorCode "vguRoundRect" VGPath VGfloat VGfloat VGfloat VGfloat VGfloat VGfloat)
			path x y width height arcWidth arcHeight))))

(define vguEllipse
  (lambda (path cx cy width height)
    (VGUErrorCode->int ((foreign-lambda VGUErrorCode "vguEllipse" VGPath VGfloat VGfloat VGfloat VGfloat)
			path cx cy width height)))) 

(define vguArc
  (lambda (path x y width height startAngle angleExtent arcType)
    (let ((a (int->VGUArcType arcType)))
      (VGUErrorCode->int ((foreign-lambda VGUErrorCode "vguArc" VGPath VGfloat VGfloat VGfloat VGfloat VGfloat VGfloat VGUArcType)
			  path x y width height startAngle angleExtent a))))) 

(define vguComputeWarpQuadToSquare
  (lambda (sx0 sy0 sx1 sy1 sx2 sy2 sx3 sy3 matrix)
    (VGUErrorCode->int ((foreign-lambda VGUErrorCode "vguComputeWarpQuadToSquare" 
					VGfloat VGfloat VGfloat VGfloat VGfloat VGfloat VGfloat VGfloat VGfloat-vector)
			sx0 sy0 sx1 sy1 sx2 sy2 sx3 sy3 matrix)))) 

(define vguComputeWarpSquareToQuad
  (lambda (dx0 dy0 dx1 dy1 dx2 dy2 dx3 dy3 matrix)
    (VGUErrorCode->int ((foreign-lambda VGUErrorCode  "vguComputeWarpSquareToQuad"
					VGfloat VGfloat VGfloat VGfloat VGfloat VGfloat VGfloat VGfloat VGfloat-vector)
			dx0 dy0 dx1 dy1 dx2 dy2 dx3 dy3 matrix)))) 

(define vguComputeWarpQuadToQuad
  (lambda (dx0 dy0 dx1 dy1 dx2 dy2 dx3 dy3 sx0 sy0 sx1 sy1 sx2 sy2 sx3 sy3 matrix)
    (VGUErrorCode->int ((foreign-lambda VGUErrorCode "vguComputeWarpQuadToQuad"
					VGfloat VGfloat 
					VGfloat VGfloat
					VGfloat VGfloat 
					VGfloat VGfloat 
					VGfloat VGfloat 
					VGfloat VGfloat 
					VGfloat VGfloat 
					VGfloat VGfloat 
					VGfloat-vector)
			dx0 dy0 dx1 dy1 dx2 dy2 dx3 dy3 sx0 sy0 sx1 sy1 sx2 sy2 sx3 sy3 matrix))))
