#include "DTime.h"
#include "DSystem.h"
#include <windows.h>

DTime::DTime()
{
	m_deltaTime = 0;
	m_fixedDeltaTime = 0.02;
	m_timer = 0;
	m_lag = 0;
	m_beginTime = 0;
	m_currentTime = 0;
	m_previousTime = 0;
	m_frameTime = 0;
	m_frameCount = 0;
	m_fps = 0;
	m_sleepTime = (m_fixedDeltaTime * 1000);
}


DTime::~DTime()
{
}

void DTime::Init()
{
	m_currentTime = clock();
	m_beginTime = m_currentTime;
	m_previousTime = m_currentTime;
	m_frameTime = m_currentTime;
}

void DTime::Update()
{
	m_currentTime = clock();
	m_deltaTime = ((double)(m_currentTime - m_previousTime)) * 0.001f;
	m_timer = ((double)(m_currentTime - m_beginTime)) * 0.001f;
	m_previousTime = m_currentTime;

	//根据实际deltaTime进行补偿
	m_lag += m_deltaTime;

	m_frameCount++;

	if (m_currentTime - m_frameTime > 5) {
		double ftime = ((double)m_frameCount) / (m_currentTime - m_frameTime);
		m_fps = (ftime * 1000);
		m_frameTime = m_currentTime;
		m_frameCount = 0;
	}

	DShader::SetGlobalFloat(D_SC_TIME, m_timer);
}

void DTime::Wait()
{
	Sleep(m_sleepTime);
}

void DTime::Shutdown()
{
	m_deltaTime = 0;
	m_fixedDeltaTime = 0;
	m_timer = 0;
	m_lag = 0;
	m_beginTime = 0;
	m_currentTime = 0;
	m_previousTime = 0;
	m_frameTime = 0;
	m_frameCount = 0;
	m_fps = 0;
	m_sleepTime = 0;
}

bool DTime::BeginFixedUpdateLoop()
{
	bool result = m_lag >= m_fixedDeltaTime;
	m_lag -= m_fixedDeltaTime;
	return result;
}

double DTime::GetDeltaTime()
{
	return DSystem::GetTimeMgr()->m_deltaTime;
}

double DTime::GetFixedDeltaTime()
{
	return DSystem::GetTimeMgr()->m_fixedDeltaTime;
}

double DTime::GetTimer()
{
	return DSystem::GetTimeMgr()->m_timer;
}

unsigned int DTime::GetFPS()
{
	return DSystem::GetTimeMgr()->m_fps;
}
