namespace vg {
  class VGError : public exception {
  private:
    VGErrorCode ec_;

    char* error_string(VGErrorCode error) {
      switch(error) {
      case VG_NO_ERROR:
	return " VG_NO_ERROR ";
      case VG_BAD_HANDLE_ERROR:
	return " VG_BAD_HANDLE_ERROR ";
      case VG_ILLEGAL_ARGUMENT_ERROR:
	return " VG_ILLEGAL_ARGUMENT_ERROR ";
      case VG_OUT_OF_MEMORY_ERROR:
	return " VG_OUT_OF_MEMORY_ERROR ";
      case VG_PATH_CAPABILITY_ERROR:
	return " VG_PATH_CAPABILITY_ERROR ";
      case VG_UNSUPPORTED_IMAGE_FORMAT_ERROR:
	return " VG_UNSUPPORTED_IMAGE_FORMAT_ERROR ";
      case VG_UNSUPPORTED_PATH_FORMAT_ERROR:
	return " VG_UNSUPPORTED_PATH_FORMAT_ERROR ";
      case VG_IMAGE_IN_USE_ERROR:
	return " VG_IMAGE_IN_USE_ERROR ";
      case VG_NO_CONTEXT_ERROR:
	return " VG_NO_CONTEXT_ERROR ";
      default:
	return " undefined error ";
      }
    }
  public:
    VGError(VGErrorCode ec):ec_(ec){}

    const char* what() const throw() {
      return error_string(ec);
  };
  
  class MaskLayer {
  public:
    MaskLayer() {
    }
    
  };

  class Path {
  };

  class Paint {
  };

  class Image {
  private:
    VGImage image;
    Image() : image(NULL) {};
    void error_check() {
      VGErrorCode error = vgGetError(void);
      if(error != VG_NO_ERROR) {
	throw VGError(error);
      }
    }
  public:
    Image(VGImageFormat fmt, VGint width, VGint height, VGbitfield quality) { 
      createImage(fmt,width,height,quality);
    };
    ~Image() {
      destroyImage();
    };
    
    void createImage(VGImageFormat fmt, VGint width, VGint height, VGbitfield quality) {
      if(image == NULL) {
	image = vgCreateImage(fmt,width,height,quality);
	error_check();
      }
    }

    void destroyImage(void) {
      if(image != NULL) {
	vgDestroyImage(image);
	image = NULL;
	error_check();
      }
    }

    void clearImage(VGint x, VGint y, VGint width, VGint height) {
      void vgClearImage(image, x, y, width, height);
      error_check();
    }

    void imageSubData(const void * data, VGint dataStride,
		      VGImageFormat fmt, VGint x, VGint y, VGint width, VGint height) {
      vgImageSubData(image, data, dataStride, fmt, x, y, width, height);
      error_check();
    }
    
    void getImageSubData(void * data, VGint dataStride, VGImageFormat fmt, VGint x,
			 VGint y, VGint width, VGint height) {
      vgGetImageSubData(image, data, dataStride, fmt, x, y, width, height);
      error_check();
    }

  };

  class Font {
  };
}
