#include "../headers/ostreamWrapper.h"

#include "../headers/ostreamWrapper.h"

void OstreamWrapper::os_open() {}
void OstreamWrapper::os_close() {}

OstreamWrapper::OstreamWrapper(std::ostream& os) : os(os), os_closed(false), link_cnt(0)
{
	os_open();
}

OstreamWrapper::~OstreamWrapper()
{
	if(!os_closed)
		os_close();
}

void OstreamWrapper::link()
{
	if(link_cnt == 0 && os_closed)
	{
		os_open();
		os_closed = false;
	}

	link_cnt++;
}

void OstreamWrapper::unlink()
{
	link_cnt--;
	
	if(link_cnt && !os_closed)
	{
		os_close();
		os_closed = true;
	}
}

void OstreamWrapper::print(const char* message)
{ os << message << std::endl; }

void OstreamWrapper::print(const std::string& message)
{ os << message << std::endl; }
