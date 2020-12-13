#include <iostream>
#include <vector>
#include <iomanip>
#include <mutex>
#include <thread>
#include <chrono>

// Магазин работает до последнего клиента, затем через некоторое время закрывается.

const int CONSUMERS_AMOUNT = 7;

std::condition_variable condition1;
std::condition_variable condition2;

// Flags for cash desks.
bool queue1_is_empty = true;
bool queue2_is_empty = true;

// State of cash desks.
bool cashier1_ready = true;
bool cashier2_ready = true;

// Number of current consumer.
int current_consumer1 = 0;
int current_consumer2 = 0;

int current_consumer_amount = CONSUMERS_AMOUNT;

/// <summary>
/// 2 queues.
/// </summary>
struct Consumers {
	std::vector<std::thread> queue1;
	std::vector<std::thread> queue2;
};

/// <summary>
/// Simulate cash desk 1.
/// </summary>
void CashDesk1() {
	using namespace std;
	std::mutex mutex;
	std::unique_lock<std::mutex> lock(mutex);

waiting_consumers:

	// Sleeping.
	condition1.wait(lock, [] {
		return !queue1_is_empty;
		});

	cashier1_ready = false;

	// Process client.
	this_thread::sleep_for(2s);
	std::cout << "Client " << current_consumer1 << " has been processed in cash desk 1\n\n";
	--current_consumer_amount;

	queue1_is_empty = true;

	cashier1_ready = true;
	condition1.notify_all();

	if (current_consumer_amount <= 0) {
		exit(0);
	}

	goto waiting_consumers;
}

/// <summary>
/// Simulate cash desk 2.
/// </summary>
void CashDesk2() {
	using namespace std;
	std::mutex mutex;
	std::unique_lock<std::mutex> lock(mutex);

waiting_consumers:

	// Sleeping.
	condition2.wait(lock, [] {
		return !queue2_is_empty;
		});

	cashier2_ready = false;

	// Process consumer.
	this_thread::sleep_for(2s);
	std::cout << "Client " << current_consumer2 << " has been processed in cash desk 2\n\n";
	--current_consumer_amount;

	queue2_is_empty = true;
	cashier2_ready = true;
	condition2.notify_all();

	if (current_consumer_amount <= 0) {
		exit(0);
	}
	

	goto waiting_consumers;
}

/// <summary>
/// Simulate queue to cash desk 1.
/// </summary>
/// <param name="consumer_number"> Number of consumer </param>
void Queue1(const int consumer_number) {
	std::mutex mutex;
	std::mutex temp;
	std::unique_lock<std::mutex> lk(temp);

	// Waiting for the cash desk.
	condition1.wait(lk, [] {
		return cashier1_ready;
		});

	std::cout << "Client " << consumer_number << " came to the 1nd cash desk\n";
	current_consumer1 = consumer_number;
	cashier1_ready = false;
	queue1_is_empty = false;
	condition1.notify_all();
}

/// <summary>
/// Simulate queue to cash desk 2.
/// </summary>
/// <param name="consumer_number"> Number of consumer </param>
void Queue2(const int consumer_number) {
	std::mutex mutex;
	std::unique_lock<std::mutex> lk(mutex);

	// Waiting for the cash desk.
	condition2.wait(lk, [] {
		return cashier2_ready;
		});

	std::cout << "Client " << consumer_number << " came to the 2nd cash desk\n";
	current_consumer2 = consumer_number;
	cashier2_ready = false;
	queue2_is_empty = false;
	condition2.notify_all();
}

/// <summary>
/// Get 2 queues of consumers.
/// </summary>
/// <returns> 2 queues of consumers </returns>
Consumers GetConsumers() {
	using namespace std;

	Consumers consumers;
	
	for (auto i = 0; i < CONSUMERS_AMOUNT; ++i) {
		if (rand() % 2) {
			consumers.queue1.emplace_back(thread(Queue1, i + 1));
		}
		else {
			consumers.queue2.emplace_back(thread(Queue2, i + 1));
		}

		this_thread::sleep_for(1.5s);
	}

	this_thread::sleep_for(10s);

	return consumers;
}

int main(int argc, char* argv[]) {
	srand(std::chrono::system_clock::now().time_since_epoch().count());

	std::thread cashier_1(CashDesk1);
	std::thread cashier_2(CashDesk2);

	auto consumers = GetConsumers();

	cashier_1.join();
	cashier_2.join();

	for (auto&& consumer : consumers.queue1) {
		consumer.join();
	}

	for (auto&& consumer : consumers.queue2) {
		consumer.join();
	}

	return 0;
}
