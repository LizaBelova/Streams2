#include<iostream> 
#include<cstdlib> 
#include<stack> 
#include<mutex> 
#include<thread> 

using namespace std;

template<typename T>
class threadSafeStack
{
private:
	std::stack<T> arr; //сам стэк
	mutex m; //мьютэкс
public:
	void push(const T& obj)
	{
        lock_guard<mutex>lock(m); //блокируем стэк для других потоков
        cout << this_thread::get_id() << ". Push " << obj << "\n";
        arr.push(obj); 
 	}
 	T pop() //выталкивание по значению
	{ 
        lock_guard<mutex>lock(m);
		if (arr.empty())
			throw exception();
		else
		{
			T temp = arr.top();
			arr.pop();
			cout << this_thread::get_id() <<  ". Pop " << temp << endl;
			return temp;
		}
	}
	
	shared_ptr<T>  getClass() //выталкивание по указателю
	{
		lock_guard<mutex>lock(m);
		if (arr.empty())
			throw exception();
		else
		{
			auto element_ptr = make_shared<T>(arr.top());
			arr.pop();
			return element_ptr;
		}		
 	}
};

void func_thread1(threadSafeStack<int> &arr)
{
	int pop_value;
	arr.push(10);
	arr.pop();
}

void func_thread2(threadSafeStack<int> &arr)
{
	int pop_value;
	arr.push(20);
	//arr.pop(pop_value);
}

void func_thread3(threadSafeStack<int> &arr)
{
	arr.push(30);
	shared_ptr<int> pop_by_ptr;
	pop_by_ptr = arr.getClass(); cout << *pop_by_ptr << "\n";
}

 
int main()
{ 
    threadSafeStack<int> st; //создаём объект "безопасный стэк"
    thread thread1(func_thread1, ref(st)); //инициализируем поток, передав ему имя функции, которая должна выполняться в этом самом потоке. Второй аргумент - это уже первый аргумент той самой функции 
    thread thread2(func_thread2, ref(st));
    thread thread3(func_thread3, ref(st));
    thread1.detach(); //удаляет ссылку на поток + посылает потоку сигнал о его завершении, возможно, не доработав до конца, но безопасно
    thread2.detach();
    thread3.join();
    return 0; 
}
