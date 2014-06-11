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
#ifndef __SETTABLE_H_
#define __SETTABLE_H_

namespace opendnp3
{

// utility class for representing a value that may or may not be set
template <class T>
class Settable
{
public:

	Settable() : valueIsSet(false)
	{}

	Settable(const T& default) : valueIsSet(false), value(default)
	{}

	bool IsSet() const
	{
		return valueIsSet;
	}

	T Get() const
	{
		return value;
	}

	bool Pop(T& output)
	{
		if (valueIsSet)
		{
			valueIsSet = false;
			output = value;
			return true;
		}
		else
		{
			return false;
		}
	}

	void Clear()
	{
		valueIsSet = false;
	}

	void Set(const T& value_)
	{		
		value = value_;
		valueIsSet = true;		
	}

	template <class Action>
	bool Foreach(Action action);


private:

	bool valueIsSet;
	T value;
};

template <class T>
template <class Action>
bool Settable<T>::Foreach(Action action)
{
	if (valueIsSet)
	{
		action(value);
		return true;
	}
	else
	{
		return false;
	}
}

}


#endif

