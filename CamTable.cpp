/*
 * File:   CamTable.cpp
 * Author: xpalam00
 *
 * Created on 11. únor 2012, 21:15
 */

#include <map>

#include "CamTable.hpp"

CamTable::CamTable()
{
}

CamTable::~CamTable()
{
}

/**
 * Pokusí se vyhledat cílovou MAC v CAM tabulce. Přestárlé záznamy smaže, čímž
 * se vloží do tabulky záznam znovu.
 * @param mac
 * @return
 */
Iface *CamTable::find(MacAddress *mac)
{
		map<string, CamTableRecord*>::iterator it = this->records.find(mac->getAddress());

		// Hledaný cíl nebyl vůbec nalezen
		if( it == this->records.end() ) return NULL;

		// Nalezený záznam je přestárlý, smaže se a začne se hledat znovu
		if( it->second->getAge() > CamTable::MAX_AGE )
		{
			this->records.erase(it);
			return this->find(mac);
		}

		// Nalezený záznam je v pořádku a vrátí se nalezený Iface
		return it->second->iface;
}

void CamTable::add(Iface *iface, MacAddress *mac)
{
	CamTableRecord *ctr;
	map<string, CamTableRecord*>::iterator it = this->records.find(mac->getAddress());

	//cout << "Add to " << iface->getName() << " (" << iface->macAddress << ") new MAC " << mac << "?" << endl;

	if(it == this->records.end()) // záznam není v tabulce
	{
		//cout << "- Create new CTR: ";
		ctr = new CamTableRecord(iface, mac);
		it = this->records.insert(it, make_pair(mac->getAddress(), ctr));
		//cout << ctr << endl;
	}
	else
	{
		//cout << "- Already known, reset age: ";
		ctr = it->second;
		ctr->resetAge();
		//cout << ctr << endl;
	}
}

/**
 * Vypíše CAM tabulku. Vypisuje pouze záznamy, které nepřekročily maximální dobu.
 * @param stream
 * @param cam
 * @return
 */
ostream &operator<<(ostream &stream, const CamTable *cam)
{
	stream << "MAC address\tPort\tAge" << endl;
	map<string, CamTableRecord*>::const_iterator it = cam->records.begin();
	for(it = cam->records.begin(); it != cam->records.end(); it++)
	{
		if(it->second->getAge() > CamTable::MAX_AGE)
		{
			//cam->records. erase(it);
		}
		else
		{
			stream << it->second << endl;
		}
	}

  return stream;
}
