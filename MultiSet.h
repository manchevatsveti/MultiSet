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

	int getBucketId(int id) const;
	int getPosInBucket(int id) const;
	int getBegId(int num) const;
	int getLastId(int num) const;

	bool isInTwoBuckets(int num) const;
	int getFirstIdInSecondBucket(int num) const;
	void addInTwoBuckets(int num, int newCount);
	void addInOneBucket(int num, int newCount);
	void removeNumTwoBuckets(int num);
	void removeNumOneBucket(int num);

	uint8_t getCountTwoBuckets(int num) const;
	uint8_t getCountOneBucket(int num) const;


	void writeToBinary(std::ofstream& ofs) const;
	void readFromBinary(std::ifstream& ifs);

	void copyFrom(const MultiSet& other);
	void free();
	void moveFrom(MultiSet&& other);

	static void printInBinary(uint8_t num);

public:

	MultiSet(int n, uint8_t k);
	MultiSet(const MultiSet& other);
	MultiSet& operator=(const MultiSet& other);
	MultiSet(MultiSet&& other) noexcept;
	MultiSet& operator=(MultiSet&& other) noexcept;

	void add(int n);
	uint8_t countNumInSet(int num) const;
	void print() const;
	void printMem() const;

	void writeToBinaryFile(const char* filename) const;
	void readFromBinaryFile(const char* filename);

	friend MultiSet intersection(const MultiSet& lhs, const MultiSet& rhs);
	friend MultiSet operator-(const MultiSet& lhs, const MultiSet& rhs);
	friend MultiSet addition(const MultiSet& ms);
	~MultiSet();
};

