#include "bitmap.h"

class FrameProvider
{
private:
	BitMap *pageFrames;

public:
	FrameProvider(int numPages);
	~FrameProvider();

	// return an empty Frame
	int GetEmptyFrame();

	// free a Fream
	void ReleaseFrame(int numFrame);

	// return number of available Frames
	int NumAvailFrame();
};
