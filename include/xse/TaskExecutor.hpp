#ifndef XSE_EXECUTOR_H
#define XSE_EXECUTOR_H

#include<queue>
#include<functional>
#include<thread>
#include<iostream>
#include<condition_variable>
#include<mutex>

namespace XSe{

	class TaskExecutor{
		private:
			std::queue<std::function<void()>> taskQueue;
			int threadLimit, runningThreads;
			std::condition_variable cv;
			std::mutex mtx;

		public:
			TaskExecutor();

			//Constructors
			void push(std::function<void()> func);

			//Methods
			bool checkForDataAndThreadAvailability();
			static void porcessorthread(TaskExecutor *e);
			std::function<void()> wrapFunction(std::function<void()> f);
	};
}

#endif