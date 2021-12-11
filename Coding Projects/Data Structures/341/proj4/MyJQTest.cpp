// MyJQTest.cpp : a job queue using a max-heap and function pointers
// CMSC 341, Spring 2019
// Name: Hafsa Chaudhry

#include "JQueue.h"

//
// Priority functions compute an integer priority for a job.  Internal
// computations may be floating point, but must return an integer.
//
// Function definitions
int priorityFn1(const job_t &job);
int priorityFn2(const job_t &job);
int priorityFn3(const job_t &job);

int main() {

    // Create a job queue with capacity five and priority function
    // priorityFn1()

    JQueue jobQueue(5, priorityFn1);

    // Create some test jobs and insert them into the queue

    job_t testJob1("test1", 70, 3, 8,  16, 1024, 120);
    job_t testJob2("test2", 40, 50,  5, 128,  512, 300);
    job_t testJob3("test3", 32, 75,  7, 128, 1024, 1800);
    job_t testJob4("test1a", 90, 30, 8,  16, 1024, 120);

    jobQueue.insertJob(testJob3);
    jobQueue.insertJob(testJob1);
    jobQueue.insertJob(testJob2);
    jobQueue.insertJob(testJob4);

    // Print the queue, change priority function, print again, change
    // again, print again.  Changing the priority function should cause
    // the heap to be rebuilt.
    jobQueue.printJobQueue();

    jobQueue.setPriorityFn(priorityFn2);

    jobQueue.printJobQueue();

    jobQueue.setPriorityFn(priorityFn1);

    jobQueue.printJobQueue();

    jobQueue.setPriorityFn(priorityFn3);

    jobQueue.printJobQueue();

    // Remove and print jobs in priority order

    std::cout << "\nFinal queue in priority order:\n";
    while (jobQueue.numJobs() > 0) {
        job_t job = jobQueue.getNextJob();
        std::cout << job << " (" << priorityFn1(job) << ")" << std::endl;
    }

    return 0;
}

int priorityFn1(const job_t &job) {
    float pri;
    pri = (400 /10) * job.m_priority;

    if (job.m_group == 8 || job.m_group == 23)
        pri += 200;

    if (job.m_user == 11 || job.m_user == 20)
        pri += 400;

    pri = pri - job.m_priority * (3/100);

    return int(pri);
}

int priorityFn2(const job_t&job) {
    return int(job.m_priority);
}

int priorityFn3(const job_t&job) {
    return int(job.m_user);
}
