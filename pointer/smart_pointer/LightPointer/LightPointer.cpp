#include <stdio.h>
#include "RefBase.h"

static void lp_test(void);
static void sp_test(void);
int main(int argc, char **argv)
{
    //lp_test();
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

class StrongClass : public RefBase
{
public:
    StrongClass() { printf("Construct StrongClass Object\n"); }
    ~StrongClass() { printf("Destory StrongClass Object\n"); }
};
/*
 * strong pointer test
 */
static void sp_test(void)
{
    printf("%s() test...\n", __func__);
    sp<StrongClass> sc = new StrongClass();
}
