///////////////////////////////////////////////////////////
// BasicSynth - 
//
/// @file WaveOutDirect.h Send samples to the sound card using DirectSound
//
// We have two options:
// 1. Indirect - samples are put into a local buffer and then
//             copied to the direct sound buffer when the local
//             buffer is filled.
// 2. Direct - samples are written into the direct sound buffer.
//
// The direct method is slightly faster, but the indirect
// method is safer since it always has a valid buffer.
//
// Copyright 2008, Daniel R. Mitchell
///////////////////////////////////////////////////////////
/// @addtogroup grpSeq
//@{

#ifndef _WAVEOUTDIRECT_H
#define _WAVEOUTDIRECT_H 1

/// Sound output to DAC.
/// Ths class uses uses the Windows DirectSound buffer
/// as the sample buffer. The buffer is divided into
/// multiple short blocks. While one block is playing,
/// other blocks can be filled in preparation. For
/// interactive playing the lead time (block size) should
/// be short (ca. 20-50ms) and the number of blocks kept
/// small (3-6) to avoid latency problems.
/// If playing a sequence, the time can be longer
/// to allow for more lead time.
class WaveOutDirect : public WaveOutBuf
{
protected:
	IDirectSound *dirSndObj;
	IDirectSoundBuffer *dirSndBuf;

	DWORD numBlk;
	DWORD nextWrite;
	DWORD blkLen;
	DWORD bufLen;
	DWORD lastBlk;
	void *startLock;
	DWORD sizeLock;
	DWORD pauseTime;
	int outState;

	int CreateSoundBuffer(HWND w, float leadtm);

public:
	WaveOutDirect();
	~WaveOutDirect();
	/// Setup the sound output buffer.
	/// @param wnd window handle to pass to DirectSound for collaboration
	/// @param leadtm block length in seconds
	/// @param nb number of blocks (>=3)
	int Setup(HWND wnd, float leadtm, int nb = 4);
	/// Stop the sound output.
	void Stop();
	/// Flush output.
	/// This overrides the base class method and is the point
	/// where we need to switch to another block in the sound
	/// buffer. We don't actually flush anything since we are writing directly
	/// to the DirectSoundBuf data. We move the pointer forward
	/// to the next block of samples in the buffer.
	/// This has the side-effect of waiting on playback
	/// to move out of the way, and causes output to be
	/// synchronized with sample rate.
	virtual int FlushOutput();
};

/// Ths class uses an indirect buffer write.
/// Samples are put into a local buffer then
/// copied during FlushOutput. This is safer
/// since we always have a valid buffer to 
/// write into. It also allows stopping and
/// resarting the playback, although you will
/// loose samples if you do that.
class WaveOutDirectI : public WaveOutDirect
{
public:
	WaveOutDirectI();
	/// @copydoc WaveOutDirect::Setup
	int Setup(HWND wnd, float leadtm, int nb = 4);
	/// @copydoc WaveOutDirect::FlushOutput
	virtual int FlushOutput();
};

//@}
#endif
