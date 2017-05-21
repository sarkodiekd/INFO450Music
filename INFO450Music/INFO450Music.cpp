// INFO450Music.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

//SongNode is the node in singly linked list
//that contains one record: Artist,Song

class SongNode {
public:
	//constructor
	SongNode(string, string);

	//get artist
	string getArtist();

	//get song
	string getSong();

	//get next
	SongNode* getNext();

	//set next
	void setNext(SongNode*);

private:
	//record
	string artist;
	string song;

	//link to next node
	SongNode* next;
};

/*
PlayList represents a singly linked list
that contains records
*/
class PlayList {
public:
	//constructor
	PlayList();

	//destructor
	~PlayList();

	//add record
	void add(string, string);

	//remove current
	void remove();

	//skip
	void skip(int);

	//move to next song in list
	void nextSong();

	//get current song
	SongNode* getCurrentSong();

	//write to file
	void write(ofstream& ofs);

private:
	SongNode* head; //head of list
	SongNode* currentSong; //current song to play
};

//constructor
SongNode::SongNode(string artist, string song) {
	this->artist = artist;
	this->song = song;
	next = NULL;
}

//get artist
string SongNode::getArtist() {
	return artist;
}

//get song
string SongNode::getSong() {
	return song;
}

//get next
SongNode* SongNode::getNext() {
	return next;
}

//set next
void SongNode::setNext(SongNode* next) {
	this->next = next;
}

//constructor
PlayList::PlayList() {
	head = NULL;
	currentSong = NULL;
}

//destructor
PlayList::~PlayList() {
	SongNode* temp; //delete node
	SongNode* current = head;

	while (current != NULL)
	{
		temp = current;
		current = current->getNext();

		delete (temp);
	}
}

//add record
void PlayList::add(string artist, string song) {
	SongNode *newNode = new SongNode(artist, song);
	if (head == NULL)
	{
		head = newNode;
		currentSong = newNode;
	}
	else {
		//move to tail node
		SongNode *current = head;
		while (current->getNext() != NULL)
		{
			current = current->getNext();
		}
		//add as tail node
		current->setNext(newNode);
	}
}

//remove current
void PlayList::remove() {

	/*ignore empty list*/
	if (head == NULL)
	{
		return;
	}

	SongNode *prev = NULL;
	SongNode *current = head;

	while (current != currentSong)
	{
		prev = current;
		current = current->getNext();
	}

	//current will be deleted
	if (prev == NULL)
	{//delete head node

		if (currentSong == head)
		{
			currentSong = head->getNext();
		}
		head = head->getNext();
	}
	else {
		prev->setNext(current->getNext());
		currentSong = currentSong->getNext();
		if (currentSong == NULL)
		{
			currentSong = head;
		}
	}

	delete current;
}

//skip
void PlayList::skip(int n) {

	/*ignore empty list*/
	if (head == NULL)
	{
		return;
	}

	for (int i = 0; i < n; i++)
	{
		//move current to end of list
		SongNode *prev = NULL;
		SongNode *current = head;

		while (current != currentSong)
		{
			prev = current;
			current = current->getNext();
		}

		//current is currentSong now

		//find tail node
		SongNode *tail = head;
		while (tail != NULL && tail->getNext() != NULL)
		{
			tail = tail->getNext();
		}


		//no need to move the tail node
		if (current == tail)
		{
			return;
		}

		//move current to tail
		if (prev == NULL)//current node is head node
		{

			head = head->getNext();

			current->setNext(NULL);
			tail->setNext(current);

			currentSong = head;
		}
		else {

			//set current song
			currentSong = currentSong->getNext();
			if (currentSong == NULL)
			{
				currentSong = head;
			}

			prev->setNext(current->getNext());
			current->setNext(NULL);
			tail->setNext(current);


		}
	}
}

//move to next song in list
void PlayList::nextSong() {
	if (currentSong != NULL)
	{
		currentSong = currentSong->getNext();

		if (currentSong == NULL)
		{
			currentSong = head;
		}
	}
}

//get current song
SongNode* PlayList::getCurrentSong() {
	return currentSong;
}

//write to file
void PlayList::write(ofstream& ofs) {
	//current node to iterate list
	SongNode *current = head;

	//iterate list
	while (current != NULL)
	{
		//write to stream
		ofs << current->getArtist() << "," << current->getSong() << endl;

		current = current->getNext();//next node
	}
}

int main() {

	//linked list
	PlayList list;

	//input file name
	string filename;

	//a line
	string line;

	//record
	string artist, song;

	//current song node
	SongNode *currentSong;

	//prompt for file name
	cout << "Please enter a full path filename of an existing playlist text file: ";
	getline(cin, filename);

	//open a file in read mode
	ifstream infile;
	infile.open(filename.c_str());

	//check if file can be opened
	if (infile.is_open()) {
		//read line by line
		while (!infile.eof()) {

			getline(infile, line);

			stringstream ss(line);
			getline(ss, artist, ',');
			getline(ss, song);

			//add to list
			list.add(artist, song);
		}

		//close the opened file
		infile.close();
	}
	else {
		cout << "File not found" << endl;
	}

	//display menu
	string command = "";

	//run until user wants to quit
	while (command != "0") {

		cout << "1. add song" << endl;
		cout << "2. play song" << endl;
		cout << "3. skip song" << endl;
		cout << "4. delete song" << endl;
		cout << "5. save to file" << endl;
		cout << "0. quit" << endl;

		cout << "You choice? ";
		getline(cin, command);

		if (command == "1")//add song
		{
			cout << "Enter artist: ";
			getline(cin, artist);

			cout << "Enter song: ";
			getline(cin, song);

			list.add(artist, song);
		}
		else if (command == "2") {//play song
			currentSong = list.getCurrentSong();
			if (currentSong == NULL)
			{
				cout << "There is no song" << endl;
			}
			else {
				cout << currentSong->getSong() << " (" << currentSong->getArtist() << ") is playing" << endl;
			}

			list.nextSong();
		}
		else if (command == "3") {//skip song
			cout << "Enter n: ";
			getline(cin, line);

			istringstream iss(line);
			int n;
			iss >> n;

			list.skip(n);
		}
		else if (command == "4") {//delete song
			list.remove();
		}
		else if (command == "5") {//save to file

			ofstream outfile;
			outfile.open(filename.c_str());

			//check if file can be opened
			if (outfile.is_open()) {

				//write to file
				list.write(outfile);

				//close file
				outfile.close();
			}
			else {
				cout << "Could not write to file" << endl;
			}
		}
		else if (command == "0") {//quit

		}
		else {
			cout << "Invalid selection" << endl;
		}
	}

	return 0;
}



