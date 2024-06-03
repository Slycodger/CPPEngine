#pragma once
#ifndef LIST
#define LIST

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
		Delete();
	}
	type* operator [] (int Index)
	{
		if (Index >= Length || Index < 0)
		{
			return nullptr;
		}
		return &list[Index];
	}
	type AddToList(type Thing)
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
		for (int i = 0; i < Length; i++)
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
	void DeleteListObj()
	{
		delete[](list);
		list = nullptr;
		Length = 0;
		Allocated = 0;
	}
	bool Contains(const type& Thing)
	{
		for (int i = 0; i < Length; i++)
		{
			if (*list[i] == Thing)
				return true;
		}
		return false;
	}
	bool Contains(const type& Thing, unsigned int& Pos)
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
	void RemoveFromList(int Index)
	{
		if (Index >= Length || Index < 0)
		{
			return;
		}
		type* Temp = new type[Allocated];
		int Obj = 0;
		for (int i = 0; i < Length; i++)
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
	void RemoveObjFromList(type& Thing)
	{
		RemoveFromList(GetIndex(Thing));
	}
	void Delete()
	{
		if (list == nullptr)
			return;
		delete[](list);
		list = nullptr;
		Length = 0;
		Allocated = 0;
	}
	type* ListPtr()
	{
		type* Ret = new type[Length];

		for (int i = 0; i < Length; i++)
		{
			Ret[i] = list[i];
		}

		return Ret;
	}
	int GetIndex(type& Obj)
	{
		for (int i = 0; i < Length; i++)
		{
			if (list[i] == Obj)
				return i;
		}
		return -1;
	}
	void NewList(int Count)
	{
		Delete();
		list = new type[Count];
		Allocated = Count;
		Length = 0;
	}
};

#endif