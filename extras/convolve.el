;;; convolve.el --- convolution for emacs calc

;; Copyright (C) 2015 Devin Homan.

;; Authors: Devin Homan

;; This file is not currently part of GNU Emacs.

;; This program is free software: you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation, either version 3 of the License, or
;; (at your option) any later version.

;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GNU Emacs. If not, see <http://www.gnu.org/licenses/>.

(defmath convolve (input kernel &optional tile)
  "Convolve input with kernel. If TILE is a number, then the
input image will be padded with that number; else, padding will
be done by clamping.  That is, for input values that lie outside
the bounds of the input, the closest edge value of the input is
used."
  (interactive 2 "conv")
  (when (not (and
	      (matrixp input)
	      (matrixp kernel)))
    (error "Input and kernel must both be vectors."))
  (let* ((input-dimens (mat-dimens input))
	 (input-height (car input-dimens))
	 (input-width (cadr input-dimens))
	 (kernel-dimens (mat-dimens kernel))
	 (kernel-height (car kernel-dimens))
	 (kernel-width (cadr kernel-dimens))
	 (output '(vec)))
    (for ((y 1 input-height))
	 (let ((output-row '(vec)))
	   (for ((x 1 input-width))
		(let ((sum 0))
		  (for ((j 1 kernel-height) (i 1 kernel-width))
		       (setq sum
			     (+ sum
				(*
				 (subscr kernel j i)
				 (let ((image-x ;; (- (+ i x) 1)
					(- (+ x i) (ceil (/ kernel-width 2)))
						)
				       (image-y ;; (- (+ j y) 1)
					(- (+ y j) (ceil (/ kernel-height 2)))
						))
				   (if (and (>= image-x 1)
					    (<= image-x input-width)
					    (>= image-y 1)
					    (<= image-y input-height))
				       (subscr input image-y image-x)
				     (cond ;TODO, other two modes supported by OpenVG
				      ;; ((equal tile "repeat")
				      ;;  (subscr input
				      ;; 	       (mod image-y input-height)
				      ;; 	       (mod image-x input-width)))
				      ;; ((equal tile "reflect")
				      ;;  (subscr input
				      ;; 	       (if (evenp (floor (/ image-y input-height)))
				      ;; 		   (mod image-y input-height)
				      ;; 		 (- input-height 1
				      ;; 		    (mod image-y input-height)))
				      ;; 	       (if (evenp (floor (/ image-x input-width)))
				      ;; 		   (mod image-x input-width)
				      ;; 		 (- input-width 1
				      ;; 		  (mod image-x input-width)))))
				      ((numberp tile) tile) ;fill
				      (t ;default pad
				       (subscr input
					       (max 1 (min image-y input-height))
					       (max 1 (min image-x input-width)))))))))))
		  (setq output-row (rcons output-row sum))))
	   (setq output (rcons output output-row))))
    output))
