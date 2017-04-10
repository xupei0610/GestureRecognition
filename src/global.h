#ifndef GLOBAL_H
#define GLOBAL_H
/**
 * @file
 * @author Pei Xu, xupei0610 at gmail.com
 * @brief The global.h file defines the default paramters to initialize the system.
 */

#ifndef CAMERA_FPS
/**
 * @brief CAMERA_FPS is the FPS of the camera
 */
#define CAMERA_FPS 50
#endif

#ifndef DEFAULT_ROI_MARGIN_LEFT
/**
 * @brief DEFAULT_ROI_MARGIN_LEFT is the default left margin, in percent, of the region of interesting on the frame captured by the camera.
 */
#define DEFAULT_ROI_MARGIN_LEFT 50
#endif
#ifndef DEFAULT_ROI_MARGIN_RIGHT
/**
 * @brief DEFAULT_ROI_MARGIN_RIGHT is the default right margin, in percent, of the region of interesting on the frame captured by the camera.
 */
#define DEFAULT_ROI_MARGIN_RIGHT 50
#endif
#ifndef DEFAULT_ROI_MARGIN_TOP
/**
 * @brief DEFAULT_ROI_MARGIN_TOP is the default top margin, in percent, of the region of interesting on the frame captured by the camera.
 */
#define DEFAULT_ROI_MARGIN_TOP 10
#endif
#ifndef DEFAULT_ROI_MARGIN_BOTTOM
/**
 * @brief DEFAULT_ROI_MARGIN_BOTTOM is the default bottom margin, in percent, of the region of interesting on the frame captured by the camera.
 */
#define DEFAULT_ROI_MARGIN_BOTTOM 150
#endif
#ifndef DEFAULT_ROI_START_X
/**
 * @brief DEFAULT_ROI_START_X is the default left bound, in percent, of the region of interesting on the frame captured by the camera.
 */
#define DEFAULT_ROI_START_X 48
#endif
#ifndef DEFAULT_ROI_END_Y
/**
 * @brief DEFAULT_ROI_END_X is the default right bound, in percent, of the region of interesting on the frame captured by the camera.
 */
#define DEFAULT_ROI_END_X 98
#endif
#ifndef DEFAULT_ROI_START_Y
/**
 * @brief DEFAULT_ROI_START_Y is the default top bound, in percent, of the region of interesting on the frame captured by the camera.
 */
#define DEFAULT_ROI_START_Y 2
#endif
#ifndef DEFAULT_ROI_END_Y
/**
 * @brief DEFAULT_ROI_END_Y is the default bottom bound, in percent, of the region of interesting on the frame captured by the camera.
 */
#define DEFAULT_ROI_END_Y 68
#endif

#ifndef DEFAULT_SKIN_COLOR_MIN_H
/**
  * @brief DEFAULT_SKIN_COLOR_MIN_H is the default lower bound of the skin color filter for hue in HSV color space.
  */
#define DEFAULT_SKIN_COLOR_MIN_H 30
#endif
#ifndef DEFAULT_SKIN_COLOR_MAX_H
/**
  * @brief DEFAULT_SKIN_COLOR_MAX_H is the default upper bound of the skin color filter for hue in HSV color space.
  */
#define DEFAULT_SKIN_COLOR_MAX_H 180
#endif
#ifndef DEFAULT_SKIN_COLOR_MIN_S
/**
  * @brief DEFAULT_SKIN_COLOR_MIN_S is the default lower bound of the skin color filter for saturation in HSV color space.
  */
#define DEFAULT_SKIN_COLOR_MIN_S 0
#endif
#ifndef DEFAULT_SKIN_COLOR_MAX_S
/**
  * @brief DEFAULT_SKIN_COLOR_MAX_S is the default upper bound of the skin color filter for saturation in HSV color space.
  */
#define DEFAULT_SKIN_COLOR_MAX_S 255
#endif
#ifndef DEFAULT_SKIN_COLOR_MIN_V
/**
  * @brief DEFAULT_SKIN_COLOR_MIN_V is the default lower bound of the skin color filter for value in HSV color space.
  */
#define DEFAULT_SKIN_COLOR_MIN_V 100
#endif
#ifndef DEFAULT_SKIN_COLOR_MAX_V
/**
  * @brief DEFAULT_SKIN_COLOR_MAX_V is the default upper bound of the skin color filter for value in HSV color space.
  */
#define DEFAULT_SKIN_COLOR_MAX_V 255
#endif

#ifndef DEFAULT_SKIN_DETECTION_AREA
/**
 * @brief DEFAULT_SKIN_DETECTION_AREA is the default minimum area of a hand contour who is considered as a hand.
 */
#define DEFAULT_SKIN_DETECTION_AREA 5000
#endif
#ifndef DEFAULT_SKIN_MORPHOLOGY
/**
 * @brief DEFAULT_SKIN_MORPHOLOGY is the default flag, boolean value, if the morphological transformation is performed or not.
 */
#define DEFAULT_SKIN_MORPHOLOGY true
#endif

#ifndef DEFAULT_SAMPLING_AMOUNT_PER_TIME
/**
 * @brief DEFAULT_SAMPLING_AMOUNT_PER_TIME is the amount of samples collected once.
 */
#define DEFAULT_SAMPLING_AMOUNT_PER_TIME 200
#endif
#ifndef DEFAULT_SAMPLING_INTERVAL
/**
 * @brief DEFAULT_SAMPLING_INTERVAL is the minimum interval, in ms, between two sampling actions.
 */
#define DEFAULT_SAMPLING_INTERVAL 200
#endif

#ifndef SAMPLE_SIZE_WIDTH
/**
 * @brief SAMPLE_SIZE_WIDTH is the width of the sample image.
 *
 * Change in this parameter may cause invalidity of samples collected previously.
 */
#define SAMPLE_SIZE_WIDTH 64
#endif
#ifndef SAMPLE_SIZE_HEIGHT
/**
 * @brief SAMPLE_SIZE_HEIGHT is the height of the sample image.
 *
 * Change in this parameter may cause invalidity of samples collected previously.
 */
#define SAMPLE_SIZE_HEIGHT 64
#endif
#ifndef SAMPLING_IMAGE_FORMAT
/**
 * @brief SAMPLING_IMAGE_FORMAT is the format of the sample image
 *
 * Change in this parameter may cause invalidity of samples collected previously.
 */
#define  SAMPLING_IMAGE_FORMAT "PGM"
#endif


#ifndef CAFFE_WORK_MODE
/**
 * @brief CAFFE_WORK_MODE is default caffe work mode which only can be `CPU` or `GPU`.
 */
#define CAFFE_WORK_MODE CPU
#endif


// @cond
#define SETTING_STRING_DELIMITER "!x0x?"
// @endcond

#endif // GLOBAL_H
