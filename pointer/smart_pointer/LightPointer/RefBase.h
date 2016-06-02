#ifndef REF_BASE_H
#define REF_BASE_H

#include <stdint.h> //int32_t
#include <stdio.h>

template<typename T> class wp;

#define COMPARE(_op_)                                  \
inline bool operator _op_ (const sp<T>& o) const {     \
    return m_ptr _op_ o.m_ptr;                         \
}                                                      \
inline bool operator _op_ (const wp<T>& o) const {     \
    return m_ptr _op_ o.m_ptr;                         \
}                                                      \
inline bool operator _op_ (const T* o) const {         \
    return m_ptr _op_ o;                               \
}                                                      \
template<typename U>                                   \
inline bool operator _op_ (const sp<U>& o) const {     \
    return m_ptr _op_ o.m_ptr;                         \
}                                                      \
template<typename U>                                   \
inline bool operator _op_ (const wp<U>& o) const {     \
    return m_ptr _op_ o.m_ptr;                         \
}                                                      \
template<typename U>                                   \
inline bool operator _op_ (const U* o) const {         \
    return m_ptr _op_ o;                               \
}
/*
 * const函数为何可以更改?
 * static_cast是什么东西?
 */
static inline int32_t android_atomic_add(int32_t increment, volatile int32_t *ptr)
{
    __asm__ __volatile__ ("lock; xaddl %0, %1"
                          : "+r" (increment), "+m" (*ptr)
                          : : "memory");
    /* increment now holds the old value of *ptr */
    return increment;
}
static inline int32_t android_atomic_inc(volatile int32_t *val)
{
    return android_atomic_add(1, val);
}

static inline int32_t android_atomic_dec(volatile int32_t *val)
{
    return android_atomic_add(-1, val);
}

template <class T>
class LightRefBase {
public:
    inline LightRefBase(): mCount(0) { printf("%s()\n", __func__); }
    inline void incStrong(const void *id) const {
        android_atomic_inc(&mCount);
    }
    inline void decStrong(const void *id) const {
        if (android_atomic_dec(&mCount) == 1) {
            delete static_cast< const T*>(this); //TODO
        }
    }
    
    inline int32_t getStrongCount() const {
        return mCount;
    }

protected:
    inline ~LightRefBase() { printf("%s()\n", __func__); }

private:
    mutable volatile int32_t mCount;
};

class RefBase
{
public:
    void incStrong(const void* id) const;
    void decStrong(const void* id) const;

    void forceIncStrong(const void* id) const;

    int32_t getStrongCount() const;

    class weakref_type
    {
    public:
        RefBase* refBase() const;

        void incWeak(const void* id);
        void decWeak(const void* id);

        bool attemptIncStrong(const void* id);
        void getWeakCount() const;
        void printRefs() const;
        void trackMe(bool enable, bool retain);
    };

    weakref_type* createWeak(const void* id) const;
    weakref_type* getWeakRefs() const;

    inline void printRefs() const { getWeakRefs()->printRefs(); }
    inline void trackMe(bool enable, bool retain)
    {
        getWeakRefs()->trackMe(enable, retain);
    }

protected:
    RefBase();
    virtual ~RefBase();

    enum {
        OBJECT_LIFETIME_WEAK    = 0x0001,
        OBJECT_LIFETIME_FOREVER = 0x0003,
    };

    void extendOjectLifetime(int32_t mode);

    enum {
        FIRST_INC_STRONG = 0x0001,
    };

    virtual void onFirstRef();
    virtual void onLastStrongRef(const void* id);
    virtual void onIncStrongAttempted(uint32_t flags, const void* id);
    virtual void onLastWeakRef(const void* id);

private:
    friend class weakref_type;
    class weakref_impl;

    RefBase(const RefBase& o);
    RefBase& operator= (const RefBase& o);

    weakref_impl* const mRefs;
};

template <typename T>
class sp {
public:
    typedef typename RefBase::weakref_type weakref_type;

    inline sp() : m_ptr(0) {}

    sp(T *other);
    sp(const sp<T>& other);
    template<typename U> sp(U* other);
    template<typename U> sp(const sp<U>& other);

    ~sp();

    // Assignment
    sp& operator = (T* other);
    sp& operator = (const sp<T>& other);

    template<typename U> sp& operator = (const sp<U>& other);
    template<typename U> sp& operator = (U* other);

    void force_set(T* other);

    // Reset
    void clear();

    // Accessors
    inline T& operator* () const { return *m_ptr; }
    inline T* operator-> () const { return m_ptr; }
    inline T* get() const { return m_ptr; }

    // Operators
    COMPARE(==)
    COMPARE(!=)
    COMPARE(>)
    COMPARE(<)
    COMPARE(<=)
    COMPARE(>=)

private:
    template<typename Y> friend class sp;
    template<typename Y> friend class wp;

    // Optimization for wp::promote
    sp(T* p, weakref_type* refs);

    T* m_ptr;
};

template<typename T>
sp<T>::sp(T* other)
    : m_ptr(other)
{
    if (other) {
        other->incStrong(this);
    }
}

template<typename T>
sp<T>::sp(const sp<T>& other)
    : m_ptr(other.m_ptr)
{
    if (m_ptr) {
        m_ptr->incStrong(this);
    }
}

template<typename T>
sp<T>::~sp()
{
    if (m_ptr) {
        m_ptr->decStrong(this);
    }
}
#endif
