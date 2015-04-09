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

(import chicken scheme foreign foreigners)
(require openvg)
(require cvpi)
(let ((settings (cvpi_egl_settings_create)))
  (if (zero? settings)
      (printf "Is zero~N")
      (printf "Is not zero~N"))
  (free settings))

(let ((destroyer #f))
  (->> (vgCreateImage width height destroyer) (doSomethingWithIt) (force destroyer)))
(let ((destroyer #f))
  ((force destroyer) (doSomethingWithIt (vgCreateImage width height destroyer))))

(with-promises ((destroyImage1 image1 (createImage))
		(destroyImage2 image2 (createImage))
		(destroyImage3 image3))
	       (modImge image1)
	       (set!-values (destroyImage3 image3) (createImage))
	       (force destroyImage1))
