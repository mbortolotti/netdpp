#include "dpp_exception.h"


dpp_exception::dpp_exception(const char *message)
{
	msg = message;
}

const char* dpp_exception::what() const
{
	return msg;
}
