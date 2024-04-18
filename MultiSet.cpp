#include "MultiSet.h"

bool MultiSet::isKValid(uint8_t k) const
{
	return ( k >= 1 && k<=8);
}

bool MultiSet::isInSet(int n) const
{
	return (n>=0 && n <= this->n);
}

bool MultiSet::isEmpty(int id) const
{
	int bucketId = getBucketId(id);
	int posInBucket = getPosInBucket(id);
	
	uint8_t mask = 1 << posInBucket;
	if (multiSet[bucketId] & mask) {
		return false;//it is not an empty index
	}
	else {
		return true;
	}
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

void MultiSet::printNumById(int id) const
{
	std::cout << (id / k) + 1 << " ";
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

	delete[] multiSet;//free()?
	multiSet = new uint8_t[size];
	ifs.read((char*)multiSet, sizeof(uint8_t) * size);
}

MultiSet::MultiSet(int n, uint8_t k)
{
	this->n = n;

	if(isKValid(k))//error handling
		this->k = k;

	size = n * k;

	multiSet = new uint8_t[size];
}

void MultiSet::add(int num)
{
	if (!isInSet(n)) {
		//error handling
	}

	int newCount = countNumInSet(num) + 1;

	if (newCount >= ((1 << k) - 1)) {
		std::cout << "Already k nums added";
		return;
	}

	int begId = getBegId(num);
	int lastId = getLastId(num);

	int bucket = getBucketId(begId);
	bool isInTwoBuckets = false;

	int firstIdSecBucket = -1;
	for (int i = begId + 1; i <= lastId; i++) {
		if (bucket != getBucketId(i)) {
			firstIdSecBucket = i;
			isInTwoBuckets = true;
			break;
		}
	}

	if (isInTwoBuckets) {
		int idInFirstBucket = firstIdSecBucket - begId;//how many of the indexes are in the first bucket
		multiSet[getBucketId(begId)] &= ~(((1 << idInFirstBucket) - 1) << getPosInBucket(begId));
		multiSet[getBucketId(lastId)] &= ~((1 << (k - idInFirstBucket)) - 1);

		int countSecond = newCount >> idInFirstBucket;
		int countFirst = newCount - (countSecond << idInFirstBucket);
		multiSet[getBucketId(begId)] |= (countFirst << getPosInBucket(begId));
		multiSet[getBucketId(lastId)] |= countSecond;
	}
	else {
		multiSet[getBucketId(begId)] &= ~(((1 << k) - 1) << getPosInBucket(begId));
		multiSet[getBucketId(begId)] |= (newCount << getPosInBucket(begId));
	}
	
	
}

int MultiSet::countNumInSet(int num) const
{
	if (!isInSet(num)) {
		//error handling
	}

	int begId = getBegId(num);
	int lastId = getLastId(num);

	int bucket = getBucketId(begId);
	bool isInTwoBuckets = false;

	int firstIdSecBucket = -1;
	for (int i = begId + 1; i <= lastId; i++) {
		if (bucket != getBucketId(i)) {
			firstIdSecBucket = i;
			isInTwoBuckets = true;
			break;
		}
	}
	int res = 0;

	if (isInTwoBuckets) {
		int idInFirstBucket = firstIdSecBucket - begId;//how many of the indexes are in the first bucket
		uint8_t res1 = ((multiSet[getBucketId(begId)] & (((1 << idInFirstBucket) - 1) << getPosInBucket(begId))) >> getPosInBucket(begId));
		uint8_t res2 = multiSet[getBucketId(lastId)] & ((1 << (k-idInFirstBucket)) - 1);

		res = (res2 << idInFirstBucket) + res1;
	}
	else {
		res = ((multiSet[getBucketId(begId)] & (((1 << k) - 1) << getPosInBucket(begId))) >> getPosInBucket(begId));
	}

	return res;
}

void MultiSet::print() const
{
	for (int i = 0; i < size; i++) {
		if (!isEmpty(i)) {
			printNumById(i);
		}
	}
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
	delete[] multiSet;
}

MultiSet intersection(const MultiSet& lhs, const MultiSet& rhs)
{
	int n = std::min(lhs.n, rhs.n);
	int k = std::min(lhs.k, rhs.k);

	for (int i = 0; i < n; i++) {
		int countlhs = lhs.countNumInSet(i);
		int countRhs = rhs.countNumInSet(i);

		int count = std::min(countlhs, countRhs);
	}
	return MultiSet(0,0);
}
