#ifndef REF_BASE_H
#define REF_BASE_H

#include <stdint.h> //int32_t
#include <stdio.h>
#include <pthread.h> //pthread_mutex_t
#include <string.h> // strerror()
#include <errno.h> // errno
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define DEBUG_REFS 1
#define DEBUG_REFS_ENABLED_BY_DEFAULT 1
#define DEBUG_REFS_CALLSTACK_PATH "/dev/shm"
#define LOG_ALWAYS_FATAL_IF LOG_ASSERT
#define LOG_ASSERT(condition, format...) \
    do {if (!(condition)) {printf("Assert failed: " #condition ": "format);}}while(0);
#define LOG_WLWAYS_FATAL(format...) \
    do { printf(format); } while(0);
#define ALOGD printf
#define ALOGE printf
#define DEBUG() printf("%s():%d\n", __func__, __LINE__)

template<typename T> class wp;

typedef int32_t status_t;
typedef char String8;

class Mutex {
public:
    enum {
        PRIVATE = 0,
        SHARED = 1
    };

    Mutex();
    Mutex(const char* name);
    Mutex(int type, const char* name = NULL);
    ~Mutex();

    // lock or unlock the mutex
    status_t   lock();
    void       unlock();

    // lock if possiable; return 0 on success, error otherwise
    status_t   tryLock();

    // Manages the mutex automatically, It'll be locked when Autolock is
    // constructed and released when Autolock goes out of scope.
    class Autolock {
    public:
        inline Autolock(Mutex& mutex) : mLock(mutex) { mLock.lock(); }
        inline Autolock(Mutex* mutex) : mLock(*mutex) { mLock.lock(); }
        inline ~Autolock() { mLock.unlock(); }
    private:
        Mutex& mLock;
    };

private:
//    friend class Condition;

    // A mutex cannot be copied
               Mutex(const Mutex&);
    Mutex&     operator = (const Mutex&);

    pthread_mutex_t mMutex;
};

inline Mutex::Mutex()
{
    pthread_mutex_init(&mMutex, NULL);
}

inline Mutex::Mutex(__attribute__((unused)) const char* name)
{
    pthread_mutex_init(&mMutex, NULL);
}

inline Mutex::Mutex(int type, __attribute__((unused)) const char* name)
{
    if (type == SHARED) {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&mMutex, &attr);
        pthread_mutexattr_destroy(&attr);
    } else {
        pthread_mutex_init(&mMutex, NULL);
    }
}

inline Mutex::~Mutex()
{
    pthread_mutex_destroy(&mMutex);
}

inline status_t Mutex::lock()
{
    return pthread_mutex_lock(&mMutex);
}

inline void Mutex::unlock()
{
    pthread_mutex_unlock(&mMutex);
}

inline status_t Mutex::tryLock()
{
    return pthread_mutex_trylock(&mMutex);
}

typedef Mutex::Autolock AutoMutex;

/*
 * macro COMPARE()
 */
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

#define COMPARE_WEAK(_op_)                             \
inline bool operator _op_ (const sp<T>& o) const {     \
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
inline bool operator _op_ (const U* o) const {         \
    return m_ptr _op_ o;                               \
}
/*
 * const函数为何可以更改?
 * static_cast是什么东西?
 */
int32_t android_atomic_cas(int32_t old_value, int32_t new_value, volatile int32_t *ptr)
{
    int32_t prev;
    __asm__ __volatile__ ("lock; cmpxchgl %1, %2"
                          : "=a" (prev)
                          : "q" (new_value), "m" (*ptr), "0" (old_value)
                          : "memory");
    return prev != old_value;
}

static inline int32_t android_atomic_or(int32_t value, volatile int32_t *ptr)
{
    int32_t prev, status;
    do {
        prev = *ptr;
        status = android_atomic_cas(prev, prev | value, ptr);
    } while (__builtin_expect(status != 0, 0));
    return prev;
}
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

static inline int32_t android_atomic_cmpxchg(int32_t old_value, int32_t new_value, volatile int32_t *ptr)
{
    return android_atomic_cas(old_value, new_value, ptr);
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

        bool    attemptIncStrong(const void* id);
        int32_t getWeakCount() const;
        void    printRefs() const;
        void    trackMe(bool enable, bool retain);
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

    //! Flags for extendObjectLifetime()
    enum {
        OBJECT_LIFETIME_WEAK    = 0x0001,
        OBJECT_LIFETIME_FOREVER = 0x0003,
    };

    void extendObjectLifetime(int32_t mode);

    //! Flags for onStrongAttempted()
    enum {
        FIRST_INC_STRONG = 0x0001,
    };

    virtual void onFirstRef();
    virtual void onLastStrongRef(const void* id);
    virtual bool onIncStrongAttempted(uint32_t flags, const void* id);
    virtual void onLastWeakRef(const void* id);

private:
    friend class weakref_type;
    class weakref_impl;

    RefBase(const RefBase& o);
    RefBase& operator= (const RefBase& o);

    weakref_impl* const mRefs;
};

#define INITIAL_STRONG_VALUE (1 << 28)

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
        , mStrongRefs(new ref_entry)
        , mWeakRefs(new ref_entry)
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
        addRef(&mWeakRefs, id, mWeak);
    }

    void removeWeakRef(const void* id)
    {
        if (!mRetain) {
            removeRef(&mWeakRefs, id);
        } else {
            addRef(&mWeakRefs, id, -mWeak);
        }
    }

    void trackMe(bool track, bool retain)
    {
        mTrackEnabled = track;
        mRetain = retain;

    }

    void printRefs() const
    {
#if 0
        String8 text; //TODO

        {
            Mutex::Autolock _l(mMutex);
            char buf[128];
            sprintf(buf, "Strong references on RefBase %p (weakref_tyep %p):\n", mBase, this);
            text.append(buf);
            printRefsLocked(&text, mStrongRefs);
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
#endif
    }

private:
    struct ref_entry
    {
        struct ref_entry* next;
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
            LOG_WLWAYS_FATAL("RefBase: add id %p on RefBase %p"
                                "(weakref_type  %p) that doesn't exist!\n",
                                id, mBase, this);
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
                                "(weakref_type  %p) that doesn't exist!\n",
                                id, mBase, this);
        }
    }

    void renameRefsId(ref_entry* r, const void* old_id, const void* new_id)
    {
        if (mTrackEnabled) {
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
            char inc = refs->ref >= 0 ? '+' : '-';
            sprintf(buf, "\t%c ID %p (ref %d):\n",
                    inc, refs->id, refs->ref);
#if DEBUG_REFS_CALLBACK_ENABLED
            out->append(refs->stack.toStrong("\t\t"));
#else
            //out->append("\t\t(call stacks disabled");
            printf("\t\t(call stacks disabled\n");
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

RefBase::weakref_type* RefBase::createWeak(const void *id) const
{
    mRefs->incWeak(id);
    return mRefs;
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
        if (impl->mStrong == INITIAL_STRONG_VALUE) {
            delete impl->mBase;
        } else {
//            LOGV("Freeing refs %p of old RefBase %p\n", this, impl->mBase);
            delete impl;
        }
    } else {
        impl->mBase->onLastWeakRef(id);
        if ((impl->mFlags & OBJECT_LIFETIME_FOREVER) != OBJECT_LIFETIME_FOREVER) {
            delete impl->mBase;
        }
    }
}

bool RefBase::weakref_type::attemptIncStrong(const void* id)
{
    incWeak(id);

    weakref_impl* const impl = static_cast<weakref_impl*>(this);

    int32_t curCount = impl->mStrong;
    LOG_ASSERT(curCount >= 0, "attemptIncStrong called on %p after underflow", this);

    while (curCount > 0 && curCount != INITIAL_STRONG_VALUE) {
        if (android_atomic_cmpxchg(curCount, curCount + 1, &impl->mStrong) == 0) {
            break;
        }
        curCount = impl->mStrong;
    }

    if (curCount <= 0 || curCount == INITIAL_STRONG_VALUE) {
        bool allow;
        if (curCount == INITIAL_STRONG_VALUE) {
            // Attempting to acquire first strong reference... this is allowed
            // if the object does NOT have a longer lifetime(meaning the
            // implementaion doesn't need to see this), or if the implementation
            // allows it to happen.
            allow = (impl->mFlags & OBJECT_LIFETIME_WEAK) != OBJECT_LIFETIME_WEAK
                || impl->mBase->onIncStrongAttempted(FIRST_INC_STRONG, id);
        } else {
            // Attempting to revive the object... this is allowed
            // if the object DOES have a longer lifetime (so we can safely
            // call the object with only a weak ref) and the implementation
            // allows it to happen.
            allow = (impl->mFlags & OBJECT_LIFETIME_WEAK) == OBJECT_LIFETIME_WEAK
                && impl->mBase->onIncStrongAttempted(FIRST_INC_STRONG, id);

        }
        if (!allow) {
            decWeak(id);
            return false;
        }
        curCount = android_atomic_inc(&impl->mStrong);

        // If the strong reference count has already been incremented by
        // someone else, the implementor of onIncStrongAttemped() is holding
        // an unneeded reference. So call onLastStrongRef() here to remove it.
        // (No, this is not pretty.) Note that we MUST NOT do this if we
        // are in fact acquiring the first reference.
        if (curCount > 0 && curCount < INITIAL_STRONG_VALUE) {
            impl->mBase->onLastStrongRef(id);
        }
    }

    impl->addWeakRef(id);
    impl->addStrongRef(id);

#if PRINT_REFS
    LOGD("attemptIncStrong of %p from %p: cnt=%d\n", this, id, curCount);
#endif

    if (curCount == INITIAL_STRONG_VALUE) {
        android_atomic_add(-INITIAL_STRONG_VALUE, &impl->mStrong);
        impl->mBase->onFirstRef();
    }

    return true;
}

int32_t  RefBase::weakref_type::getWeakCount() const
{
    return static_cast<const weakref_impl*>(this)->mWeak;
}

void RefBase::onFirstRef() { }
void RefBase::onLastStrongRef(const void* id) { }
bool RefBase::onIncStrongAttempted(uint32_t flags, const void* id)
{
    return (flags & FIRST_INC_STRONG) ? true : false;
}

void RefBase::onLastWeakRef(const void* id) { }

RefBase::~RefBase()
{
//    LOGV("Destroying RefBase %p (refs %p)\n", this, mRefs);
    if (mRefs->mWeak == 0) {
//        LOGV("Freeing refs %p of old RefBase %p\n", mRefs, this);
        delete mRefs;
    }
}

void RefBase::extendObjectLifetime(int32_t mode)
{
    android_atomic_or(mode, &mRefs->mFlags);
}

int32_t RefBase::getStrongCount() const
{
    return mRefs->mStrong;
}

RefBase::weakref_type* RefBase::getWeakRefs() const
{
    return mRefs;
}

/*
 * strong pointer
 */
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
sp<T>::sp(T *other, weakref_type* refs)
    :m_ptr((other && refs->attemptIncStrong(this)) ? other : 0)
{
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

/*
 * weak pointer
 * 弱指针的最大特点是它不能直接操作目标对象
 */
template <typename T>
class wp
{
public:
    typedef typename RefBase::weakref_type weakref_type; // typename主要作用是告诉complier一个“变量”是一个新的“类型”

    inline wp() : m_ptr(0) { }

    wp(T* other);
    wp(const wp<T>& other);
    wp(const sp<T>& other);
    template<typename U> wp(U* other);
    template<typename U> wp(const sp<U>& other);
    template<typename U> wp(const wp<U>& other);

    ~wp();

    // Assignment
    wp& operator = (T* other);
    wp& operator = (const wp<T>& other);
    wp& operator = (const sp<T>& other);

    void set_object_and_refs(T* other, weakref_type* refs);

    // promotion to sp
    sp<T> promote() const;

    // Reset
    void clear();

    inline weakref_type* get_refs() const { return m_refs; }

    inline T* unsafe_get() const { return m_ptr; }

    // Operators
    COMPARE_WEAK(==)
    COMPARE_WEAK(!=)
    COMPARE_WEAK(>)
    COMPARE_WEAK(<)
    COMPARE_WEAK(>=)
    COMPARE_WEAK(<=)

    inline bool operator == (const wp<T>& o) const {
        return (m_ptr == o.m_ptr) && (m_refs == o.m_refs);
    }

    template<typename U>
    inline bool operator == (const wp<U>& o) const {
        return m_ptr == o.m_ptr;
    }

    inline bool operator > (const wp<T>& o) const {
        return (m_ptr == o.m_ptr) ? (m_refs > o.m_refs) : (m_ptr > o.m_ptr);
    }

    inline bool operator < (const wp<T>& o) const {
        return (m_ptr == o.m_ptr) ? (m_refs < o.m_refs) : (m_ptr < o.m_ptr);
    }

    template<typename U>
    inline bool operator < (const wp<U>& o) const {
        return (m_ptr == o.m_ptr) ? (m_refs < o.m_refs) : (m_ptr < o.m_ptr);
    }

    inline bool operator != (const wp<T>& o) { return m_refs != o.m_refs; }
    template<typename U> inline bool operator != (const wp<U>& o) const { return !operator == (o); }
    inline bool operator <= (const wp<T>& o) const { return !operator > (o); }
    template<typename U> inline bool operator <= (const wp<U>& o) const { return !operator > (o); }
    inline bool operator >= (const wp<T>& o) const { return !operator < (o); }
    template<typename U> inline bool operator >= (const wp<U>& o) const { return !operator < (o); }

private:
    template<typename Y> friend class sp;
    template<typename Y> friend class wp;

    T*               m_ptr;
    weakref_type*    m_refs;

};

template<typename T>
wp<T>::wp(T* other)
    : m_ptr(other)
{
    if (m_ptr) {
        m_refs = other->createWeak(this);
    }
}

template<typename T>
wp<T>::~wp()
{
    if (m_ptr) {
        m_refs->decWeak(this);
    }
}

template<typename T>
sp<T> wp<T>::promote() const
{
    return sp<T>(m_ptr, m_refs);
}
#endif
