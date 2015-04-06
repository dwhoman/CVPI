;(define debug? (make-parameter #t))
;;; ffi for openvg
;#>
;#ifndef _OPENVG_H
;#include "openvg.h"
;#endif
;<#

(module openvg *
	(import chicken)
	(import scheme)
	(import foreign)
	(import foreigners)

	(include "openvg.scm"))
