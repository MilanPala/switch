/*
 * File:   Switch.cpp
 * Author: xpalam00
 *
 * Created on 11. únor 2012, 20:18
 */

#include "Switch.hpp"

#define DBG false

pthread_cond_t cond;

Switch::Switch() {
	if(DBG) cerr << "Switch:Switch()" << endl;

	pthread_attr_t attr;
	int res;

	/* vytvoření implicitních atributů */
	if( (res = pthread_attr_init(&attr)) != 0 )
	{
		fprintf(stderr, "pthread_attr_init() err %d\n", res);
	}

	pthread_cond_init(&cond, NULL);

	pthread_create(&(this->_id), NULL, &_entry, this);

	//int res;
	void *result;
	/* čekání na dokončení a převzetí stavu */
	if( (res = pthread_join(_id, &result)) != 0 )
	{
		fprintf(stderr, "pthread_attr_init() err %d\n",res);
	}
}

/**
 * Pokusí se otevřít všechna Ethernetová rozhraní systému.
 */
void Switch::initDevices()
{
	short ret = -1;
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_if *devList;
	ret = pcap_findalldevs(&devList, errbuf);
	if (ret == -1) {
		cerr << errbuf << endl;
		exit(1);
	}

	char errbuf_libnet[LIBNET_ERRBUF_SIZE];

	if(DBG) cerr << "Dev list:" << endl;
	for(devList=devList; devList != NULL; devList = devList->next)
	{
		pcap_t *pcap_handle = pcap_open_live(devList->name, BUFSIZ, 1, 1000, errbuf);
		if (pcap_handle == NULL)
		{
			if(DBG) cerr << "Couldn't open device " << devList->name << ". (" << errbuf << ")" << endl;
			continue;
		}

		// Test, zda nalezené rozhraní je ethernetové
		if(pcap_datalink(pcap_handle) != DLT_EN10MB)
		{
			if(DBG) cerr << "Rozhranní " << devList->name << " není Ethrnet." << endl;
			continue;
		}
		pcap_close(pcap_handle);

		// Test na loopback
		if( (devList->flags & PCAP_IF_LOOPBACK) == true ) { continue; }

		// zjistí MAC adresu interface
		libnet_t *l = libnet_init(LIBNET_RAW4, devList->name, errbuf_libnet);
		if ( l == NULL )
		{
			fprintf(stderr, "libnet_init() failed: %s\n", errbuf_libnet);
			continue;
			//exit(EXIT_FAILURE);
		}
		struct libnet_ether_addr * mac_addr = libnet_get_hwaddr(l);
		if ( mac_addr == NULL ) { continue; }

		MacAddress *mac = new MacAddress(mac_addr);

		libnet_destroy(l);

		Iface *iface = new Iface(this);
		iface->setName(devList->name);
		iface->macAddress = mac;
		this->ifaces->add(iface);

		if(DBG) cerr << devList->name << devList->flags << endl;
		if( devList->description != NULL ) cout << devList->description << endl;
	}
	if(DBG) cerr << "Dev list end." << endl;
}

Switch::~Switch()
{
	delete this->camTable;
	delete this->ifaces;
}

CamTable * Switch::getCamTable()
{
	return this->camTable;
}

Ifaces * Switch::getIfaces()
{
	return this->ifaces;
}

void *Switch::_entry(void *data)
{
	if(DBG) cerr << "Switch::_entry()" << endl;
	Switch *thread = (Switch *)data;
	return thread->main();
}

void *Switch::main()
{
	if(DBG) cerr << "Switch::main()" << endl;

	this->camTable = new CamTable();
	this->ifaces = new Ifaces();

	// inicializace portů
	this->initDevices();

	// uživatelské rozhraní
	this->userPromt();

	return 0;
}

/**
 * Obsluhuje uživatelské rozhraní - čeká na příkazy a provadí operace
 */
void Switch::userPromt()
{
	if(DBG) cerr << "Switch::userPromt()" << endl;

	string userCommand;
	bool ask = true;

	do
	{
		cout << endl << "switch> ";
		cin >> userCommand;

		if( userCommand.compare("stat") == 0 )
		{
			cout << this->getIfaces();
		}
		else if ( userCommand.compare("cam") == 0 )
		{
			cout << this->getCamTable();
		}
		else if ( userCommand.compare("igmp") == 0 )
		{}
		else if ( userCommand.compare("quit") == 0 )
		{
			ask = false;
		}
		else
		{
			cout << "Bad command. Try stat|cam|igmp|quit." << endl;
		}
	} while(ask == true);
}