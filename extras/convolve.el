;;; convolve.el --- convolution for emacs calc

;; Copyright (C) 2015 Devin Homan. Devin Homan

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
  "Convolve INPUT with KERNEL. If TILE is a number, then the
input image will be padded with that number; else, padding will
be done by clamping.  That is, for input values that lie outside
the bounds of the input, the closest edge value of the input is
used. Use `convrep' and `convref' to do convolution with repeated
tiling and reflected tiling respectively. The kernel and image
origins are at the kernel and image centers, rather than in the
upper left corner. See the Kronos OpenVG version 1.1 manual, page
133 or details."
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
				 (let ((image-x
					(- (+ x i) (ceil (/ kernel-width 2)))
						)
				       (image-y
					(- (+ y j) (ceil (/ kernel-height 2)))
						))
				   (if (and (>= image-x 1)
					    (<= image-x input-width)
					    (>= image-y 1)
					    (<= image-y input-height))
				       (subscr input image-y image-x)
				     (cond
				      ((equal tile "repeat")
				       (subscr input
				      	       (+ 1 (mod (- image-y 1) input-height))
				      	       (+ 1 (mod (- image-x 1) input-width))))
				      ((equal tile "reflect")
				       (subscr input
				      	       (if (evenp (floor (/ (- image-y 1) input-height)))
				      		   (+ 1 (mod (- image-y 1) input-height))
						 (- input-height
						    (mod (- image-y 1) input-height)))
					       (if (evenp (floor (/ (- image-x 1) input-width)))
				      		   (+ 1 (mod (- image-x 1) input-width))
						 (- input-width
						    (mod (- image-x 1) input-width)))))
				      ((numberp tile) tile) ;fill
				      (t ;default pad
				       (subscr input
					       (max 1 (min image-y input-height))
					       (max 1 (min image-x input-width)))))))))))
		  (setq output-row (rcons output-row sum))))
	   (setq output (rcons output output-row))))
    output))

(defmath convrep (input kernel)
  "Convolve INPUT with KERNEL. Tiling is done by repeating the
INPUT. See the Calc function `convolve'."
  (interactive 2 "convrep")
  (convolve input kernel "repeat"))

(defmath convref (input kernel)
  "Convolve INPUT with KERNEL. Tiling is done by reflecting the
INPUT. See the Calc function `convolve'."
  (interactive 2 "convref")
  (convolve input kernel "reflect"))