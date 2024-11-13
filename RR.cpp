#include <iostream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <queue>

using namespace std;

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int waiting_time;
    int response_time;
    int completion_time;
    int turnaround_time;
    int start_time;
};

void calculateTimes(Process processes[], int n, int q) {
    queue<int> ready_queue;
    int current_time = 0;
    int completed = 0;
    bool is_completed[100] = {0};

    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].start_time = -1;
    }

    // Add the first arrived process to the ready queue
    int next_arrival = 0;
    if (processes[0].arrival_time == 0) {
        ready_queue.push(0);
    } else {
        current_time = processes[0].arrival_time;
        ready_queue.push(0);
    }


    while (completed != n) {
        if (ready_queue.empty()) {
            // Find the next arriving process
            int next_process = -1;
            int min_arrival = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (!is_completed[i] && processes[i].arrival_time > current_time && processes[i].arrival_time < min_arrival) {
                    min_arrival = processes[i].arrival_time;
                    next_process = i;
                }
            }

            if (next_process != -1) {
                current_time = processes[next_process].arrival_time;
                ready_queue.push(next_process);
            } else {
                break; // All processes completed
            }
        }


        int current_process = ready_queue.front();
        ready_queue.pop();

        if (processes[current_process].start_time == -1) {
            processes[current_process].start_time = current_time;
            processes[current_process].response_time = processes[current_process].start_time - processes[current_process].arrival_time;
        }

        int time_slice = min(q, processes[current_process].remaining_time);
        processes[current_process].remaining_time -= time_slice;
        current_time += time_slice;


        // Add newly arrived processes to the ready queue
        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && processes[i].arrival_time <= current_time && processes[i].arrival_time > current_time - time_slice && i != current_process) {
                ready_queue.push(i);
            }
        }

        if (processes[current_process].remaining_time == 0) {
            processes[current_process].completion_time = current_time;
            processes[current_process].turnaround_time = processes[current_process].completion_time - processes[current_process].arrival_time;
            processes[current_process].waiting_time = processes[current_process].turnaround_time - processes[current_process].burst_time;
            is_completed[current_process] = true;
            completed++;
        } else {
            ready_queue.push(current_process);
        }
    }
}


void printGanttChart(Process processes[], int n, int q) {
    int total_time = 0;
    for (int i = 0; i < n; i++) {
        total_time = max(total_time, processes[i].completion_time);
    }

    vector<int> timeline(total_time, -1);

    queue<int> ready_queue;
    int current_time = 0;
    bool is_completed[100] = {0};
    int remaining_time[100];

    for (int i = 0; i < n; i++) {
        remaining_time[i] = processes[i].burst_time;
    }
    if (processes[0].arrival_time == 0) {
        ready_queue.push(0);
    } else {
        current_time = processes[0].arrival_time;
        ready_queue.push(0);
    }

    while (current_time < total_time) {
         if (ready_queue.empty()) {
            // Find the next arriving process
            int next_process = -1;
            int min_arrival = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (!is_completed[i] && processes[i].arrival_time > current_time && processes[i].arrival_time < min_arrival) {
                    min_arrival = processes[i].arrival_time;
                    next_process = i;
                }
            }

            if (next_process != -1) {
                current_time = processes[next_process].arrival_time;
                ready_queue.push(next_process);
            } else {
                break; // All processes completed
            }
        }

        int current_process = ready_queue.front();
        ready_queue.pop();

        int time_slice = min(q, remaining_time[current_process]);

        for (int i = 0; i < time_slice; i++) {
            timeline[current_time + i] = processes[current_process].pid;
        }
        remaining_time[current_process] -= time_slice;
        current_time += time_slice;

        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && processes[i].arrival_time <= current_time && processes[i].arrival_time > current_time - time_slice && i != current_process) {
                ready_queue.push(i);
            }
        }

        if (remaining_time[current_process] == 0) {
            is_completed[current_process] = true;
        } else {
            ready_queue.push(current_process);
        }
    }


    cout << "\nGantt Chart:\n";
    cout << " ";

    int prev_pid = timeline[0];
    int count = 1;
    for (int i = 1; i <= total_time; i++) {
        if (i == total_time || timeline[i] != prev_pid) {
            for (int j = 0; j < count; j++) cout << "--";
            cout << " ";
            count = 1;
            prev_pid = (i < total_time) ? timeline[i] : -1;
        } else {
            count++;
        }
    }
    cout << "\n|";

    prev_pid = timeline[0];
    count = 1;
    for (int i = 1; i <= total_time; i++) {
        if (i == total_time || timeline[i] != prev_pid) {
            for (int j = 0; j < count - 1; j++) cout << " ";
            if (prev_pid != -1) cout << "P" << prev_pid;
            else cout << " ";
            for (int j = 0; j < count - 1; j++) cout << " ";
            cout << "|";
            count = 1;
            prev_pid = (i < total_time) ? timeline[i] : -1;
        } else {
            count++;
        }
    }
    cout << "\n ";

    prev_pid = timeline[0];
    count = 1;
    for (int i = 1; i <= total_time; i++) {
        if (i == total_time || timeline[i] != prev_pid) {
            for (int j = 0; j < count; j++) cout << "--";
            cout << " ";
            count = 1;
            prev_pid = (i < total_time) ? timeline[i] : -1;
        } else {
            count++;
        }
    }
    cout << "\n";

    int current_time_print = 0;
    cout << current_time_print;
    prev_pid = timeline[0];
    count = 1;
    for (int i = 1; i <= total_time; i++) {
        if (i == total_time || timeline[i] != prev_pid) {
            current_time_print += count;
            if(current_time_print < 10) cout << "  ";
            else cout << " ";

            for (int j = 1; j < count; j++) cout << "  ";
            cout << current_time_print;

            count = 1;
            prev_pid = (i < total_time) ? timeline[i] : -1;
        } else {
            count++;
        }
    }
    cout << endl;
}


int main() {
    int n;
    cout << "Nhap so luong tien trinh: ";
    cin >> n;

    Process processes[100];

    for (int i = 0; i < n; i++) {
        cout << "\nTien trinh " << i + 1 << ":\n";
        processes[i].pid = i + 1;
        cout << "Arrival time: ";
        cin >> processes[i].arrival_time;
        cout << "Burst time: ";
        cin >> processes[i].burst_time;
    }

    int q;
    cout << "Nhap time quantum q: ";
    cin >> q;

    calculateTimes(processes, n, q);

    cout << "\nBang thong tin tien trinh:\n";
    cout << setw(5) << "PID" << setw(15) << "Arrival Time" << setw(15) << "Burst Time"
         << setw(20) << "Response Time" << setw(15) << "Waiting Time"
         << setw(20) << "Turnaround Time\n";

    float avg_waiting_time = 0, avg_response_time = 0, avg_turnaround_time = 0;

    for (int i = 0; i < n; i++) {
        cout << setw(5) << processes[i].pid << setw(15) << processes[i].arrival_time
             << setw(15) << processes[i].burst_time
             << setw(20) << processes[i].response_time << setw(15) << processes[i].waiting_time
             << setw(20) << processes[i].turnaround_time << endl;

        avg_waiting_time += processes[i].waiting_time;
        avg_response_time += processes[i].response_time;
        avg_turnaround_time += processes[i].turnaround_time;
    }

    avg_waiting_time /= n;
    avg_response_time /= n;
    avg_turnaround_time /= n;

    printGanttChart(processes, n, q);

    cout << "\nThoi gian doi trung binh: " << fixed << setprecision(2) << avg_waiting_time;
    cout << "\nThoi gian dap ung trung binh: " << avg_response_time;
    cout << "\nThoi gian hoan thanh trung binh: " << avg_turnaround_time << endl;

    return 0;
}
