#include <stdio.h>
#include <stdint.h>

static int mTrackEnabled = true;

struct ref_entry
{
    struct ref_entry* next;
    const void* id;

    int32_t ref;
};

void addRef(ref_entry** refs, const void* id, int32_t mRef)
{
    if (mTrackEnabled) {
        ref_entry* ref = new ref_entry;

        ref->ref = mRef;
        ref->id = id;

        ref->next = *refs;
        *refs = ref;
    }
}

bool removeRef(ref_entry** refs, const void* id)
{
    if (mTrackEnabled) {

        ref_entry* ref = *refs;
        while (ref->id != NULL) {
            if (ref->id == id) {
                *refs = ref->next;
                delete ref;
                return true;
            }

            if (ref->next) {
                refs = &ref->next;
                ref = *refs;
            } else { // no more entry
                break;
            }
        }
    }
    return false;
}

void printRef(ref_entry** refs)
{
    printf("next = %p\n", (*refs)->next);
    printf("id = %p\n", (*refs)->id);
    printf("ref = %d\n", (*refs)->ref);
}

void dumpRefList(ref_entry** refs)
{
    if (refs != NULL && *refs != NULL) {
        for (ref_entry* ref = *refs; ref != NULL; ref = ref->next) {
            printRef(&ref);
        }
    } else  {
        if (!refs) {
            printf("refs is NULL\n");
        } else {
            printf("*refs is NULL\n");
        }
    }
}

/*
 * refs充当表头的角色(总是指向表头的那个entry), 初始化应该是NULL或者new ref_entry
 */
int main(int argc, char **argv)
{
    const int MAX = 10;
    struct ref_entry *refs = new ref_entry;
    int number[MAX];
    int i;
    bool ok;

    printf("sizeof(ref_entry) = %ld\n", sizeof(ref_entry));
    printf("refs = %p\n", refs);
    printRef(&refs);

    // add MAX entry
    for (i = 0; i < MAX; i++) {
        addRef(&refs, (void *)&number[i], i);
    }

    dumpRefList(&refs);

    // delete MAX entry
    for (i = MAX - 1; i >= 0; i--) {
        ok = removeRef(&refs, (void *)&number[i]);
        printf("refs = %p ok = %d\n", refs, ok);
    }

    printf("refs = %p\n", refs);

    // remain refs
    delete refs;

    return 0;
}
