/*       +------------------------------------+
 *       | Inspire Internet Relay Chat Daemon |
 *       +------------------------------------+
 *
 *  InspIRCd: (C) 2002-2007 InspIRCd Development Team
 * See: http://www.inspircd.org/wiki/index.php/Credits
 *
 * This program is free but copyrighted software; see
 *            the file COPYING for details.
 *
 * ---------------------------------------------------
 */

#ifndef INSPIRCD_TIMER_H
#define INSPIRCD_TIMER_H

/** Timer class for one-second resolution timers
 * InspTimer provides a facility which allows module
 * developers to create one-shot timers. The timer
 * can be made to trigger at any time up to a one-second
 * resolution. To use InspTimer, inherit a class from
 * InspTimer, then insert your inherited class into the
 * queue using Server::AddTimer(). The Tick() method of
 * your object (which you should override) will be called
 * at the given time.
 */
class InspTimer : public Extensible
{
 private:
	/** The triggering time
	 */
	time_t trigger;
	long secs;
	bool repeat;
 public:
	/** Default constructor, initializes the triggering time
	 * @param secs_from_now The number of seconds from now to trigger the timer
	 * @param now The time now
	 * @param repeating Repeat this timer every secs_from_now seconds if set to true
	 */
	InspTimer(long secs_from_now,time_t now, bool repeating = false)
	{
		trigger = now + secs_from_now;
		secs = secs_from_now;
		repeat = repeating;
	}
	/** Default destructor, does nothing.
	 */
	virtual ~InspTimer() { }
	/** Retrieve the current triggering time
	 */
	virtual time_t GetTimer()
	{
		return trigger;
	}
	/** Called when the timer ticks.
	 */
	virtual void Tick(time_t TIME) = 0;

	bool GetRepeat()
	{
		return repeat;
	}

	long GetSecs()
	{
		return secs;
	}

	void CancelRepeat()
	{
		repeat = false;
	}
};


/** This class manages sets of InspTimers, and triggers them at their defined times.
 * This will ensure timers are not missed, as well as removing timers that have
 * expired and allowing the addition of new ones.
 */
class TimerManager : public Extensible
{
 protected:
	/** A group of timers all set to trigger at the same time
	 */
	typedef std::vector<InspTimer*> timergroup;
	/** A map of timergroups, each group has a specific trigger time
	 */
	typedef std::map<time_t, timergroup*> timerlist;

 private:

	/** The current timer set, a map of timergroups
	 */
	timerlist Timers;

 public:
	/** Tick all pending InspTimers
	 * @param TIME the current system time
	 */
	void TickTimers(time_t TIME);
	/** Add an InspTimer
	 * @param T an InspTimer derived class to add
	 * @param secs_from_now You may set this to the number of seconds
	 * from the current time when the timer will tick, or you may just
	 * leave this unset and the values set by the InspTimers constructor
	 * will be used. This is used internally for re-triggering repeating
	 * timers.
	 */
	void AddTimer(InspTimer* T, long secs_from_now = 0);
	/** Delete an InspTimer
	 * @param T an InspTimer derived class to delete
	 */
	void DelTimer(InspTimer* T);
	/** Tick any timers that have been missed due to lag
	 * @param TIME the current system time
	 */
	void TickMissedTimers(time_t TIME);
};

#endif
