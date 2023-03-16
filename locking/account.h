#pragma once

#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <atomic>
#include <mutex>

#include <string>



/** An Account keeps track of a non-negative amount of money, expressed in
pounds and pence -- e.g. the total of a restaurant bill. */
class Account
{
public:
	/** Add an amount to the total. */
	void add(unsigned int pounds, unsigned int pence);

	/** Retrieve the total, formatted as a string (e.g. "GBP 4.95"). */
	std::string total();

private:
	std::mutex account_m;
	unsigned int pounds_ = 0;
	unsigned int pence_ = 0;
};

#endif
