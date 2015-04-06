;;; ffi for vgu
;#>
;#ifndef _VGU_H
;#include "vgu.h"
;#endif

;#ifndef _OPENVG_H
;#include "openvg.h"
;#endif
;<#
(module vgu *
	(import chicken scheme foreign foreigners)
	(include "../openvg/openvg.scm")
	(include "vgu.scm"))
