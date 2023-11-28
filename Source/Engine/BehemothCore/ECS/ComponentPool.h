#pragma once

#include <vector>
#include "SparseSet.h"
#include "Entity.h"

class IComponentPool
{
public:
	virtual ~IComponentPool() = default;

 	virtual std::size_t size()
 	{
 		return std::size_t{};
 	}

	virtual std::vector<Entity>& GetEntities()
	{
		std::vector<Entity> data;
		return data;
	}
 
};

template<typename T>
class ComponentPool : public IComponentPool, public SparseSet<T>
{
public:
	ComponentPool() = default;

    std::size_t size() override 
    {
    	return SparseSet<T>::size();
    }

	virtual std::vector<Entity>& GetEntities()
	{
		return SparseSet<T>::dense;
	}


};