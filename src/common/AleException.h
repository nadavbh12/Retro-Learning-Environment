/*
 * AleException.h
 *
 *  Created on: Jan 5, 2016
 *      Author: administrator
 */

#ifndef ALEEXCEPTION_H_
#define ALEEXCEPTION_H_

#include <iostream>
#include <exception>
using namespace std;

class AleException : public exception
{
public:
	AleException();
	AleException(string str) : description(str){};
	AleException(const char* str) : description(str){};
	virtual const char* what() const throw()
	{
		return description.c_str();
	}
private:
	string description;
};

#endif /* ALEEXCEPTION_H_ */
