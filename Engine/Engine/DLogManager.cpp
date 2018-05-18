#include "DLogManager.h"
#include "DImGUI.h"

DLogManager::DLogManager()
{
	m_logMsgs = 0;
}


DLogManager::~DLogManager()
{
}

void DLogManager::Init()
{
	m_logMsgs = new std::vector<DLogMsg*>();
}

void DLogManager::Shutdown()
{
	int i;
	int size = m_logMsgs->size();
	for (i = 0; i < size; i++) {
		delete m_logMsgs->at(i);
	}
	m_logMsgs->clear();
	delete m_logMsgs;
	m_logMsgs = NULL;
}

void DLogManager::DrawMsgs()
{
	if (DLOG_SHOW == false)
		return;
	int i;
	int size = m_logMsgs->size();

	ImGui::Begin("Console");
	
	for (i = 0; i < size && i <= DLOG_MAX_SHOW_LOG_NUMS; i++) {
		DLogMsg* msg = m_logMsgs->at(i);
		if (msg != NULL)
			DrawMsg(msg);
	}
	
	ImGui::End();
}

void DLogManager::AddLog(DLogMsg * msg)
{
	m_logMsgs->insert(m_logMsgs->begin(), msg);
	if (m_logMsgs->size() > DLOG_MAX_CACHE_LOG_NUMS) {
		m_logMsgs->pop_back();
	}
}

void DLogManager::DrawMsg(DLogMsg * msg)
{
	ImVec4 color;
	
	if (msg->type == DLogType::Info) {
		color.x = 1;
		color.y = 1;
		color.z = 1;
		color.w = 1;
	}
	else if (msg->type == DLogType::Warning) {
		color.x = 1;
		color.y = 1;
		color.z = 0;
		color.w = 1;
	}
	else if (msg->type == DLogType::Error) {
		color.x = 1;
		color.y = 0;
		color.z = 0;
		color.w = 1;
	}
	ImGui::TextColored(color, msg->msg.data());
}
