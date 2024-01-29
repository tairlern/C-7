#pragma once

#include<fstream>
#include<cstring>
#include<iostream>
using namespace std;
class Family
{
public:
	int familyId;
	char lastName[20];
	int numFamily;
	char phon[10];
	bool isClass[6]={false};
	//=======================


	/*int getFamilyId();
	bool*  getisClass();*/
	
	Family();
	friend istream& operator >>(istream& os, Family& f);

};
//bool* Family::getisClass() {
//	return isClass;
//}
//int Family::getFamilyId() {
//	return familyId;
//}

Family::Family()
{
	familyId = 0;
	char TlastName[20] = { "" };
	strcpy_s(this->lastName, strlen(TlastName) + 1, TlastName);
	numFamily = 0;
	char Tphon[20] = { "" };
	strcpy_s(this->phon, strlen(Tphon) + 1, Tphon);
}
istream& operator >>(istream& os, Family& f) {
	os >> f.familyId >> f.lastName >> f.numFamily >> f.phon;
	return os;
}