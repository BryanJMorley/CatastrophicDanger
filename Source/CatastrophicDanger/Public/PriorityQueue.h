

#pragma once

#include "CoreMinimal.h"

template <typename InElementType>
struct TPriorityQueueNode {
    InElementType Element;
    int Priority;

    TPriorityQueueNode()
    {
    }

    TPriorityQueueNode(InElementType InElement, int InPriority)
    {
        Element = InElement;
        Priority = InPriority;
    }

    bool operator<(const TPriorityQueueNode<InElementType> Other) const
    {
        return Priority < Other.Priority;
    }

    bool operator==(const TPriorityQueueNode<InElementType> Other) const
    {
        return Element == Other.Element;
    }

};

template <typename InElementType>
class TPriorityQueue {
public:
    TPriorityQueue()
    {
        Array.Heapify();
    }

public:
    // Always check if IsEmpty() before Pop-ing!
    InElementType Pop()
    {
        TPriorityQueueNode<InElementType> Node;
        Array.HeapPop(Node);
        return Node.Element;
    }

    TPriorityQueueNode<InElementType> PopNode()
    {
        TPriorityQueueNode<InElementType> Node;
        Array.HeapPop(Node);
        return Node;
    }

    void Push(InElementType Element, int Priority)
    {
        Array.HeapPush(TPriorityQueueNode<InElementType>(Element, Priority));
    }

    bool IsEmpty() const
    {
        return Array.Num() == 0;
    }

    inline int Num() const
    {
        return Array.Num();
    }

    bool Contains(InElementType Element) const {
        return Array.Contains(TPriorityQueueNode<InElementType>(Element, 0));
    }

    bool Remove(InElementType Element)
    {
        if (Array.Contains(TPriorityQueueNode<InElementType>(Element, 0))) {
           Array.HeapRemoveAt(Array.Find(TPriorityQueueNode<InElementType>(Element, 0)));
           return true;
        }
        return false;
    }

private:
    TArray<TPriorityQueueNode<InElementType>> Array;
};