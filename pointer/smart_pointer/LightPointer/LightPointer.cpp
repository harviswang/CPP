#include <stdio.h>
#include "RefBase.h"

static void sp_test(void);
int main(int argc, char **argv)
{
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
static void sp_test(void)
{
    // 指针转换, 实际是调用拷贝构造函数
    {
        LightClass *pLightClass = new LightClass();
        sp<LightClass> lpOut = pLightClass;

        printf("LightClass Ref Count: %d.\n", pLightClass->getStrongCount());
        {
            sp<LightClass> lpInner = lpOut;
            printf("LightClass Ref Count: %d.\n", pLightClass->getStrongCount());
        }

        printf("LightClass Ref Count: %d.\n", pLightClass->getStrongCount());
    }
    // 指针调用拷贝构造函数
    {
        sp<LightClass> lp = new LightClass();
    }

    //delete pLightClass;
}
