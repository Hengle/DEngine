#include "DException.h"
#include "DLog.h"
#include <string>

DException::DException(const char * message) : std::exception(message)
{

#if _DEBUG
	char msg[1024];
	sprintf_s(msg, 1024, "DException:%s", message);

	DLog::Err(msg);
#endif
}
