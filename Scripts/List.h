#pragma once

//decrepted script
//Mostly meant for custom lists that need weird functions

template <class type>
struct List
{
	unsigned int Length = 0;
	unsigned int Allocated = 0;
	type* list = nullptr;
	List(int Count) : Allocated(Count), Length(0)
	{
		if (Count <= 0)
			return;
		list = new type[Count];
	}
	~List()
	{
		deleteList();
	}
	type* operator [] (unsigned int Index)
	{
		if (Index >= Length || Index < 0)
		{
			return nullptr;
		}
		return &list[Index];
	}

	//Adds object of 'type' to list
	type addToList(type Thing)
	{
		if (Length < Allocated)
		{
			list[Length] = Thing;
			Length++;
			return Thing;
		}
		type* Temp = nullptr;
		if (Allocated > 0)
		{
			Temp = new type[Allocated * 2];
			Allocated *= 2;
		}
		else
		{
			Temp = new type[1];
			Allocated = 1;
		}
		for (unsigned int i = 0; i < Length; i++)
		{
			Temp[i] = list[i];
		}
		Temp[Length] = Thing;
		delete[](list);
		list = nullptr;
		Length++;
		list = Temp;
		Temp = nullptr;
		return Thing;
	}

	//returns true if list containts 'Thing'
	bool contains(const type& Thing)
	{
		for (int i = 0; i < Length; i++)
		{
			if (*list[i] == Thing)
				return true;
		}
		return false;
	}

	//returns true if list contains 'Thing' and sets 'Pos' to the index it is at
	bool contains(const type& Thing, unsigned int& Pos)
	{
		for (int i = 0; i < Length; i++)
		{
			if (list[i] == Thing)
			{
				Pos = i;
				return true;
			}
		}
		return false;
	}

	//Removes object from list at 'Index'
	void removeFromList(unsigned int Index)
	{
		if (Index >= Length || Index < 0)
		{
			return;
		}
		type* Temp = new type[Allocated];
		int Obj = 0;
		for (unsigned int i = 0; i < Length; i++)
		{
			if (i == Index)
				continue;
			Temp[Obj] = list[i];
			Obj++;
		}
		delete[](list);
		list = Temp;
		Length--;
	}

	//Removes object from list if 'Thing' is in list
	void removeObjFromList(type& Thing)
	{
		RemoveFromList(GetIndex(Thing));
	}

	//Deletes the lists internal buffer
	void deleteList()
	{
		if (list == nullptr)
			return;
		delete[](list);
		list = nullptr;
		Length = 0;
		Allocated = 0;
	}

	//Returns a pointer to the lists internal buffer
	type* listPtr()
	{
		type* Ret = new type[Length];

		for (int i = 0; i < Length; i++)
		{
			Ret[i] = list[i];
		}

		return Ret;
	}

	//Returns the index of an object in the list if it exists
	int getIndex(type& Obj)
	{
		for (int i = 0; i < Length; i++)
		{
			if (list[i] == Obj)
				return i;
		}
		return -1;
	}

	//Deletes the current list and starts a new one
	void newList(int Count)
	{
		deleteList();
		list = new type[Count];
		Allocated = Count;
		Length = 0;
	}
};