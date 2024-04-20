#include "MultiSet.h"

bool MultiSet::isKValid(uint8_t k) const
{
	return ( k >= 1 && k<=8);
}

bool MultiSet::isInSet(int n) const
{
	return (n>=0 && n <= this->n);
}

int MultiSet::getBucketId(int id) const
{
	return id / BUCKET_SIZE;
}

int MultiSet::getPosInBucket(int id) const
{
	return id % BUCKET_SIZE;
}

int MultiSet::getBegId(int num) const
{
	return num * k;
}

int MultiSet::getLastId(int num) const
{
	return ((num + 1) * k) - 1;
}

bool MultiSet::isInTwoBuckets(int num) const
{
	int begId = getBegId(num);
	int lastId = getLastId(num);

	int bucket = getBucketId(begId);

	for (int i = begId + 1; i <= lastId; i++) {
		if (bucket != getBucketId(i)) {
			return true;
		}
	}

	return false;
}

int MultiSet::getFirstIdInSecondBucket(int num) const
{
	int begId = getBegId(num);
	int lastId = getLastId(num);

	int bucket = getBucketId(begId);

	for (int i = begId + 1; i <= lastId; i++) {
		if (bucket != getBucketId(i)) {
			return i;
		}
	}
	return -1;
}

void MultiSet::addInTwoBuckets(int num, int newCount)
{
	int begId = getBegId(num);
	int lastId = getLastId(num);

	int idInFirstBucket = getFirstIdInSecondBucket(num) - begId;//how many of the indexes are in the first bucket

	removeNumTwoBuckets(num);

	int countSecond = newCount >> idInFirstBucket;
	int countFirst = newCount - (countSecond << idInFirstBucket);

	multiSet[getBucketId(begId)] |= (countFirst << getPosInBucket(begId));
	multiSet[getBucketId(lastId)] |= countSecond;
}

void MultiSet::addInOneBucket(int num, int newCount)
{
	int begId = getBegId(num);

	removeNumOneBucket(num);
	
	multiSet[getBucketId(begId)] |= (newCount << getPosInBucket(begId));
}

void MultiSet::removeNumTwoBuckets(int num)
{
	int begId = getBegId(num);
	int lastId = getLastId(num);

	int idInFirstBucket = getFirstIdInSecondBucket(num) - begId;//how many of the indexes are in the first bucket

	uint8_t mask1 = ~(((1 << idInFirstBucket) - 1) << getPosInBucket(begId));
	uint8_t mask2 = ~((1 << (k - idInFirstBucket)) - 1);

	multiSet[getBucketId(begId)] &= mask1;
	multiSet[getBucketId(lastId)] &= mask2;
}

void MultiSet::removeNumOneBucket(int num)
{
	int mask = ~(((1 << k) - 1) << getPosInBucket(getBegId(num)));

	multiSet[getBucketId(getBegId(num))] &= mask;
}

uint8_t MultiSet::getCountTwoBuckets(int num) const
{
	int begId = getBegId(num);
	int lastId = getLastId(num);

	int idInFirstBucket = getFirstIdInSecondBucket(num) - begId;//how many of the indexes are in the first bucket

	uint8_t mask1 = ((1 << idInFirstBucket) - 1) << getPosInBucket(begId);
	uint8_t mask2 = (1 << (k - idInFirstBucket)) - 1;

	uint8_t res1 = (multiSet[getBucketId(begId)] & mask1 ) >> getPosInBucket(begId);
	uint8_t res2 = multiSet[getBucketId(lastId)] & mask2;

	return (res2 << idInFirstBucket) + res1;
}

uint8_t MultiSet::getCountOneBucket(int num) const
{
	int begId = getBegId(num);
	uint8_t mask = (((1 << k) - 1) << getPosInBucket(begId));

	return ((multiSet[getBucketId(begId)] & mask) >> getPosInBucket(begId));
}

void MultiSet::printInBinary(uint8_t num)
{
	for (int i = BUCKET_SIZE - 1; i >= 0; i--) {// the least significant bit is rightmost (00100000 = 32) 
		uint8_t mask = (1 << i);                //if this is the first byte of a multiSet with k = 3  --> 00100000
		std::cout << ((num & mask) >> i) ;                                                            //  22111000
	}
	std::cout << " ";
}

void MultiSet::writeToBinary(std::ofstream& ofs) const
{
	ofs.write((const char*)&n, sizeof(n));//n
	ofs.write((const char*)&k, sizeof(k));//k
	ofs.write((const char*)&size, sizeof(size));//size
	ofs.write((const char*)multiSet, sizeof(uint8_t) * size);
}

void MultiSet::readFromBinary(std::ifstream& ifs)
{
	ifs.read((char*)&n, sizeof(n));
	ifs.read((char*)&k, sizeof(k));
	ifs.read((char*)&size, sizeof(size));

	free();
	multiSet = new uint8_t[size];
	ifs.read((char*)multiSet, sizeof(uint8_t) * size);
}

void MultiSet::copyFrom(const MultiSet& other)
{
	n = other.n;
	k = other.k;
	size = other.size ;

	multiSet = new uint8_t[size];

	for (int i = 0; i < size; i++) {
		multiSet[i] = other.multiSet[i];
	}
}

void MultiSet::free()
{
	delete[] multiSet;
}

void MultiSet::moveFrom(MultiSet&& other)
{
	n = other.n;
	k = other.k;
	size = other.size ;

	multiSet = other.multiSet;

	other.multiSet = nullptr;
}

MultiSet::MultiSet(int n, uint8_t k)
{
	this->n = n;

	if(isKValid(k))//error handling
		this->k = k;

	size = ((n * k) / 8) + 1;

	multiSet = new uint8_t[size]{ 0 };
}

MultiSet::MultiSet(const MultiSet& other)
{
	copyFrom(other);
}

MultiSet& MultiSet::operator=(const MultiSet& other)
{
	if (this != &other) {
		free();
		copyFrom(other);
	}
	return *this;
}

MultiSet::MultiSet(MultiSet&& other) noexcept
{
	moveFrom(std::move(other));
}

MultiSet& MultiSet::operator=(MultiSet&& other) noexcept
{
	if (this != &other) {
		free();
		moveFrom(std::move(other));
	}
	return *this;
}

void MultiSet::add(int num)
{
	if (!isInSet(n)) {
		//error handling
	}

	int newCount = countNumInSet(num) + 1;

	if (newCount > ((1 << k) - 1)) {
		std::cout << num << " is already added " << (int)k << " times ";
		return;
	}

	if (isInTwoBuckets(num)) {
		addInTwoBuckets(num, newCount);
	}
	else {
		addInOneBucket(num, newCount);
	}
	
	
}

uint8_t MultiSet::countNumInSet(int num) const
{
	if (!isInSet(num)) {
		//error handling
	}

	if (isInTwoBuckets(num)) {
		return getCountTwoBuckets(num);
	}
	else {
		return getCountOneBucket(num);
	}

	return 0;
}

void MultiSet::print() const
{
	for (int i = 0; i <= n; i++) {
		if (countNumInSet(i)>0) {
			std::cout << i << " ";
		}
	}
	std::cout << std::endl;
}

void MultiSet::printMem() const
{
	for (int i = 0; i < size; i++) {
		printInBinary((int)multiSet[i]);
	}
	std::cout << std::endl;
}

void MultiSet::writeToBinaryFile(const char* filename) const
{
	std::ofstream ofs(filename, std::ios::out | std::ios::binary);

	if (!ofs.is_open()) {
		//error handling
		std::cout << "Unable to open file";
	}

	writeToBinary(ofs);
}

void MultiSet::readFromBinaryFile(const char* filename)
{
	std::ifstream ifs(filename, std::ios::in | std::ios::binary);
	
	if (!ifs.is_open()) {
		//error handling
	}

	readFromBinary(ifs);
}

MultiSet::~MultiSet()
{
	free();
}

MultiSet intersection(const MultiSet& lhs, const MultiSet& rhs)
{
	int n = std::min(lhs.n, rhs.n);
	int k = std::min(lhs.k, rhs.k);

	MultiSet ms(n, k);

	for (int i = 0; i <= n; i++) {
		int countlhs = lhs.countNumInSet(i);
		int countRhs = rhs.countNumInSet(i);

		int count = std::min(countlhs, countRhs);

		for (int j = 0; j < count; j++) {
			ms.add(i);
		}
	}
	return ms;
}

MultiSet operator-(const MultiSet& lhs, const MultiSet& rhs)
{
	int n = lhs.n;
	int k = lhs.k;

	MultiSet ms(n, k);

	for (int i = 0; i <= ms.n; i++) {
		int countlhs = lhs.countNumInSet(i);
		int countRhs = rhs.countNumInSet(i);

		int count = countlhs - countRhs;

		for (int j = 0; j < count; j++) {
			ms.add(i);
		}
	}
	return ms;
}

MultiSet addition(const MultiSet& ms)
{
	int n = ms.n;
	int k = ms.k;

	MultiSet ms1(n, k);

	for (int i = 0; i <= ms1.n; i++) {

		int maxcount = ((1 << ms.k) - 1);
		int count = maxcount - ms.countNumInSet(i);

		for (int j = 0; j < count; j++) {
			ms1.add(i);
		}
	}
	return ms1;
}
