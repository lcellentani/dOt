#ifndef __DOT_STOPWATCH_H_HEADER_GUARD__
#define __DOT_STOPWATCH_H_HEADER_GUARD__

namespace dot
{
namespace core
{

class StopWatch
{
public:
	StopWatch() : mRunning(false) {}
	virtual ~StopWatch() { }

	virtual void Start() = 0;

	virtual void Stop() = 0;

	virtual void Reset() = 0;

	const bool IsRunning() const { return mRunning; }

	/// Get elapsed time
	/// Time in seconds after start. If the stop watch is still running (i.e. there
	/// was no call to #stop()) then the elapsed time is returned, otherwise the
	/// summed time between all #start() and #stop() calls is returned
	/// \return The elapsed time in seconds
	virtual const float GetTime() const = 0;

protected:
	bool mRunning;
};

class StopWatchFactory
{
public:
	StopWatchFactory() { }
	virtual ~StopWatchFactory() { }

	virtual StopWatch * CreateStopWatch() = 0;
};

} // namespace core
} // namespace dot

#endif // __DOT_STOPWATCH_H_HEADER_GUARD__