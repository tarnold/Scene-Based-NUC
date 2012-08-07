#include "Kalman2.h"


/*
		Kalman

	Makes the Kalman filters
	prediction and estimation.
*/
CvPoint Kalman(CvKalman *KF, CvMat *measurement, CvMat *prediction, CvMat *estimate, int count) {

CvPoint s_point;

	prediction = cvKalmanPredict(KF, 0);
	cvSet1D(measurement, 0, cvRealScalar(mouse_position.x));
	cvSet1D(measurement, 1, cvRealScalar(mouse_position.y));
	estimate = cvKalmanCorrect(KF, measurement);
	s_point = cvPoint(cvGetReal1D(estimate, 0), cvGetReal1D(estimate, 1));

	return s_point;

}


/*
		Mouse

	Keeps track of the users mouse
	position so the Kalman Filter
	can make the proper predictions.
*/
void mouse(int mouse_event, int x, int y, int mouse_flags, void *params) {

	mouse_control = mouse_position;
	mouse_position.x = x;
	mouse_position.y = y;

}


/*
		Main

	Initializes matrices and images
	to make the calculations for
	the Kalman Filter.
*/
int main(int argc, char *argv) {

IplImage *image, *clone, *estimate;
CvMat *meas_clone, *measurement, *prediction;
CvKalman *KF;
CvRNG rng;
char exit;
double matrix[] = {1, 1, 0, 1};
CvPoint kalmanPosition;
int count = 0;


	clone = cvCreateImage(cvSize(500, 500), 8, 3);
	rng = cvRNG(-1);
	meas_clone = cvCreateMat(2, 1, CV_32F);
	exit = -1;
	cvSetZero(clone);
	cvSetZero(meas_clone);
	KF = cvCreateKalman(4, 2, 0);

	cvNamedWindow("Kalman Filter", CV_WINDOW_AUTOSIZE);
	cvSetMouseCallback("Kalman Filter", mouse, 0);

	cvSetIdentity(KF->process_noise_cov, cvRealScalar(1e-5));
	cvSetIdentity(KF->measurement_matrix, cvRealScalar(1));
	cvSetIdentity(KF->error_cov_post, cvRealScalar(1));
	cvSetIdentity(KF->measurement_noise_cov, cvRealScalar(1e-1));
	cvRandArr(&rng, KF->state_post, CV_RAND_NORMAL, cvRealScalar(0), cvRealScalar(0.1));

	for(;;) {
		image = cvClone(clone);
		measurement = cvClone(meas_clone);
		prediction = cvCreateMatHeader(2, 1, CV_32F);
		estimate = cvCreateMatHeader(2, 1, CV_32F);
		kalmanPosition = Kalman(KF, measurement, prediction, estimate, count++);
		cvCircle(image, cvPoint(kalmanPosition.x, kalmanPosition.y), 2, CV_RGB(255, 255, 255), 2, CV_AA, 0); 
		cvShowImage("Kalman Filter", image);
		cvReleaseImage(&image);
		cvReleaseMat(&measurement);
		cvReleaseMat(&prediction);
		cvReleaseMat(&estimate);

		exit = (char) cvWaitKey(100);
		if (exit > 0)
			break;
	}

	cvReleaseImage(&clone);
	cvReleaseKalman(&KF);
	cvReleaseMat(&meas_clone);
	cvDestroyWindow("Kalman Filter");

	return;

}

