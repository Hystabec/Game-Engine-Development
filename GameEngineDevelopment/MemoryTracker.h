#pragma once

#include <iostream>

struct AllocationMetric 
{
	uint32_t TotalAlloc = 0;
	uint32_t TotalFreed = 0;

	uint32_t CurrentUsage() { return TotalAlloc - TotalFreed; }
	void Reset() { TotalAlloc = 0; TotalFreed = 0; }
};

static AllocationMetric s_AllocationMetrics;

static void PrintMemoryUsage()
{
	std::cout << "Total Allocated: " << s_AllocationMetrics.TotalAlloc << " bytes |";
	std::cout << "Total Freed: " << s_AllocationMetrics.TotalFreed << " bytes |";
	std::cout << "Total Remaining: " << s_AllocationMetrics.CurrentUsage() << " bytes\n";
}

void* operator new(size_t size)
{
	s_AllocationMetrics.TotalAlloc += size;
	return malloc(size);
}

void operator delete(void* mem, size_t size)
{
	s_AllocationMetrics.TotalFreed += size;	
	free(mem);
}