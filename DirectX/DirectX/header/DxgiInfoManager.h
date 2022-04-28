#pragma once
#include "Win.h"
#include <vector>
#include <string>

class DxgiInfoManager
{
public:
	DxgiInfoManager();
	~DxgiInfoManager();
	DxgiInfoManager(const DxgiInfoManager&) = delete;
	DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;
	void Set();
	std::vector<std::string> GetMessage() const;

private:
	unsigned long long next = 0u;
	struct IDXGIInfoQueue* pDxgiInfoQueue = nullptr;
};