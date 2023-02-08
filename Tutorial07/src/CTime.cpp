#include "CTime.h"

CTime::CTime()
{
	m_iCurrentTime;
	m_iLastTime;
	m_iTimerFrequency;
}

CTime::~CTime()
{
}

void CTime::init() //Inicializacion
{
	
	QueryPerformanceCounter(&m_iLastTime);
	QueryPerformanceFrequency(&m_iTimerFrequency);
}

void CTime::update()
{
	QueryPerformanceCounter(&m_iCurrentTime);	
	m_fDeltaTime = (m_iCurrentTime.QuadPart - m_iLastTime.QuadPart) / (float)m_iTimerFrequency.QuadPart;
	m_iLastTime = m_iCurrentTime;
	m_fDeltaTime = min(m_fDeltaTime, 0.1f);
}

void CTime::render()
{
}

void CTime::destroy()
{
}