
/*Standard Utility include file*/
#include "CWM_UTILITY.h"

#include "CWM_LINK_LIST_TEMPLATE.h"

#ifdef USE_LINK_LIST_TEMPLATE

static int CWM_LINK_LIST_ADD_TO_HEAD(pLinkListHandle_t Handle, pLinkListData_t pLinkLiskdata)
{
    pLinkList_t ptr = NULL;
    pLinkListData_t ptrData = NULL;

    /*Alloc memory*/
    ptr = Handle->malloc(sizeof(LinkList_t));
    if(NULL == ptr)
        return CWM_ERROR_MEMORY_ALLOC_FAIL;
    
    ptrData = Handle->malloc(sizeof(LinkListData_t));
    if(NULL == ptrData)
    {
        Handle->free(ptr);
        return CWM_ERROR_MEMORY_ALLOC_FAIL;
    }
    memcpy(ptrData, pLinkLiskdata, sizeof(LinkListData_t));

    ptr->next = NULL;
    ptr->prev = NULL;
    ptr->pLinkLiskdata = ptrData;
    
    if(Handle->Head == NULL)
    {
        Handle->Head = ptr;
        Handle->Tail = ptr;
    }
    else
    {
        ptr->next = Handle->Head;
        Handle->Head->prev = ptr;
        Handle->Head = ptr;
    }

    return CWM_NON;
}
static int CWM_LINK_LIST_ADD_TO_TAIL(pLinkListHandle_t Handle, pLinkListData_t pLinkLiskdata)
{
    pLinkList_t ptr = NULL;
    pLinkListData_t ptrData = NULL;

    /*Alloc memory*/
    ptr = Handle->malloc(sizeof(LinkList_t));
    if(NULL == ptr)
        return CWM_ERROR_MEMORY_ALLOC_FAIL;
    
    ptrData = Handle->malloc(sizeof(LinkListData_t));
    if(NULL == ptrData)
    {
        Handle->free(ptr);
        return CWM_ERROR_MEMORY_ALLOC_FAIL;
    }
    memcpy(ptrData, pLinkLiskdata, sizeof(LinkListData_t));

    ptr->next = NULL;
    ptr->prev = NULL;
    ptr->pLinkLiskdata = ptrData;
    
    if(Handle->Head == NULL)
    {
        Handle->Head = ptr;
        Handle->Tail = ptr;
    }
    else
    {
        ptr->prev= Handle->Tail;
        Handle->Tail->next= ptr;
        Handle->Tail = ptr;
    }

    return CWM_NON;
}

static pLinkListData_t CWM_LINK_LIST_SEARCH(pLinkListHandle_t Handle, uint32_t evtHandle)
{
    pLinkList_t ptr;
    for(ptr = Handle->Head; ptr != NULL; ptr = ptr->next)
    {
        if(evtHandle == ptr->pLinkLiskdata->evtHandle)
        {
            return ptr->pLinkLiskdata;
        }
    }
    
    /*If Handle not find*/
    return NULL;
}

static int CWM_LINK_LIST_SEARCH_AND_CALLBACK(pLinkListHandle_t Handle, uint32_t evtHandle, void *data)
{
    pLinkList_t ptr;
    for(ptr = Handle->Head; ptr != NULL; ptr = ptr->next)
    {
        if(evtHandle == ptr->pLinkLiskdata->evtHandle)
        {
            if(NULL == ptr->pLinkLiskdata->evtCbfunc)
                return CWM_ERROR_NULL_POINT;
            
            ptr->pLinkLiskdata->evtCbfunc(ptr->pLinkLiskdata, data);
        }
    }
    
    /*If Handle not find*/
    return CWM_ERROR_NOT_FOUND;
}

static int LIST_REMOTE(pLinkListHandle_t Handle, pLinkList_t pRemove)
{

    /*List only have one*/
    if((Handle->Head == Handle->Tail))
    {
        Handle->Head = NULL;
        Handle->Tail = NULL;
        
        if(NULL != Handle->free)
            Handle->free(pRemove);
    }
    /*remove list on head*/
    else if(pRemove == Handle->Head)
    {
        Handle->Head = pRemove->next;
        pRemove->next->prev = NULL;
        
        if(NULL != Handle->free)
            Handle->free(pRemove);
        
    }
    /*remove list on Tail*/
    else if(pRemove == Handle->Tail)
    {
        Handle->Tail = pRemove->prev;
        pRemove->prev->next= NULL;
        
        if(NULL != Handle->free)
            Handle->free(pRemove);
    }
    else
    {
        pRemove->prev->next = pRemove->next;
        pRemove->next->prev = pRemove->pdata;
        
        if(NULL != Handle->free)
            Handle->free(pRemove);
    }
    return CWM_NON;
}

static int CWM_LINK_LIST_REMOVE(pLinkListHandle_t Handle, uint32_t evtHandle)
{
    pLinkList_t ptr;
    for(ptr = Handle->Head; ptr != NULL; ptr = ptr->next)
    {
        if(evtHandle == ptr->pLinkLiskdata->evtHandle)
        {
            return LIST_REMOTE(Handle, ptr);
        }
    }
    
    /*If Handle not find*/
    return CWM_ERROR;
}

void CWM_LINK_LIST_INIT(pLinkListHandle_t Handle)
{
    Handle->Head = NULL;
    Handle->Tail = NULL;
    Handle->AddToHead = CWM_LINK_LIST_ADD_TO_HEAD;
    Handle->AddToTail = CWM_LINK_LIST_ADD_TO_TAIL;
    Handle->Search = CWM_LINK_LIST_SEARCH;
    Handle->FindAndCallBack = CWM_LINK_LIST_SEARCH_AND_CALLBACK;
    Handle->Remove = CWM_LINK_LIST_REMOVE;
}

#endif /*USE_LINK_LIST_TEMPLATE*/
