#pragma once
#include <time.h>

class DTime
{
public:
	DTime();
	~DTime();
	void Init();
	void Update();
	void Wait();
	void Shutdown();
	bool BeginFixedUpdateLoop();

	static double GetDeltaTime();
	static double GetFixedDeltaTime();
	static double GetTimer();
	static unsigned int GetFPS();

private:
	double m_deltaTime;
	double m_fixedDeltaTime;
	double m_timer;
	double m_lag;
	unsigned int m_fps;
	unsigned int m_frameCount;
	unsigned int m_sleepTime;
	clock_t m_beginTime;
	clock_t m_currentTime;
	clock_t m_previousTime;
	clock_t m_frameTime;
};
