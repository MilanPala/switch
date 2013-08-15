/*
 * File:   Frame.cpp
 * Author: milan
 *
 * Created on 13. únor 2012, 17:01
 */

#include "Frame.hpp"

void print_hex_ascii_line(const u_char *payload, int len, int offset)
{

	int i;
	int gap;
	const u_char *ch;

	/* offset */
	printf("%05d   ", offset);

	/* hex */
	ch = payload;
	for(i = 0; i < len; i++) {
		printf("%02x ", *ch);
		ch++;
		/* print extra space after 8th byte for visual aid */
		if (i == 7)
			printf(" ");
	}
	/* print space to handle line less than 8 bytes */
	if (len < 8)
		printf(" ");

	/* fill hex gap with spaces if not full line */
	if (len < 16) {
		gap = 16 - len;
		for (i = 0; i < gap; i++) {
			printf("   ");
		}
	}
	printf("   ");

	/* ascii (if printable) */
	ch = payload;
	for(i = 0; i < len; i++) {
		if (isprint(*ch))
			printf("%c", *ch);
		else
			printf(".");
		ch++;
	}

	printf("\n");

return;
}

void print_payload(const u_char *payload, int len)
{

	int len_rem = len;
	int line_width = 16;			/* number of bytes per line */
	int line_len;
	int offset = 0;					/* zero-based offset counter */
	const u_char *ch = payload;

	if (len <= 0)
		return;

	/* data fits on one line */
	if (len <= line_width) {
		print_hex_ascii_line(ch, len, offset);
		return;
	}

	/* data spans multiple lines */
	for ( ;; ) {
		/* compute current line length */
		line_len = line_width % len_rem;
		/* print line */
		print_hex_ascii_line(ch, line_len, offset);
		/* compute total remaining */
		len_rem = len_rem - line_len;
		/* shift pointer to remaining bytes to print */
		ch = ch + line_len;
		/* add offset */
		offset = offset + line_width;
		/* check if we have line width chars or less */
		if (len_rem <= line_width) {
			/* print last line and get out */
			print_hex_ascii_line(ch, len_rem, offset);
			break;
		}
	}

return;
}

unsigned int Frame::getSizeOf()
{
	return sizeof(this->payload);
}

void Frame::setSizeOf(unsigned int size)
{
    this->size = size;
}

Frame::Frame(u_char *packet)
{
	//cout << "Frame::Frame()" << endl;
	this->payload = packet;
	static int count = 1;                   /* packet counter */

	/* declare pointers to packet headers */
	const struct sniff_ethernet *ethernet;  /* The ethernet header [1] */
	const struct sniff_ip *ip;              /* The IP header */
	const struct sniff_tcp *tcp;            /* The TCP header */
	const char *payload;                    /* Packet payload */

	int size_ip;
	int size_tcp;
	int size_payload;

	//printf("Packet number %d:\n", count);
	count++;

	/* define ethernet header */
	ethernet = (struct sniff_ethernet*)(packet);

	this->dstMac = new MacAddress((u_char *)ethernet->ether_dhost);
	this->srcMac = new MacAddress((u_char *)ethernet->ether_shost);
	//cout << " MAC source: " << sourceMacAddress << endl;
	//cout << "  MAC desti: " << destMacAddress << endl;
	//printf("       From: %02X:%02X:%02X:%02X:%02X:%02X\n", ethernet->ether_shost[0],ethernet->ether_shost[1],ethernet->ether_shost[2],ethernet->ether_shost[3],ethernet->ether_shost[4],ethernet->ether_shost[5]);

}

u_char* Frame::getPayload()
{
	return this->payload;
}

Frame::~Frame()
{
	cout << "Delete Frame" << endl;
	delete this->srcMac;
	delete this->dstMac;
}

