#include "bitmap.h"

class FrameProvider
{


public:
	FrameProvider(int numPages);
	~FrameProvider();

	// return an empty Frame
	int GetEmptyFrame();

	// free a Fream
	void ReleaseFrame(int numFrame);

	// return number of available Frames
	int NumAvailFrame();

  private:
	BitMap *pageFrames;
};
