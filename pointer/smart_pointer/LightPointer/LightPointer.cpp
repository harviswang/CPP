#include <stdio.h>
#include "RefBase.h"

static void lp_test(void);
static void sp_test(void);
int main(int argc, char **argv)
{
    lp_test();
    sp_test();

    return 0;
}

class LightClass : public LightRefBase<LightClass> {
public:
    LightClass() {
        printf("Construct LightClass Object\n");
    }
    ~LightClass() {
        printf("Destory LightClass Object\n");
    }
};

/*
 * light pointer test
 */
static void lp_test(void)
{
    printf("%s() test...\n", __func__);
    // 指针转换, 实际是调用拷贝构造函数
    {
        LightClass *pLightClass = new LightClass();
        sp<LightClass> lpOut = pLightClass;

        printf("Line:%d LightClass Ref Count: %d.\n", __LINE__, pLightClass->getStrongCount());
        {
            sp<LightClass> lpInner = lpOut;
            printf("Line:%d LightClass Ref Count: %d.\n", __LINE__, pLightClass->getStrongCount());
        }

        printf("Line:%d LightClass Ref Count: %d.\n", __LINE__, pLightClass->getStrongCount());
    }
    // 指针调用拷贝构造函数
    {
        sp<LightClass> lp = new LightClass();
    }

    //delete pLightClass;
}

class WeightClass : public RefBase
{
public:
    void printRefCount() {
        int32_t strong = getStrongCount();
        weakref_type* ref = getWeakRefs();

        printf("Strong Ref Count: %d.\n", (strong == INITIAL_STRONG_VALUE) ? 0 : strong);
        printf("Weak Ref Count: %d.\n", ref->getWeakCount());
    }
};

class StrongClass : public WeightClass
{
public:
    StrongClass() {
        printf("Construct StrongClass Object\n");
    }
    ~StrongClass() {
        printf("Destory StrongClass Object\n");
    }
};

class WeakClass : public WeightClass
{
public:
    WeakClass() {
        extendObjectLifetime(OBJECT_LIFETIME_WEAK);
        printf("Construct WeakClass Object.\n");
    }

    virtual ~WeakClass() {
        printf("Destroy WeakClass Object.\n");
    }
};

class ForeverClass : public WeightClass
{
public:
    ForeverClass() {
        extendObjectLifetime(OBJECT_LIFETIME_FOREVER);
        printf("Construct ForeverClass Object.\n");
    }

    virtual ~ForeverClass() {
        printf("Destroy ForeverClass Object.\n");
    }
};

void TestStrongClass(StrongClass* pStrongClass)
{
    wp<StrongClass> wpOut = pStrongClass;
    pStrongClass->printRefCount();

    {
        sp<StrongClass> spInner = pStrongClass;
        pStrongClass->printRefCount();
    }

    sp<StrongClass> spOut = wpOut.promote();
    printf("spOut: %p.\n", spOut.get());
}

void TestWeakClass(WeakClass* pWeakClass)
{
    wp<WeakClass> wpOut = pWeakClass;
    pWeakClass->printRefCount();

    {
        sp<WeakClass> spInner = pWeakClass;
        pWeakClass->printRefCount();
    }

    pWeakClass->printRefCount();
    sp<WeakClass> spOut = wpOut.promote();
    printf("spOut: %p.\n", spOut.get());
}

void TestForeverClass(ForeverClass* pForeverClass)
{
    wp<ForeverClass> wpOut = pForeverClass;
    pForeverClass->printRefCount();

    {
        sp<ForeverClass> spInner = pForeverClass;
        pForeverClass->printRefCount();
    }
}
/*
 * strong pointer test
 */
static void sp_test(void)
{
    printf("Test StrongClass: \n");
    StrongClass* pStrongClass = new StrongClass();
    TestStrongClass(pStrongClass);

//    printf("\nTest WeakClass: \n");
//    WeakClass* pWeakClass = new WeakClass();
//    TestWeakClass(pWeakClass);
//
//    printf("\nTest ForeverClass: \n");
//    ForeverClass* pForeverClass = new ForeverClass();
//    TestForeverClass(pForeverClass);
//    pForeverClass->printRefCount();
//    delete pForeverClass;
}
