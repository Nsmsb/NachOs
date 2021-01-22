#include "frameprovider.h"
#include "bitmap.h"
#include "synch.h"

// constructor
FrameProvider::FrameProvider(int numPages)
{
	pageFrames = new BitMap(numPages);
}

FrameProvider::~FrameProvider()
{
	delete pageFrames;
}

int
FrameProvider::GetEmptyFrame()
{
	// simple version
	int empty_frame = -1;
	empty_frame = pageFrames->Find();

	// random version
	// int random_frame_id = rand() % pageFrames->NumClear();
	// int free_frames[random_frame_id];

	// // selecting the random frame
	// for (int i = 0; i < random_frame_id; i++)
	// 	free_frames[i] = pageFrames->Find();

	// empty_frame = free_frames[random_frame_id-1];

	// // freeing frames
	// for (int i = 0; i < random_frame_id-1; i++)
	// 	pageFrames->Clear(free_frames[i]);
	
	return empty_frame;
}

void
FrameProvider::ReleaseFrame(int numFrame)
{
	// fm_mutex->P();
	pageFrames->Clear(numFrame);
	// fm_mutex->V();
}

int
FrameProvider::NumAvailFrame()
{
	return pageFrames->NumClear();
}
