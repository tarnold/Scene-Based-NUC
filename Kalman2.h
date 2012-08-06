#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <dirent.h>
#include <time.h>

typedef struct {
	int x;
	int y;
} mouse_location;

mouse_location mouse_position = {0, 0};
mouse_location mouse_control;

void mouse(int mouse_event, int x, int y, int mouse_flags, void *params);
CvPoint Kalman(CvKalman *KF, CvMat *measurement, CvMat *prediction, CvMat *estimate, int count);





