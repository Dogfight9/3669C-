#ifndef Camera_H
#define Camera_H

#include "MyDefines.h"
#include "WPILib.h"
#include "Vision/RGBImage.h"
#include "Vision/BinaryImage.h"
#include "Math.h"

class Camera
{
public:
	/*struct Scores {
			double rectangularity;
			double aspectRatioVertical;
			double aspectRatioHorizontal;
		};
		
		struct TargetReport {
			int verticalIndex;
			int horizontalIndex;
			bool Hot;
			double totalScore;
			double leftScore;
			double rightScore;
			double tapeWidthScore;
			double verticalScore;
		};
	
	double computeDistance (BinaryImage *image, ParticleAnalysisReport *report);
	double scoreAspectRatio(BinaryImage *image, ParticleAnalysisReport *report, bool vertical);
	bool scoreCompare(Scores& scores, bool vertical);
	double scoreRectangularity(ParticleAnalysisReport *report);
	double ratioToScore(double ratio);
	bool hotOrNot(TargetReport& target);*/
	void setHotSide(string c) {hSide = c;}
	string hotSide(){return hSide;}
	static Camera * GetInstance();

	
	
protected:
	Camera();
	
private:
	static Camera * instance;
	
	static string hSide;
	
	Task * m_CameraTask;
};
#endif // Compressor_H
