#include<xse/TaskExecutor.hpp>

namespace XSe
{
	TaskExecutor::TaskExecutor():taskQueue(),threadLimit(4), runningThreads(0){
		std::thread pt(TaskExecutor::porcessorthread, this);
		pt.detach();
	}
	
	void TaskExecutor::push(std::function<void()> func){
		std::function<void()> wF = this->wrapFunction(func);
		std::lock_guard<std::mutex> lck(mtx);
		this->taskQueue.push(wF);
		cv.notify_one();
	}

	bool TaskExecutor::checkForDataAndThreadAvailability(){
		return !this->taskQueue.empty() && this->runningThreads<this->threadLimit;;
	}

	void TaskExecutor::porcessorthread(TaskExecutor *e){
		while(true){
			std::unique_lock<std::mutex> lck(e->mtx);
			e->cv.wait(
				lck,[&e]{ return e->checkForDataAndThreadAvailability(); });
			lck.unlock();
			std::function<void()> poppedFunc = e->taskQueue.front();
			e->taskQueue.pop();
			std::thread t(poppedFunc);
			t.detach();
			e->runningThreads++;
		}
	}

	std::function<void()> TaskExecutor::wrapFunction(std::function<void()> f){
		return [this,f](void){
			f();
			std::lock_guard<std::mutex> lck(mtx);
			runningThreads--;
			cv.notify_one();
		};
	}
}