/*

	minimal STL vector for embedded C++

	http://www.novadsp.com/code

	Copyright (c) Jerry Evans, 1992-2009
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	* Redistributions of source code must retain the above copyright notice,
	this list of conditions and the following disclaimer.

	* Redistributions in binary form must reproduce the above copyright notice,
	this list of conditions and the following disclaimer in the documentation
	and/or other materials provided with the distribution.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
	THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
	PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
	BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
	CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
	GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
	HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
	STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
	IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef MU_VECTOR_H
#define MU_VECTOR_H

#include <stddef.h>

namespace Chordia
{

// allocation alignment
template <typename T,int alignment = 4>
class vector
{
	// how many elements
	size_t m_elements;

	// pointer to the buffer
	T*	m_pData;
	
	// initialize everything
	void init(size_t elements)
	{
		if (m_pData)
		{
			// JME audit: what if not POD?
			free(m_pData);
			m_pData = 0;
		}
		m_elements = 0;
		if (elements > 0)
		{
			void* p = 0;
			//
#ifdef _MSC_VER
			m_pData = new T[elements];
			if (m_pData)
			{
				m_elements = elements;
			}
#else
			if (posix_memalign(&p,alignment,elements * sizeof(T)) == 0)
			{
				m_pData = reinterpret_cast<T*>(p);
				m_elements = elements;
			}
#endif
		}
	}
	
	public:
	
	// plain constructor
	vector() : 
	m_pData(0), m_elements(0)	
	{

	}

	// constructor taking initial number of elements
	// and value with which to initialize
	vector(size_t elements,const T& arg) : 
		m_pData(0), m_elements(0)	
	{
		assign(elements,arg);
	}
	
	// destructor cleans up
	~vector()
	{
		init(0);
	}
	
	// returns the number of elements
	size_t size() const	
	{ 
		return m_elements; 
	}
	
	// STL style
	void assign(size_t elements,const T& arg)
	{
		// allocate
		init(elements);
		// initialize
		memset(m_pData,arg,elements * sizeof(T));
	}
	
	// array access notation for set
	T& operator[](size_t index)
	{
		return (*(m_pData + index));
	}
	
	// array access notation for get
	const T& operator[](size_t index) const
	{
		return (*(m_pData + index));
	}
};

}	// namespace

#endif
