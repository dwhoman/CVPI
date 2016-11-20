;; This file is part of CVPI.
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
