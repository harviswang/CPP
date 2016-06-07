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
    virtual ~RefBase() { };

    //! Flags for extendObjectLifetime()
    enum {
        OBJECT_LIFETIME_WEAK    = 0x0001,
        OBJECT_LIFETIME_FOREVER = 0x0003,
    };

    void extendOjectLifetime(int32_t mode);

    //! Flags for onStrongAttempted()
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

#define INITIAL_STRONG_VALUE (1 << 28)

#define LOG_ASSERT(condition, format...) \
    do {if (!(condition)) {printf("Assert failed: " #condition ": "format);}}while(0);

class RefBase::weakref_impl : public RefBase::weakref_type
{
public:
    volatile int32_t mStrong;
    volatile int32_t mWeak;
    RefBase* const   mBase;
    volatile int32_t mFlags;

#if !DEBUG_REFS
    weakref_impl(RefBase* base)
        : mStrong(INITIAL_STRONG_VALUE)
        , mWeak(0)
        , mBase(base)
        , mFlags(0)
    { }

    void addStrongRef(const void* /*id*/) { }
    void removeStrongRef(const void* /*id*/) { }
    void addWeakRef(const void* /*id*/) { }
    void removeWeakRef(const void* /*id*/) { }
    void printRefs() const { }
    void trackMe(bool, bool) { }

#else
    weakref_impl(RefBase* base)
        : mStrong(INITIAL_STRONG_VALUE)
        , mWeak(0)
        , mBase(0)
        , mFlags(0)
        , mStrongRefs(NULL)
        , mWeakRefs(NULL)
        , mTrackEnabled(!!DEBUG_REFS_ENABLED_BY_DEFAULT)
        , mRetain(false)
    {
        //LOGI("New weakref_impl %p for RefBase %p", this, base);
    }

    ~weakref_impl()
    {
        LOG_ALWAYS_FATAL_IF(!mRetain && mStrongRefs != NULL, "Strong references remain!");
        LOG_ALWAYS_FATAL_IF(!mRetain && mWeakRefs != NULL, "Weak references remain");
    }

    void addStrongRef(const void* id)
    {
        addRef(&mStrongRefs, id, mStrong);
    }

    void removeStrongRef(const void* id)
    {
        if (!mRetain) {
            removeRef(&mStrongRefs, id);
        } else {
            addRef(&mStrongRefs, id, -mStrong);
        }
    }

    void addWeakRef(const void* id)
    {
        addRef(&mStrongRefs, id, mWeak)
    }

    void removeWeakRef(const void* id)
    {
        if (!mRetain) {
            removeRef(&mWeak, id);
        } else {
            addRef(&mWeak, id, -mWeak);
        }
    }

    void trackMe(bool track, bool retain)
    {
        mTrackEnable = track;
        mRetain = retain;

    }

    void printRefs() const
    {
        String8 text; //TODO

        {
            Mutex::Autolock _l(mMutext); //TODO
            char buf[128];
            sprintf(buf, "Strong references on RefBase %p (weakref_tyep %p):\n", mBase, this);
            text.append(buf);
            printfRefsLocked(&text, mStrongRefs);
            sprintf(buf, "Weak references on RefBase %p (weakref_type %p):\n", mBase, this);
            text.append(buf);
            printRefsLocked(&text, mWeakRefs);
        }

        {
            char name[100];
            snprintf(name, 100, DEBUG_REFS_CALLSTACK_PATH "/%p.stack", this);
            int rc = open(name, O_RDWR | O_CREAT | O_APPEND, 0644);
            if (rc >= 0 ) {
                write(rc, text.string(), text.length());
                close(rc);
                ALOGD("STACK TRACE for %p saved in %s", this, name);
            } else {
                ALOGE("FAILED TO PRINT STACK TRACE for %p in %s: %s", this , name, strerror(errno));
            }
        }
    }

private:
    struct ref_entry
    {
        ref_entry* next;
        const void* id;
#if DEBUG_REFS_CALLSTACK_ENABLED
        CallStack stack;
#endif
        int32_t ref;
    };

    void addRef(ref_entry** refs, const void* id, int32_t mRef)
    {
        if (mTrackEnabled) {
            AutoMutex _l(mMutex);
            ref_entry* ref = new ref_entry;

            ref->ref = mRef;
            ref->id = id;
#if DEBUG_REFS_CALLSTACK_ENABLED
            ref->stack.update(2);
#endif
            ref->next = *refs;
            *refs = ref;
        }
    }

    void removeRef(ref_entry** refs, const void* id)
    {
        if (mTrackEnabled) {
            AutoMutex _l(mMutex);

            ref_entry* ref = *refs;
            while (ref->id != NULL) {
                if (ref->id == id) {
                    *refs = ref->next;
                    delete ref;
                    return;
                }

                refs = &ref->next;
                ref = *refs;
            }

            LOG_WLWAYS_FATAL("RefBase: removing id %p on RefBase %p"
                                "(weakref_type  %p) that doesn't exist!",
                                id, mBase, this);
        }
    }

    void renameRefsId(ref_entry* r, const void* old_id, const void* new_id)
    {
        if (mTrackEanbled) {
            AutoMutex _l(mMutex);
            ref_entry* ref = r;
            while (ref != NULL) {
                if (ref->id == old_id) {
                    ref->id = new_id;
                }
                ref = ref->next;
            }
        }
    }

    void printRefsLocked(String8* out, const ref_entry* refs) const
    {
        char buf[128];
        while (refs) {
            char inc = ref->ref >= 0 ? '+' : '-';
            sprintf(buf, "\t%c ID %p (ref %d):\n",
                    inc, refs->id, refs->ref);
#if DEBUG_REFS_CALLBACK_ENABLED
            out->append(refs->stack.toStrong("\t\t"));
#else
            out->append("\t\t(call stacks disabled");
#endif
            refs = refs->next;
        }
    }

    Mutex mMutex;
    ref_entry* mStrongRefs;
    ref_entry* mWeakRefs;

    bool mTrackEnabled;
    // Collect stack traces on addref and removeref, instead of deleting the stack references
    // on removeref that match the address ones
    bool mRetain;

#endif
};

void RefBase::incStrong(const void* id) const
{
    weakref_impl* const refs = mRefs;
    refs->addWeakRef(id);
    refs->incWeak(id);
    refs->addStrongRef(id);

    const int32_t c = android_atomic_inc(&refs->mStrong);
    LOG_ASSERT(c > 0, "incStrong() called on %p after last strong ref\n", refs);

#if PRINT_REFS
    LOGD("incStrong of %p from %p: cnt=%d\n", this, id, c);
#endif

    if (c != INITIAL_STRONG_VALUE) {
        return ;
    }

    android_atomic_add(-INITIAL_STRONG_VALUE, &refs->mStrong);
    const_cast<RefBase *>(this)->onFirstRef();
}

RefBase::RefBase()
    : mRefs(new weakref_impl(this))
{
//  LOGV("Creating refs %p with RefBase %p\n", mRefs, this);
}

void RefBase::weakref_type::incWeak(const void* id)
{
    weakref_impl* const impl = static_cast<weakref_impl*>(this);
    impl->addWeakRef(id);
    const int32_t c = android_atomic_inc(&impl->mWeak);
    LOG_ASSERT(c >= 0, "incWeak called on %p after last weak ref\n", this);
}

void RefBase::decStrong(const void* id) const
{
    weakref_impl* const refs = mRefs;
    refs->removeStrongRef(id);
    const int32_t c = android_atomic_dec(&refs->mStrong);
#if PRINT_REFS
    LOGD("decStrong of %p from %p: cnt=%d\n", this, id, c);
#endif
    LOG_ASSERT(c >= 1, "decStrong() called on %p too many  times\n", refs);
    if (c == 1) {
        const_cast<RefBase*>(this)->onLastStrongRef(id);
        if ((refs->mFlags & OBJECT_LIFETIME_WEAK) != OBJECT_LIFETIME_WEAK) {
            delete this;
        }
    }
    refs->removeWeakRef(id);
    refs->decWeak(id);
}

void RefBase::weakref_type::decWeak(const void* id)
{
    weakref_impl* const impl = static_cast<weakref_impl*>(this);
    impl->removeWeakRef(id);
    const int32_t c = android_atomic_dec(&impl->mWeak);
    LOG_ASSERT(c >= 1, "decWeak called on %p too many times\n", this);
    if (c != 1) return;

    if ((impl->mFlags & OBJECT_LIFETIME_WEAK) != OBJECT_LIFETIME_WEAK) {

    }
}
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
