/** pagequeue.c
 * ===========================================================
 * Name: Aidan Blankenship, 4 22 2026
 * Section: CS483 / M4
 * Project: PEX3 - Page Replacement Simulator
 * Purpose: Implementation of the PageQueue ADT — a doubly-linked
 *          list for LRU page replacement.
 *          Head = LRU (eviction end), Tail = MRU end.
 * =========================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "pagequeue.h"

/**
 * @brief Create and initialize a page queue with a given capacity
 */
PageQueue *pqInit(unsigned int maxSize) {
    // TODO: malloc a PageQueue, set head and tail to NULL,
    //       size to 0, maxSize to maxSize, and return the pointer
    PageQueue* pQueue = malloc(sizeof(PageQueue));
    pQueue->head = NULL;
    pQueue->tail = NULL;
    pQueue->maxSize = maxSize;
    pQueue->size = 0;

    return pQueue;
}

/**
 * @brief Access a page in the queue (simulates a memory reference)
 */
long pqAccess(PageQueue *pq, unsigned long pageNum) {
    // TODO: Search the queue for pageNum (suggest searching tail->head
    //       so you naturally count depth from the MRU end).
    //
    if(pq->tail == NULL){
        PqNode* newNode = malloc(sizeof(PqNode));
        newNode->next = NULL;
        newNode->prev = NULL;
        newNode->pageNum = pageNum;
        pq->head = newNode;
        pq->tail = newNode;
        pq->size += 1;
        return -1;
    }

    int d = 0;
    PqNode* currNode = pq->tail;
    for(int i = 0; i<pq->size; i++){
        if(currNode->pageNum == pageNum){
            break;
        }
        else{
            currNode=currNode->prev;
            d++;
        }
    }
    
    // HIT path (page found at depth d):
    //   - Remove the node from its current position and re-insert
    //     it at the tail (most recently used).
    //   - Return d.
    //  
    if(currNode != NULL && currNode->pageNum == pageNum){
        if(currNode != pq->tail && currNode != pq->head){
            currNode->prev->next = currNode->next;
            currNode->next->prev = currNode->prev;
            pq->tail->next = currNode;
            currNode->prev = pq->tail;
            currNode->next = NULL;
            pq->tail = currNode;
        }
        else if(currNode == pq->head){
            currNode->next->prev = NULL;
            pq->head = currNode->next;
            pq->tail->next = currNode;
            currNode->prev = pq->tail;
            currNode->next = NULL;
            pq->tail = currNode;
        }
        return d;
    }

    // MISS path (page not found):
    //   - Allocate a new node for pageNum and insert it at the tail.
    //   - If size now exceeds maxSize, evict the head node (free it).
    //   - Return -1.
    PqNode* newNode = malloc(sizeof(PqNode));
    newNode->next = NULL;
    newNode->pageNum = pageNum;
    pq->tail->next = newNode;
    newNode->prev = pq->tail;
    pq->tail = newNode;
    pq->size += 1;
    

    if(pq->size > pq->maxSize){
        pq->head = pq->head->next;
        free(pq->head->prev);
        pq->head->prev = NULL;
        pq->size = pq->size - 1;
    }

    return -1;
}

/**
 * @brief Free all nodes in the queue and reset it to empty
 */
void pqFree(PageQueue *pq) {
    // TODO: Walk from head to tail, free each node, then free
    //       the PageQueue struct itself.
    PqNode* temp;
    PqNode* next;
    temp = pq->head;
    for(int i=0;i<pq->size;i++){
        next = temp->next;
        free(temp);
        temp = next;
    }
    free(pq);
}

/**
 * @brief Print queue contents to stderr for debugging
 */
void pqPrint(PageQueue *pq) {
    // TODO (optional): Print each page number from head to tail,
    //                  marking which is head and which is tail.
    //                  Useful for desk-checking small traces.
}
