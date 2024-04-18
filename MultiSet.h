#include <iostream>
#include <fstream>
#pragma once

const int BUCKET_SIZE = 8;

class MultiSet
{
private:
	int n = 0;//max num
	uint8_t k = 0;
	int size = 0;
	uint8_t* multiSet = nullptr;

	bool isKValid(uint8_t) const;
	bool isInSet(int n) const;
	bool isEmpty(int id) const;

	int getBucketId(int id) const;
	int getPosInBucket(int id) const;
	int getBegId(int num) const;
	int getLastId(int num) const;

	void printNumById(int id) const;

	void writeToBinary(std::ofstream& ofs) const;
	void readFromBinary(std::ifstream& ifs);
public:
	MultiSet() {
		k = 3;
		n = 7;
		multiSet = new uint8_t[2]{ 81,7 };
	}
	MultiSet(int n, uint8_t k);
	void add(int n);//bool?
	int countNumInSet(int num) const;
	void print() const;

	void writeToBinaryFile(const char* filename) const;
	void readFromBinaryFile(const char* filename);

	friend MultiSet intersection(const MultiSet& lhs, const MultiSet& rhs);
	~MultiSet();
};

