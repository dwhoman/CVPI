#ifndef __LINUX_VIDEODEV2_H
#include <linux/videodev2.h>
#endif

int main() {
  printf("V4L2_PIX_FMT_RGB332 %d\n", V4L2_PIX_FMT_RGB332);
  printf("V4L2_PIX_FMT_RGB444 %d\n", V4L2_PIX_FMT_RGB444);
  printf("V4L2_PIX_FMT_RGB555 %d\n", V4L2_PIX_FMT_RGB555);
  printf("V4L2_PIX_FMT_RGB565 %d\n", V4L2_PIX_FMT_RGB565);
  printf("V4L2_PIX_FMT_RGB555X %d\n", V4L2_PIX_FMT_RGB555X);
  printf("V4L2_PIX_FMT_RGB565X %d\n", V4L2_PIX_FMT_RGB565X);
  printf("V4L2_PIX_FMT_BGR666 %d\n", V4L2_PIX_FMT_BGR666);
  printf("V4L2_PIX_FMT_BGR24 %d\n", V4L2_PIX_FMT_BGR24);
  printf("V4L2_PIX_FMT_RGB24 %d\n", V4L2_PIX_FMT_RGB24);
  printf("V4L2_PIX_FMT_BGR32 %d\n", V4L2_PIX_FMT_BGR32);
  printf("V4L2_PIX_FMT_RGB32 %d\n", V4L2_PIX_FMT_RGB32);

/*Grey formats */
  printf("V4L2_PIX_FMT_GREY %d\n", V4L2_PIX_FMT_GREY);
  printf("V4L2_PIX_FMT_Y4 %d\n", V4L2_PIX_FMT_Y4);
  printf("V4L2_PIX_FMT_Y6 %d\n", V4L2_PIX_FMT_Y6);
  printf("V4L2_PIX_FMT_Y10 %d\n", V4L2_PIX_FMT_Y10);
  printf("V4L2_PIX_FMT_Y12 %d\n", V4L2_PIX_FMT_Y12);
  printf("V4L2_PIX_FMT_Y16 %d\n", V4L2_PIX_FMT_Y16);

/* Grey bit-packed formats */
  printf("V4L2_PIX_FMT_Y10BPACK %d\n", V4L2_PIX_FMT_Y10BPACK);

/* Palette formats */
  printf("V4L2_PIX_FMT_PAL8 %d\n", V4L2_PIX_FMT_PAL8);

/* Luminance+Chrominance formats */
  printf("V4L2_PIX_FMT_YVU410 %d\n", V4L2_PIX_FMT_YVU410);
  printf("V4L2_PIX_FMT_YVU420 %d\n", V4L2_PIX_FMT_YVU420);
  printf("V4L2_PIX_FMT_YUYV %d\n", V4L2_PIX_FMT_YUYV);
  printf("V4L2_PIX_FMT_YYUV %d\n", V4L2_PIX_FMT_YYUV);
  printf("V4L2_PIX_FMT_YVYU %d\n", V4L2_PIX_FMT_YVYU);
  printf("V4L2_PIX_FMT_UYVY %d\n", V4L2_PIX_FMT_UYVY);
  printf("V4L2_PIX_FMT_VYUY %d\n", V4L2_PIX_FMT_VYUY);
  printf("V4L2_PIX_FMT_YUV422P %d\n", V4L2_PIX_FMT_YUV422P);
  printf("V4L2_PIX_FMT_YUV411P %d\n", V4L2_PIX_FMT_YUV411P);
  printf("V4L2_PIX_FMT_Y41P %d\n", V4L2_PIX_FMT_Y41P);
  printf("V4L2_PIX_FMT_YUV444 %d\n", V4L2_PIX_FMT_YUV444);
  printf("V4L2_PIX_FMT_YUV555 %d\n", V4L2_PIX_FMT_YUV555);
  printf("V4L2_PIX_FMT_YUV565 %d\n", V4L2_PIX_FMT_YUV565);
  printf("V4L2_PIX_FMT_YUV32 %d\n", V4L2_PIX_FMT_YUV32);
  printf("V4L2_PIX_FMT_YUV410 %d\n", V4L2_PIX_FMT_YUV410);
  printf("V4L2_PIX_FMT_YUV420 %d\n", V4L2_PIX_FMT_YUV420);
  printf("V4L2_PIX_FMT_HI240 %d\n", V4L2_PIX_FMT_HI240);
  printf("V4L2_PIX_FMT_HM12 %d\n", V4L2_PIX_FMT_HM12);
  printf("V4L2_PIX_FMT_M420 %d\n", V4L2_PIX_FMT_M420);

/* two planes -- one Y, one Cr + Cb interleaved  */
  printf("V4L2_PIX_FMT_NV12 %d\n", V4L2_PIX_FMT_NV12);
  printf("V4L2_PIX_FMT_NV21 %d\n", V4L2_PIX_FMT_NV21);
  printf("V4L2_PIX_FMT_NV16 %d\n", V4L2_PIX_FMT_NV16);
  printf("V4L2_PIX_FMT_NV61 %d\n", V4L2_PIX_FMT_NV61);

/* two non contiguous planes - one Y, one Cr + Cb interleaved  */
  printf("V4L2_PIX_FMT_NV12M %d\n", V4L2_PIX_FMT_NV12M);
  printf("V4L2_PIX_FMT_NV12MT %d\n", V4L2_PIX_FMT_NV12MT);

/* three non contiguous planes - Y, Cb, Cr */
  printf("V4L2_PIX_FMT_YUV420M %d\n", V4L2_PIX_FMT_YUV420M);

/* Bayer formats - see http://www.siliconimaging.com/RGB%20Bayer.htm */
  printf("V4L2_PIX_FMT_SBGGR8 %d\n", V4L2_PIX_FMT_SBGGR8);
  printf("V4L2_PIX_FMT_SGBRG8 %d\n", V4L2_PIX_FMT_SGBRG8);
  printf("V4L2_PIX_FMT_SGRBG8 %d\n", V4L2_PIX_FMT_SGRBG8);
  printf("V4L2_PIX_FMT_SRGGB8 %d\n", V4L2_PIX_FMT_SRGGB8);
  printf("V4L2_PIX_FMT_SBGGR10 %d\n", V4L2_PIX_FMT_SBGGR10);
  printf("V4L2_PIX_FMT_SGBRG10 %d\n", V4L2_PIX_FMT_SGBRG10);
  printf("V4L2_PIX_FMT_SGRBG10 %d\n", V4L2_PIX_FMT_SGRBG10);
  printf("V4L2_PIX_FMT_SRGGB10 %d\n", V4L2_PIX_FMT_SRGGB10);
  printf("V4L2_PIX_FMT_SBGGR12 %d\n", V4L2_PIX_FMT_SBGGR12);
  printf("V4L2_PIX_FMT_SGBRG12 %d\n", V4L2_PIX_FMT_SGBRG12);
  printf("V4L2_PIX_FMT_SGRBG12 %d\n", V4L2_PIX_FMT_SGRBG12);
  printf("V4L2_PIX_FMT_SRGGB12 %d\n", V4L2_PIX_FMT_SRGGB12);
	/* 10bit raw bayer DPCM compressed to 8 bits */
  printf("V4L2_PIX_FMT_SGRBG10DPCM8 %d\n", V4L2_PIX_FMT_SGRBG10DPCM8);
	/*
	 * 10bit raw bayer, expanded to 16 bits
	 * xxxxrrrrrrrrrrxxxxgggggggggg xxxxggggggggggxxxxbbbbbbbbbb...
	 */
  printf("V4L2_PIX_FMT_SBGGR16 %d\n", V4L2_PIX_FMT_SBGGR16);

/* compressed formats */
  printf("V4L2_PIX_FMT_MJPEG %d\n", V4L2_PIX_FMT_MJPEG);
  printf("V4L2_PIX_FMT_JPEG %d\n", V4L2_PIX_FMT_JPEG);
  printf("V4L2_PIX_FMT_DV %d\n", V4L2_PIX_FMT_DV);
  printf("V4L2_PIX_FMT_MPEG %d\n", V4L2_PIX_FMT_MPEG);
  printf("V4L2_PIX_FMT_H264 %d\n", V4L2_PIX_FMT_H264);
  printf("V4L2_PIX_FMT_H264_NO_SC %d\n", V4L2_PIX_FMT_H264_NO_SC);
  printf("V4L2_PIX_FMT_H263 %d\n", V4L2_PIX_FMT_H263);
  printf("V4L2_PIX_FMT_MPEG1 %d\n", V4L2_PIX_FMT_MPEG1);
  printf("V4L2_PIX_FMT_MPEG2 %d\n", V4L2_PIX_FMT_MPEG2);
  printf("V4L2_PIX_FMT_MPEG4 %d\n", V4L2_PIX_FMT_MPEG4);
  printf("V4L2_PIX_FMT_XVID %d\n", V4L2_PIX_FMT_XVID);
  printf("V4L2_PIX_FMT_VC1_ANNEX_G %d\n", V4L2_PIX_FMT_VC1_ANNEX_G);
  printf("V4L2_PIX_FMT_VC1_ANNEX_L %d\n", V4L2_PIX_FMT_VC1_ANNEX_L);

/*  Vendor-specific formats   */
  printf("V4L2_PIX_FMT_CPIA1 %d\n", V4L2_PIX_FMT_CPIA1);
  printf("V4L2_PIX_FMT_WNVA %d\n", V4L2_PIX_FMT_WNVA);
  printf("V4L2_PIX_FMT_SN9C10X %d\n", V4L2_PIX_FMT_SN9C10X);
  printf("V4L2_PIX_FMT_SN9C20X_I420 %d\n", V4L2_PIX_FMT_SN9C20X_I420);
  printf("V4L2_PIX_FMT_PWC1 %d\n", V4L2_PIX_FMT_PWC1);
  printf("V4L2_PIX_FMT_PWC2 %d\n", V4L2_PIX_FMT_PWC2);
  printf("V4L2_PIX_FMT_ET61X251 %d\n", V4L2_PIX_FMT_ET61X251);
  printf("V4L2_PIX_FMT_SPCA501 %d\n", V4L2_PIX_FMT_SPCA501);
  printf("V4L2_PIX_FMT_SPCA505 %d\n", V4L2_PIX_FMT_SPCA505);
  printf("V4L2_PIX_FMT_SPCA508 %d\n", V4L2_PIX_FMT_SPCA508);
  printf("V4L2_PIX_FMT_SPCA561 %d\n", V4L2_PIX_FMT_SPCA561);
  printf("V4L2_PIX_FMT_PAC207 %d\n", V4L2_PIX_FMT_PAC207);
  printf("V4L2_PIX_FMT_MR97310A %d\n", V4L2_PIX_FMT_MR97310A);
  printf("V4L2_PIX_FMT_SN9C2028 %d\n", V4L2_PIX_FMT_SN9C2028);
  printf("V4L2_PIX_FMT_SQ905C %d\n", V4L2_PIX_FMT_SQ905C);
  printf("V4L2_PIX_FMT_PJPG %d\n", V4L2_PIX_FMT_PJPG);
  printf("V4L2_PIX_FMT_OV511 %d\n", V4L2_PIX_FMT_OV511);
  printf("V4L2_PIX_FMT_OV518 %d\n", V4L2_PIX_FMT_OV518);
  printf("V4L2_PIX_FMT_STV0680 %d\n", V4L2_PIX_FMT_STV0680);
  printf("V4L2_PIX_FMT_TM6000 %d\n", V4L2_PIX_FMT_TM6000);
  printf("V4L2_PIX_FMT_CIT_YYVYUY %d\n", V4L2_PIX_FMT_CIT_YYVYUY);
  printf("V4L2_PIX_FMT_KONICA420 %d\n", V4L2_PIX_FMT_KONICA420);
  printf("V4L2_PIX_FMT_JPGL %d\n", V4L2_PIX_FMT_JPGL);
  printf("V4L2_PIX_FMT_SE401 %d\n", V4L2_PIX_FMT_SE401);

  return 0;
}
