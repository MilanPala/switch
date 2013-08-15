/*
 * File:   SwitchException.cpp
 * Author: milan
 *
 * Created on 12. únor 2012, 13:55
 */

#include "SwitchException.hpp"

virtual const char* SwitchException::what() const throw()
{
	return this->message;
}

