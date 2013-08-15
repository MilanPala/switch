/*
 * File:   RunSwitch.cpp
 * Author: xpalam00
 *
 * Created on 12. únor 2012, 12:39
 */

#include <cstdlib>
#include <string>
#include <iostream>

#include "Switch.hpp"

using namespace std;

Switch * mainSwitch;

/**
 * Inicializuje program switch a spustí ho
 */
int main(int argc, char** argv) {

	mainSwitch = new Switch();

	return 0;
}

