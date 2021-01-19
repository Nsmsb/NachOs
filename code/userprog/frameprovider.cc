#include "frameprovider.h"
#include "bitmap.h"
#include "synch.h"

// static Semaphore *fm_mutex = new Semaphore("frmaeProvider Semaphore", 1);

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
	// fm_mutex->P();
	int empty_frame = pageFrames->Find();
	// fm_mutex->V();

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