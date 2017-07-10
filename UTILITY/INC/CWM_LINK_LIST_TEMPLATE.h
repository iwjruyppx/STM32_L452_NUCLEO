
#ifndef __CWM_LINK_LIST_TEMPLATE_H
#define __CWM_LINK_LIST_TEMPLATE_H

#ifdef USE_LINK_LIST_TEMPLATE
typedef struct {
    uint32_t evtHandle;
    void *evtData;
    void (*evtCbfunc)(void *, void *);
} LinkListData_t, *pLinkListData_t;

typedef struct LinkListS_t {
    struct LinkListS_t *next;
    struct LinkListS_t *prev;
    union {
        void *pdata;
        pLinkListData_t pLinkLiskdata;
    };
}LinkList_t, *pLinkList_t;

typedef struct LinkListH_t{
    pLinkList_t Head;
    pLinkList_t Tail;
    int (*AddToHead)(struct LinkListH_t *, pLinkListData_t);
    int (*AddToTail)(struct LinkListH_t *, pLinkListData_t);
    pLinkListData_t (*Search)(struct LinkListH_t *, uint32_t);
    int (*FindAndCallBack)(struct LinkListH_t *, uint32_t, void *);
    int (*Remove)(struct LinkListH_t *, uint32_t);

    /*User need to implement this API*/
    CWM_MALLOC malloc;
    CWM_FREE free;
    /*User need to implement this API End*/
} LinkListHandle_t, *pLinkListHandle_t;

void CWM_LINK_LIST_INIT(pLinkListHandle_t Handle);

#endif /*USE_LINK_LIST_TEMPLATE*/
#endif /* __CWM_LINK_LIST_TEMPLATE_H */
