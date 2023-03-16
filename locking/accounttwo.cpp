#include <cstdio>
#include <string>
#include <sstream>

#include "account.h"

using std::string;

void Account::add(unsigned int tpounds, unsigned int tpence)
{
	std::lock_guard<std::mutex>guard(account_m);
	pounds_ += tpounds;
	pence_ += tpence;

	// Ensure pence_ stays in the range 0-99.
	if (pence_ >= 100) {
		pounds_++;
		pence_ -= 100;
	}
}

string Account::total()
{
	char buf[40];
	snprintf(buf, sizeof buf, "GBP %u.%02u", pounds_, pence_);
	//std::stringstream ss;
	//ss << "GBP " << pounds_.load() << "." << pence_.load() << "\n";
	return string(buf);
}

