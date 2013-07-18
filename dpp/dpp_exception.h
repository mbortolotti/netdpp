#ifndef DPP_EXCEPTION
#define DPP_EXCEPTION

#include <exception>

class dpp_exception :
	public std::exception
{
public:
	dpp_exception(const char *message);
	const char* what() const;

private:
	const char * msg;
};

#endif
