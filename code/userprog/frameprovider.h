#include "bitmap.h"

class FrameProvider
{
<<<<<<< HEAD
  public:
=======
private:
	BitMap *pageFrames;

public:
>>>>>>> 23d7c9307cb4bb555a76e7038707ef57e671695a
	FrameProvider(int numPages);
	~FrameProvider();

	// return an empty Frame
	int GetEmptyFrame();

	// free a Fream
	void ReleaseFrame(int numFrame);

	// return number of available Frames
	int NumAvailFrame();
<<<<<<< HEAD
  private:
	BitMap *pageFrames;
=======
>>>>>>> 23d7c9307cb4bb555a76e7038707ef57e671695a
};
