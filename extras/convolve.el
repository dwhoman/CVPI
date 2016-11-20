;;; convolve.el --- convolution for emacs calc

;; Copyright 2017 Devin Homan
;;
;; Licensed under the Apache License, Version 2.0 (the "License");
;; you may not use this file except in compliance with the License.
;; You may obtain a copy of the License at
;;
;;     http://www.apache.org/licenses/LICENSE-2.0
;;
;; Unless required by applicable law or agreed to in writing, software
;; distributed under the License is distributed on an "AS IS" BASIS,
;; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
;; See the License for the specific language governing permissions and
;; limitations under the License.

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
