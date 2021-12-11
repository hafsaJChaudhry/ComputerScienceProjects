// JQueue: a job queue using a max-heap and function pointers
// Based on MQueue by Scott Bennett (sb13@umbc.edu)
// CMSC 341, Spring 2019

#include "JQueue.h"

//
// JQueue implementations go here!!!!
//
//This is the constructor for the JQueue class. It must be provided with the capacity of the queue and a pointer to the prioritization function. If the capacity is less than one, throw an out_of_range exception.
JQueue::JQueue(int capacity, prifn_t priFn){
    m_capacity = capacity;
    m_heap = new job_t[m_capacity];
    m_size = 0;
    setPriorityFn(priFn);
}
/*****************************************************************************/
//Destructor for the JQueue class. All dynamically-allocated data must be deallocated.
JQueue::~JQueue(){
    delete [] m_heap;
    m_heap = NULL;
}
/*****************************************************************************/
//Copy constructor for the JQueue class. Must make a deep copy of the rhs object
JQueue::JQueue(const JQueue& rhs){
    int count = 1;
    m_size = rhs.m_size;
    m_heap = new job_t[rhs.m_capacity];

    while(count <= rhs.m_size){
        m_heap[count] = rhs.m_heap[count];
        count++;
    }

    m_capacity = rhs.m_capacity;
    priority = rhs.priority;
}
/*****************************************************************************/
//Assignment operator for the JQueue class. Remember to check for self-assignment and to free all dynamically allocated data members of the host. You should not use the copy constructor in the implementation of the assignment operator.
JQueue& JQueue::operator=(const JQueue& rhs){
    if(this != &rhs){
        delete [] m_heap;
        int count = 1;
        m_size = rhs.m_size;
        m_heap = new job_t[rhs.m_capacity];

        while(count <= rhs.m_size){
            m_heap[count] = rhs.m_heap[count];
            count++;
        }

        m_capacity = rhs.m_capacity;
        priority = rhs.priority;
    }
    return *this;
}
/*****************************************************************************/
//Insert a job into the job queue. Must maintain the max-heap property.
void JQueue::insertJob(const job_t input){
    if(m_size < m_capacity){
        m_size = m_size + 1;
        m_heap[m_size] = input;
        insertHelper(m_size);
    }else if(m_size == m_capacity)
        throw std::out_of_range("Heap is filled to capacity");
}
/*****************************************************************************/
void JQueue::insertHelper(int size){
    if(size > 1){

        if(priority(m_heap[size/2]) < priority(m_heap[size])){
            job_t child = m_heap[size];
            m_heap[size] = m_heap[size/2];
            m_heap[size/2] = child;
        }

        insertHelper(size/2);
    }
    if(size == 0)
        return;
}

/*****************************************************************************/
//Extract the highest priority job from the job queue. Must maintain the max-heap property.
job_t JQueue::getNextJob(){
    if(m_size != 0){

        //highest temp priority job
        job_t maxRoot = m_heap[1];
        m_heap[1] = m_heap[m_size];
        m_size = m_size -1;

        buildHeap(1);
        return maxRoot;
    }
    else
        throw std::out_of_range("Heap has a size of 0");

    return getNextJob();
}
/*****************************************************************************/
//Return the current number of jobs in the queue.
int JQueue::numJobs() const{
    return m_size;
}
/*****************************************************************************/
//Print the contents of the job queue. The jobs should be listed be in array order not priority order (although the first job listed should have highest priority).
void JQueue::printJobQueue() const{
    std::cout << "Job Queue Size " << m_size << std::endl;
    std::cout << "Number of jobs in the queue " << numJobs() << std::endl;

    int count = 1;

    while(count <= m_size){
        std::cout << "[" << count << "]" << m_heap[count] << "(" << priority(m_heap[count]) << ")" << std::endl;
        count++;
    }
}
/*****************************************************************************/
//Get the current priority function.
prifn_t JQueue::getPriorityFn(){
    return priority;
}
/*****************************************************************************/
//Set a new priority function. Must rebuild the heap!
void JQueue::setPriorityFn(prifn_t priFn){
    priority = priFn;

    //builds heap
    int count = m_size / 2;
    while(count >= 1){
        buildHeap(count);
        count = count - 1;
    }
}
/*****************************************************************************/
//recursive function to find left and right child to assign max and assist in building heap...bubble down
void JQueue::buildHeap(int value){
    int max = value;
    int rightChild = 2 * value + 1;
    int leftChild = 2 * value;

    //checks size of left and right child and then assigns max accordingly
    if(leftChild <= m_size){
        if(priority(m_heap[value]) < priority(m_heap[leftChild]))
            max = leftChild;
    }else if(rightChild <= m_size){
        if(priority(m_heap[max]) < priority(m_heap[rightChild]))
            max = rightChild;
    }

    //updates max if conditions met above
    if(value != max){
        job_t temp = m_heap[value];
        m_heap[value] = m_heap[max];
        m_heap[max] = temp;
        buildHeap(max);
    }
}
/*****************************************************************************/
//Dump the entire heap, not just the portion currently in use. For debugging purposes.
void JQueue::dump() const{}
/*****************************************************************************/

// Overloaded insertion operator for job_t
std::ostream& operator<<(std::ostream& sout, const job_t& job)
{
    sout << "Job: " << job.m_name << ", pri: " << job.m_priority
         << ", u: " << job.m_user << ", g: " << job.m_group << ", proc: "
         << job.m_proc << ", mem: " << job.m_mem << ", time: " << job.m_time;
    return sout;
}
