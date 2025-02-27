#include <iostream> // Include iostream library - Input/output operations
#include <unistd.h> // Include unistd.h - POSIX API functions (e.g., pipe(), fork(), read(), write())
#include <sys/types.h> // Include sys/types.h - Data types (e.g., pid_t);
#include <sys/wait.h> // Include sys/wait.h - Allows parent processes to wait for child processes to finish
#include <cstring> // Include cstring - String handling (e.g., strlen())

using namespace std; // Use standard namespace

// Main function - Program execution
int main() {

    int pipeFD[2]; // An array declaration of 2 ints - Holds file descriptors for pipe

    // Create the pipe:
    // pipeFD[0] => Read
    // pipeFD[1] => Write

    if (pipe(pipeFD) == -1) { // If pipe() returns -1, error has occurred,
        cerr << "Error: Failed to create the pipe."; // Print error message
        return 1; // Exit program
    }

    // Create a child process with fork()
    pid_t pid = fork(); 

    if (pid < 0) { // If fork() returns a negative value (fails),
        cerr << "Error: Fork failed."; // Print error message
        return 1; // Exit program
    }

    if (pid == 0) { // Execute in child process - pid
        // Child process only reads from pipe, does not write
        close(pipeFD[1]); // Close write end
        char buffer[200]; // Buffer for 200 chars (to store incoming messages)

        // Read incoming messages from read -> buffer 
        ssize_t readBytes = read(pipeFD[0], buffer, sizeof(buffer) - 1); // read till (sizeof(buffer) - 1)

        if (readBytes < 0) { // If read() returns a neg. value (fails),
            cerr << "Error: Failed to read from the pipe."; // Print error message
            return 1; // Exit program
        }

        buffer[readBytes] = '\0'; // Null-terminate buffer (forms proper C string)

        // Print message from parent process
        cout << "Child process received message: " << buffer << "\n";

        close(pipeFD[0]); // Close read end
    } else {
        // Parent process only writes to pipe, does not read
        close(pipeFD[0]); // Close read end
        
        // Message from parent process -> child process
        const char* messageToChild = "I am the parent process, and this is my message to the child process.";

        // Write messageToChild to write end of the pipe
        ssize_t writeBytes = write(pipeFD[1], messageToChild, strlen(messageToChild)); // strlen(messageToChild) - Returns # of chars in message

        if (writeBytes < 0) { // If write() returns neg. value (fails),
            cerr << "Error: Failed to write to the pipe."; // Print error message
            return 1; // Exit program
        }

        close(pipeFD[1]); // Close write end

        // Wait for child processes to finish executing
        wait(NULL); // wait() - Will suspend exec until child processes terminate
    }
    return 0; // Terminate program
}
