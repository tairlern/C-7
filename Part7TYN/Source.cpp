#include"Family.h"
#include<fstream>
#include<cstring>
#include<iostream>
#include<queue>
using namespace std;

enum option { ADD = 1, DEL, UPDATE, WAITING, RISHUM, PRINT, CLASS, };
static queue<Family>waite;
void setFile(fstream& file) {
	Family f;
	for (int i = 0; i < 100; i++)
	{
		file.write((char*)&f, sizeof(Family));
	}
}
void add(fstream& file) {
	Family f;
	Family temp;
	std::cout << "enter Family id ,lasrname,numofFumily,phon" << endl;
	std::cin >> f;
	if (f.familyId < 1 || f.familyId > 100) {
		throw "ERROR: invalid family number ";
	}
	file.seekg((f.familyId - 1) * sizeof(Family), ios::beg);//בדיקה אם קיים כבר
	file.read((char*)&temp, sizeof(Family));
	if (temp.familyId != 0)
		throw "ERROR:Family is already in the file";

	file.seekg((f.familyId - 1) * sizeof(Family), ios::beg);
	file.write((char*)&f, sizeof(Family));

}
void del(fstream& file, int id) {
	Family t;
	if (id < 1 || id>100)
		throw "ERROR: Invalid family number";
	file.seekg((id - 1) * sizeof(Family), ios::beg);
	file.write((char*)&t, sizeof(Family));
}
void update(fstream& file, int id)
{
	bool whentClass[6];//הערכים של הלקוח
	bool flug = false;
	int count = 0;
	Family t;
	Family source;
	if (id < 1 || id>100)
		throw "ERROR: Invalid family number";
	file.seekg((id - 1) * sizeof(Family), ios::beg);//הבאת הלקוח לפי מיספר
	file.read((char*)&source, sizeof(Family));
	if (source.familyId == 0) {
		throw "ERROR: Family is not in the file";
	}
	//אם הלקוח קיים 
	for (int i = 0; i < 6; i++)
	{
		cout << "enter 1 if yue went to  take part in class number else enter 0: " << i + 1 << endl;
		cin >> whentClass[i];
	}
	for (int i = 0; i < 6; i++)
	{
		file.seekg(0, ios::beg);
		if (whentClass[i]) {
			count = 0;
			for (int j = 0; j < 100; j++)
			{
				file.seekg((j) * sizeof(Family), ios::beg);
				file.read((char*)&t, sizeof(Family));
				if (t.familyId != 0 && t.isClass[i] == true)
					count++;
			}
			if (count < 10)
			{
				source.isClass[i] = true;
				whentClass[i] = false;
			}
			else {
				flug = true;
			}

		}
	}
	file.seekg((id - 1) * sizeof(Family), ios::beg);
	file.write((char*)&source, sizeof(Family));
	if (flug) {
		for (int i = 0; i < 6; i++)
		{
			source.isClass[i] = whentClass[i];
		}
		waite.push(source);
	}
}
void waiting(fstream& file, queue<Family>q) {
	q = waite;//הצבעה על תור הממתינים
	queue<Family>tempq;
	bool arr[6];
	Family temp, t,result;
	char tav;
	int count;
	bool flug = false;
	while (q.size() > 0) {
		temp = q.front();
		for (int i = 0; i < 6; i++)//המערך שלא מטופל
		{
			arr[i] = temp.isClass[i];
		}
		for (int i = 0; i < 6; i++)
		{
			if (arr[i]) {
				file.seekg(0, ios::beg);
				count = 0;
				for (int j = 0; j < 100; j++)
				{
					file.seekg((j) * sizeof(Family), ios::beg);
					file.read((char*)&t, sizeof(Family));
					if (t.familyId != 0 && t.isClass[i] == true)
						count++;
				}
				if (count < 10)
				{
					cout << "name : " << temp.lastName << "phon: " << temp.phon << endl;
					cout << "Are you still interested in signing up for the class? if yes enter Y else enter N " << endl;
					cin >> tav;
					if (tav == 'Y') {
						
					 //עידכון בתוך הקןבץ
						file.seekg((temp.familyId - 1) * sizeof(Family), ios::beg);
						file.read((char*)&result, sizeof(Family));
						result.isClass[i] = true;
						file.seekg(0, ios::beg);
						file.seekg((temp.familyId - 1) * sizeof(Family), ios::beg);
						file.write((char*)&result, sizeof(Family));
					}
					arr[i] = false;
				}
				else {
					flug = true;//רצה להכנס ולא יכל
				}
			}
		}
		for (int i = 0; i < 6; i++)//עידכון הנתונים והכנסתם לתור
		{
			temp.isClass[i] = arr[i];
		}

		if (flug) {
			tempq.push(temp);
		}
		q.pop();
	}
	while (tempq.size() > 0)
	{
		waite.push(tempq.front());
		tempq.pop();
	}
}
bool rishum(fstream& file, int id, int numClass) {
	Family f;
	if (id < 1 || id>100)
		throw "ERROR: Invalid family number";
	if (numClass < 1 || numClass>6)
		throw "ERROR: Invalid class number";
	file.seekg((id - 1) * sizeof(Family), ios::beg);//הבאת הלקוח לפי מיספר
	file.read((char*)&f, sizeof(Family));
	if (f.familyId == 0)
		throw "ERROR: Family is not in file";
	if (f.isClass[numClass])
		return true;
	return false;
}
void print(fstream& file, int id) {
	Family f;
	bool flug=false;
	if (id < 1 || id>100)
		throw "ERROR: Invalid family number";
	file.seekg((id - 1) * sizeof(Family), ios::beg);//הבאת הלקוח לפי מיספר
	file.read((char*)&f, sizeof(Family));
	if (f.familyId == 0)
		throw "ERROR: Family is not in file";
	std::cout << "family id :" << f.familyId << " last name : " << f.lastName << " phon : " << f.phon << " num of person in family" << f.numFamily<<" in class: ";
	for (int i = 0; i < 6; i++)
	{
		if (f.isClass[i])
			flug = true;
	}
	if (flug)
		std::cout << "Y" << endl;
	else {
		std::cout << "N" << endl;
	}


}
void inClass(fstream& file, int numClass) {
	Family f;
	if (numClass < 1 || numClass>6)
		throw "ERROR: Invalid class number";
	for (int i = 0; i < 100; i++)
	{
		file.seekg((i) * sizeof(Family), ios::beg);//הבאת הלקוח לפי מיספר
		file.read((char*)&f, sizeof(Family));
		if (f.familyId!=0&&f.isClass[numClass-1]==true)
			cout << f.lastName << " ";
	}
	cout << " " << endl;
}
int main() {

	queue<Family> q;
	fstream file;
	file.open("families.txt", ios::binary | ios::in | ios::out);
	if (!file)
	{
		cout << "ERROR: couldn't open file\n";
		return 0;
	}
	setFile(file);
	int choice;
	int snum;
	int cnum;
	cout << "Choices are:\n0 to exit\n1 to add a family\n2 to delete a family\n3 to update rishum to classes\n4 to update waiting to classes \n5 to check rishum for a classas\n6 to print a family\n7 to print all the families that participate in a specific class\n";
	cout << "enter 0-7:\n";
	cin >> choice;
	while (choice)
	{
		switch (choice)
		{
		case ADD://add to the file
			try { add(file); }
			catch (const char* msg) { cout << msg; }
			break;
		case DEL://delete from file
			cout << "enter number of family to delete:\n";
			cin >> snum;
			try { del(file, snum); }
			catch (const char* msg) { cout << msg; }
			break;
		case UPDATE://update the list of classes of a family
			cout << "enter number of family to update:\n";
			cin >> snum;
			try { update(file, snum); }
			catch (const char* msg) { cout << msg; }
			break;
		case WAITING://update the list of classes of a waiting family
			waiting(file, q);
			break;
		case RISHUM://check rishum to a specific class
			cout << "enter number of family to check rishum:\n";
			cin >> snum;
			cout << "enter number of class to check rishum:\n";
			cin >> cnum;
			try
			{
				cout << "The family is" << (rishum(file, snum, cnum) ? " " : " not ") << "taking the class\n";
			}
			catch (const char* msg) { cout << msg; }
			break;
		case PRINT://print the details of a specific family
			cout << "enter number of family to print:\n";
			cin >> snum;
			try { print(file, snum); }
			catch (const char* msg) { cout << msg; }
			break;
		case CLASS://print the details of all the families that are taking a specific class
			cout << "enter number of class to check rishum:\n";
			cin >> cnum;
			try { inClass(file, cnum); }
			catch (const char* msg) { cout << msg; }
			break;
		default:
			cout << "ERROR: invalid choice\n";

		}
		cout << "\nenter 0-7:\n";
		cin >> choice;
	}
	file.close();


	return 0;
}