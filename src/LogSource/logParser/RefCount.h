#pragma once
#include <windows.h>

class RefCount
{
    mutable volatile long m_Crefs;
    public:
    
    RefCount(void) { m_Crefs = 0; }
    
    virtual ~RefCount() {}
    
    int UpCount(void) const
    {
        return InterlockedIncrement(&m_Crefs);
    }
    
    int DownCount(void) const
    {
        int val = InterlockedDecrement(&m_Crefs);
        if (!val) 
	{
            delete this;
	}
        return val;
    }
};

template <class T> class Ptr
{
    T* m_Ptr;
    public:

    Ptr(const Ptr<T>& other) : m_Ptr(other.m_Ptr)
    {
        if (m_Ptr)
        {
            m_Ptr->UpCount();
        }
    }
    
    Ptr(T* ptr = 0) : m_Ptr(ptr)
    {
        if (m_Ptr)
        {
            m_Ptr->UpCount();
        }
    }
    
    ~Ptr(void)
    {
        if (m_Ptr)
        {
            m_Ptr->DownCount();
        }
    }

    operator T*(void) const { return m_Ptr; }
    
    operator T*(void) { return m_Ptr; }
    
    T& operator*(void) const { return *m_Ptr; }
    
    T& operator*(void) { return *m_Ptr; }

	T* operator->(void) const { return m_Ptr; }
    
    T* operator->(void) { return m_Ptr; }
    
    bool operator == (const T* ptr) const { return (m_Ptr == ptr); }
    
    bool operator == (const Ptr<T> &ptr) const { return (m_Ptr == ptr.m_Ptr); }
    
    bool operator != (const T * ptr) const { return (m_Ptr != ptr); }
    
    bool operator != (const Ptr<T> &ptr) const { return (m_Ptr != ptr.m_Ptr); }

	bool operator !() const { return (m_Ptr == 0); }
    
    Ptr& operator=(Ptr<T> &ptr) {return operator=((T *) ptr);}
    
    Ptr& operator=(T* ptr)
    {
        if (m_Ptr != ptr)
        {
            if (m_Ptr) 
            {
                m_Ptr->DownCount(); 
            }
            m_Ptr = ptr; 
            if (m_Ptr)
            {
                m_Ptr->UpCount();
            }
        }
        return *this;
    }
};

