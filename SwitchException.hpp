/*
 * File:   SwitchException.hpp
 * Author: milan
 *
 * Created on 12. únor 2012, 13:55
 */

#ifndef SWITCHEXCEPTION_HPP
#define	SWITCHEXCEPTION_HPP

#include <exception>
#include <string>

using namespace std;

class SwitchException: public exception {
public:
	virtual const char* what() const throw();
private:
	string message;

};

#endif	/* SWITCHEXCEPTION_HPP */

