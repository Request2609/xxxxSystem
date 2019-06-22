#include"heapTimer.h"
ClientData :: ClientData() {
}
ClientData :: ~ClientData() {
}
HeapTimer :: HeapTimer() {    
}
HeapTimer :: HeapTimer(int delay) {
    expire = time(NULL)+delay ;
}
HeapTimer :: ~HeapTimer() {
}

TimeHeap :: TimeHeap() {
}
//初始化堆
TimeHeap :: TimeHeap(int cap):capacity(cap) {
    array = new HeapTimer*[capacity] ;
    if(!array) {
        throw std :: exception() ;
    }
    for(int i=0; i< capacity; i++) {
        array[i] = nullptr  ;  
    }
} 

//根据已有数组初始化堆
TimeHeap :: TimeHeap(HeapTimer** int_array, int size, int cap) {
    if(cap < size) {
        throw std::exception() ;
    }
    array = new HeapTimer*[cap] ;
    if(!array) {
        throw std::exception() ;
    }
    for(int i=0; i<size; i++) {
        array[i] = int_array[i] ;
    }
    //进行下虑
    for(int i=size/2-1; i>=0; i--) {
        percolateDown(i) ;
    }
}

TimeHeap :: ~TimeHeap() {
    for(int i=0; i<size; i++) {
        if(!array[i]) {
            delete array ;
        }
    } 
    delete array ;
}

//添加新节点
void TimeHeap::addTimer(HeapTimer* timer) {
    
    if(!timer) {
        return ;
    }
    //当空间不够时，需要扩容
    if(size >= capacity) {
        reSize() ;
    }
    int hole = size++ ;
    int parent = 0 ;
    //采用上虑的方式添加节点，自底向下
    for(; hole>0; hole = parent) {
        //获取叶子节点的父节点
        parent = (hole-1)/2 ;
        if(array[parent]->expire > timer->expire) {
            array[hole] = array[parent] ;
        }
        else {
            break ;
        }
    }
    array[hole] = timer ;
}

//对堆节点进行下虑，确保hole个节点满足最小堆性质
void TimeHeap :: percolateDown(int hole) {
    HeapTimer* tmp = array[hole] ;
    int child = 0 ;
    for(; hole*2+1 < size; hole = child) {
        child = hole*2 + 1 ;
        //要是左孩子的时间戳大于右节点的时间戳，以最小的为基准,所以child++
        if(child < size && array[child]->expire > array[child+1]->expire) {
            child++ ;
        }
        if(tmp->expire > array[child]->expire) {
            array[hole] = array[child] ;
        }
        else {
            break ;
        }
    }
    array[hole] = tmp ;
}

//添加定时器，先放在数组末尾，在进行上虑使其满足最小堆
void TimeHeap :: delTimer(HeapTimer* timer) {
    if(!timer) {
        return  ;
    }
    timer->cbFunc = nullptr ;
}

//删除堆顶计时器
//先删除掉array[0]节点，将array中最后一个元素调整到根节点
//并进行下虑操作
void TimeHeap :: popTimer() {
    if(!size) {
        return ;
    }
    if(array[0]) {
        delete array[0] ;
        array[0] = array[--size] ;
        //对根节点进行下虑
        percolateDown(0) ;
    }
}

//在时间堆中找时间的节点
void TimeHeap :: tick() {
    HeapTimer* tmp = array[0] ;
    time_t cur = time(NULL) ;
    while(!size) {
        if(tmp) {
            break ;
        }
        if(tmp->expire > cur) {
            break ;
        }
        if(array[0]->cbFunc) {
            //时间到调用回调函数
            array[0]->cbFunc(array[0]->userData) ;
        }
        popTimer() ;
        tmp = array[0] ;
    }
}

//空间不足时，使空间扩大为原来的两倍
void TimeHeap::reSize() {
    HeapTimer** tmp = new HeapTimer*[capacity*2] ;
    for(int i=0; i<2*capacity; i++) {
        tmp[i] = nullptr ;
    }
    if(!tmp) {
        throw std :: exception() ;
    }
    capacity*=2 ;
    for(int i=0; i<size; i++) {
        tmp[i] = array[i] ;
    }
    delete[]array ;
    array = tmp ;
}

