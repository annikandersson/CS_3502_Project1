#include <iostream> // Include iput/output stream library - Printing  to the console
#include <thread>  // Include thread library for std::thread - Multi-threading
#include <vector> // Include vector library for std::vector - Storing threads
#include <mutex> // Include the mutex library for std::mutex - Syncrhonization
#include <chrono> // Include chrono library - Time functions (e.g., sleep)
#include <cstdlib> // Include C standard library - Functions (e.g., srand, rand)
#include <ctime>  // Include C time library - Seed random number generator

using namespace std; // Use standard namespace 

// Global mutex (mutual exclusion, synchronization primitive) to protect shared resources (Phase 2)
mutex account_mutex; // Create mutex named account_mutex

// Define BankAccount class
class BankAccount {
    public: // Public section of class
        int balance; // Public variable to hold account balance

        mutex mtx; // Each bank acc has a mutex 

        // Constructor - Initializes bank account with initial balance
        BankAccount(int initial_balance) { // Constructor with parameter initial_balance
            balance = initial_balance; // Set balance equal to initial balance
        }

        // Method to deposit specified amount into account
        void deposit(int amount) {
            balance += amount;  // Add/increase balance by specified amount
        }

        // Method to withdraw specified amount from account
        void withdraw(int amount) { 
            balance -= amount; // Subtract/decrease balance by specified amount
        }
};

// Phase 1: Basic Thread Operations - Demo

void phase1_basicThreads() {
    cout << "Phase 1: Basic Thread Operations\n"; // Print header for Phase 1

    // Define lambda function to be executed by each thread
    auto threadFunction = [](int thread_id) { // Lambda - Takes an integer thread_id
        cout << "Thread " << thread_id << " is running.\n"; // Print message showing thread is active
    };

    vector<thread> threads; // Vector to hold thread objects

    // Create 5 threads to add to the vector
    for (int i = 0; i < 5; i++) { // Loop 0 - 4 (5 total threads)
        threads.push_back(thread(threadFunction, i)); // Create new thread w/ threadFunction and thread
    }

    // Wait for all threads to finish by joining each one
    for (auto &t : threads) { // Iterate through each thread in the vector
        t.join(); // Call join() to wait for thread to complete
    }
}

// Phase 2: Resource Protection - Demo

void phase2_resourceProtection() {
    cout << "Phase 2: Resource Protection\n"; // Print header for Phase 2

    BankAccount account(1000); // Create BankAccount object with initial balance (1000)

    // Define lambda function to deposit money into account SAFELY
    auto depositFunction = [&account](int amount, int thread_id) { // Lambda - Captures account by reference
        lock_guard<mutex> guard(account_mutex); // Create lock_guard - Lock global mutex automatically
        cout << "Thread " << thread_id << " depositing " << amount << " -\n"; // Print how much each thread deposits
        account.deposit(amount); // Deposit specified amount into acc
        cout << "Account balance after deposit by thread " << thread_id << ": " << account.balance << "\n"; // Print new balance after deposit
    };

    vector<thread> threads;

    for (int i = 0; i < 5; i++) {
        threads.push_back(thread(depositFunction, (i+1)*100, i));
    }

    for (auto &t : threads) {
        t.join();
    }

    cout << "Final account balance: " << account.balance << "\n";
}

// Phase 3: Deadlock Creation - Demo

void phase3_deadlockCreation() {
    cout << "Phase 3: Deadlock Creation\n"; // Printer header for Phase 3

    BankAccount account1(1000); // Create an account (account1) with initial balance of 1000
    BankAccount account2(1000); // Create another account (account2) with initial balance of 1000

    // Lamda function to transfer $100 from account1 to account2
    auto transfer1_2 = [&account1, &account2]() { // Lamda captures both accounts by REFERENCE
        account1.mtx.lock(); // Lock account1's mutex
        this_thread::sleep_for(chrono::milliseconds(100)); // Create 100s delay to increase likelihood of deadlock
        account2.mtx.lock(); // Lock account2's mutex
        account1.withdraw(100); // Withdraw $100 from account1
        account2.deposit(100); // Deposit $100 to account2
        cout << "Account 1 transferrred $100 to Account 2.\n"; // Print message regarding transfer
        account2.mtx.unlock(); // Unlock account2's mutex
        account1.mtx.unlock(); // Unlock account1's mutex
    };

    // Lamda function to transfer $100 from account2 to account1
    auto transfer2_1 = [&account1, &account2]() { // Lamda captures both accounts by REFERENCE
        account2.mtx.lock(); // Lock account1's mutex
        this_thread::sleep_for(chrono::milliseconds(100)); // Create 100s delay to increase likelihood of deadlock
        account1.mtx.lock(); // Lock account2's mutex
        account2.withdraw(100); // Withdraw $100 from account1
        account1.deposit(100); // Deposit $100 to account2
        cout << "Account 2 transferrred $100 to Account 1.\n"; // Print message regarding transfer
        account1.mtx.unlock(); // Unlock account2's mutex
        account2.mtx.unlock(); // Unlock account1's mutex
};

    // Create 2 threads (likely to deadlock)
    thread t1(transfer1_2); // t1 to perform transfer1_2
    thread t2(transfer2_1); // t2 to perform transfer 2_1

    // Join threads (when/if deadlock occurs, joins will BLOCK forever)
    t1.join(); // Wait for t1 to finish
    t2.join(); // Wait for t2 to finish

    // Print final balance (will not print if deadlock occurs)
    cout << "Account 1 Balance: " << account1.balance << "\n"; // Prints account1 balance
    cout << "Account 2 Balance: " << account2.balance << "\n"; // Prints account2 balance
}

// Phase 4: Deadlock Resolution - Demo

void phase4_deadlockResolution() {
    cout << "Phase 4: Deadlock Resolution\n"; 

    BankAccount account1(1000);
    BankAccount account2(1000);

    // Lambda function - Safe transfer from acc 1 to acc 2 using std::lock
    auto safeTransfer1_2 = [&account1, &account2]() {
        // Lock both mutexes simultaneously to avoid deadlock
        lock(account1.mtx, account2.mtx);
        // Create lock guards to "adopt" locked mutexes
        lock_guard<mutex> lock1(account1.mtx, adopt_lock);
        lock_guard<mutex> lock2(account2.mtx, adopt_lock);
        account1.withdraw(100);
        account2.deposit(100);
        cout << "Safely transferred $100 from Account 1 to Account 2.\n";
    };

    // Lambda function - Safe transfer from acc 2 to acc 1 using std::lock
    auto safeTransfer2_1 = [&account1, &account2]() {
        // Lock both mutexes simultaneously to avoid deadlock
        lock(account1.mtx, account2.mtx);
        // Create lock guards to "adopt" locked mutexes
        lock_guard<mutex> lock1(account1.mtx, adopt_lock);
        lock_guard<mutex> lock2(account2.mtx, adopt_lock);
        account2.withdraw(100);
        account1.deposit(100);
        cout << "Safely transferred $100 from Account 2 to Account 1.\n";
    };

    // Two threads to safe transfers
    thread t1(safeTransfer1_2);
    thread t2(safeTransfer2_1);

    // Join threads to wait for each to finish
    t1.join();
    t2.join();

    // Print final balances for both accounts after safe transfer
    cout << "Account 1 Balance: " << account1.balance << "\n";
    cout << "Account 2 Balance: " << account2.balance << "\n";
}

// Main - Execute multi-threading
int main() {
    srand(time(0)); // "Seed" random number generator w/ current time

    phase1_basicThreads(); // Running Phase 1

    phase2_resourceProtection(); // Running Phase 2

    // PHASE 3: Uncomment when testing, deadlock potentially will stop/freeze program
    // phase3_deadlockCreation(); // Running Phase 3

    phase4_deadlockResolution(); // Running Phase 4

    return 0; // Program termination
}


