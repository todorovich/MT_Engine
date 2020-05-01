#pragma once

#include "precompiled.hpp"

namespace mt 
{
	template<class Type, int number_of_objects>
	class ObjectPool
	{
	private:
		std::vector<Type>			_data;
		std::priority_queue<int, std::vector<int>, std::greater<int>> 	_unused_indicies;
		std::set<int>				_used_indicies;

	public:

		ObjectPool()
			: _data(vector<Type>(number_of_objects))
		{
			for (auto i = 0; i < number_of_objects; i++)
			{
				_unused_indicies.push(i);
			}
		}

		~ObjectPool() = default;

		ObjectPool(const ObjectPool& other) = delete;

		ObjectPool(ObjectPool&& other) = delete;

		ObjectPool operator=(const ObjectPool& other) = delete;
		
		ObjectPool operator=(ObjectPool&& other) = delete;

		Type* GetMemory()
		{
			if (_unused_indicies.size() == 0)
			{
			    // To Do: This was previously throw, not sure what that changes.
				return nullptr;
			}

			auto index = _unused_indicies.top();

			_unused_indicies.pop();

			_used_indicies.insert(index);

			return &_data[index];

		}

		void ReleaseMemory(Type* returned_memory)
		{
			int index = static_cast<int>(returned_memory - &_data[0]);

			if (index < 0 || index >= number_of_objects)
			{
				return;
			}

			*returned_memory = Type();

			_used_indicies.erase(index);

			_unused_indicies.push(index);

		}
	};

	
}