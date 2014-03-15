/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef __LINKED_LIST_ADAPTER_H_
#define __LINKED_LIST_ADAPTER_H_

#include "Indexable.h"

namespace openpal
{

template <class ValueType>
class ListNode
{
public:
	ListNode(): prev(nullptr), next(nullptr)
	{}

	ValueType value;

private:
	ListNode* prev;
	ListNode* next;

	template <class T, class U>
	friend class LinkedListAdapter;

	template <class T>
	friend class LinkedListIterator;
};


template <class ValueType>
class LinkedListIterator
{
public:
	LinkedListIterator(ListNode< ValueType>* pStart) : pCurrent(pStart)
	{}

	bool HasNext()
	{
		return (pCurrent != nullptr);
	}

	ListNode< ValueType>* Next()
	{
		assert(pCurrent != nullptr);
		auto pRet = pCurrent;
		pCurrent = pCurrent->next;
		return pRet;
	}

private:
	ListNode<ValueType>* pCurrent;
};


// A container adapter for a -linked list
template <class ValueType, class IndexType>
class LinkedListAdapter : public HasSize<IndexType>
{
public:

	typedef LinkedListIterator<ValueType> Iterator;

	LinkedListAdapter(Indexable<ListNode<ValueType>, IndexType> aUnderlying) :
		HasSize<IndexType>(0),
		pHead(nullptr),
		pTail(nullptr),
		pFree(nullptr),
		underlying(aUnderlying)
	{
		Initialize();
	}

	void Clear()
	{
		Initialize();
		this->size = 0;
	}

	inline ListNode<ValueType>* Head()
	{
		return pHead;
	}

	Iterator Iterate()
	{
		return Iterator(pHead);
	}

	ListNode<ValueType>* Add(const ValueType& value);

	void Remove(ListNode<ValueType>* apNode);

	inline bool IsFull() const;

private:
	ListNode<ValueType>* pHead;
	ListNode<ValueType>* pTail;
	ListNode<ValueType>* pFree;
	Indexable<ListNode<ValueType>, IndexType> underlying;

	inline static void Link(ListNode<ValueType>* prev, ListNode<ValueType>* next);

	void Initialize();
};

template <class ValueType, class IndexType>
ListNode<ValueType>* LinkedListAdapter<ValueType, IndexType>::Add(const ValueType& value)
{
	if(pFree == nullptr) return nullptr;
	else
	{
		auto pNode = pFree;
		pNode->value = value;
		pFree = pFree->next;
		pNode->next = nullptr;
		Link(pTail, pNode);
		pTail = pNode;
		if(pHead == nullptr) pHead = pTail;
		++(this->size);
		return pTail;
	}
}

template <class ValueType, class IndexType>
void LinkedListAdapter<ValueType, IndexType>::Remove(ListNode<ValueType>* apNode)
{
	if(apNode->prev == nullptr) // it's the head
	{
		if(apNode->next == nullptr) pHead = pTail = nullptr; // list is now empty
		else pHead = apNode->next; // head but not tail
	}
	else
	{
		if(apNode->next == nullptr) pTail = apNode->prev; // was only the tail
	}

	// attach the adjacent nodes to eachother if they exist
	Link(apNode->prev, apNode->next);

	// Now that the data list is complete, attach the freed node to the front of the free list
	apNode->next = pFree;
	if(pFree != nullptr) pFree->prev = apNode;
	apNode->prev = nullptr; // it's the head now
	pFree = apNode;
	--(this->size);
}

template <class ValueType, class IndexType>
bool LinkedListAdapter<ValueType, IndexType>::IsFull() const
{
	return (pFree == nullptr);
}


template <class ValueType, class IndexType>
void LinkedListAdapter<ValueType, IndexType>::Link(ListNode<ValueType>* first, ListNode<ValueType>* second)
{
	if(first) first->next = second;
	if(second) second->prev = first;
}

template <class ValueType, class IndexType>
void LinkedListAdapter<ValueType, IndexType>::Initialize()
{
	if(underlying.IsNotEmpty())
	{
		pFree = &underlying[0];
		for(IndexType i = 1; i < underlying.Size(); ++i)
		{
			Link(&underlying[i - 1], &underlying[i]);
		}
	}
}

}

#endif
