#pragma once
#ifndef LIST
#define LIST
#include "GlobalVars.h"
template<class type>
struct adjType
{
	type *Thing = nullptr;
	void Delete()
	{
		delete(Thing);
		Thing = nullptr;
	}
	void operator = (const type Obj)
	{
		if (Thing == nullptr)
			Thing = new type(0);
		*Thing = Thing;
	}
	void operator = (const adjType Obj)
	{
		if (Thing == nullptr)
			Thing = new type ();
		*Thing = *Obj.Thing;
	}
};

template <class type>
struct List
{
	int Length = 0;
	adjType<type> *list = nullptr;
	List(int Count) : Length(Count) 
	{
		if (Count <= 0)
			return;
		list = new adjType<type>[Count];
	}
	type* operator [] (int Index)
	{
		if (Index >= Length)
		{
			Fails::Break = true;
			Fails::BreakCode = -2;
			return nullptr;
		}
		if (list[Index].Thing == nullptr)
			list[Index].Thing = new type ();
		return list[Index].Thing;
	}
	void AddToList(type* Thing)
	{
		for (int i = 0; i < Length; i++)
		{
			if (list[i].Thing == nullptr)
			{
				list[i].Thing = Thing;
				return;
			}
		}
		adjType<type>* Temp = nullptr;
		if (Length > 0)
			Temp = new adjType<type>[Length + 100] {nullptr};
		else
			Temp = new adjType<type>[100] {nullptr};
		for (int i = 0; i < Length; i++)
		{
			Temp[i].Thing = list[i].Thing;
		}
		Temp[Length].Thing = Thing;
		delete[](list);
		list = nullptr;
		Length++;
		list = Temp;
		Temp = nullptr;
	}
	void DeleteListObj()
	{
		delete[](list);
		list = nullptr;
		Length = 0;
	}
	bool Contains(type &Thing)
	{
		for(int i = 0; i < Length; i++)
		{
			if (*list[i].Thing == Thing)
				return true;
		}
		return false;
	}
	void RemoveFromList(int Index)
	{
		if (list[Index].Thing == nullptr)
		{
			Fails::Break = true;
			Fails::BreakCode = -2;
			return;
		}
		adjType<type>* Temp = new adjType<type>[Length - 1];
		int Obj = 0;
		for(int i = 0; i < Length; i++)
		{
			if (i == Index)
				continue;
			Temp[Obj] = list[i];
			Obj++;
		}
		for (int i = 0; i < Length; i++)
		{
			list[i].Delete();
		}
		delete[](list);
		list = Temp;
		Length--;
	}
	void RemoveObjFromList(type& Thing)
	{
		for (int i = 0; i < Length; i++)
		{
			if (*list[i].Thing == Thing)
				RemoveFromList(i);
		}
	}
	void Delete()
	{
		for(int i = 0; i < Length; i++)
		{
			list[i].Delete();
		}
		delete[](list);
		list = nullptr;
		Length = 0;
	}
	type* ListPtr()
	{
		type* Ret = new type[Length];

		for(int i = 0; i < Length; i++)
		{
			Ret[i] = *list[i].Thing;
		}

		return Ret;
	}
	int GetIndex(type &Obj)
	{
		for(int i = 0; i < Length; i++)
		{
			if (*list[i].Thing == Obj)
				return i;
		}
		return -1;
	}
	int OpenSlot()
	{
		for(int i = 0; i < Length; i++)
		{
			if (list[i].Thing = nullptr)
				return i;
		}
		return -1;
	}
	void NewList(int Count)
	{
		Delete();
		list = new adjType<type>[Count];
	}
};

#endif