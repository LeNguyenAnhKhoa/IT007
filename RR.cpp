#include <iostream>
#include <iomanip>
#include <queue>
using namespace std;

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;  // Thời gian còn lại
    int waiting_time;
    int response_time;
    int completion_time;
    int turnaround_time;
};

void calculateTimes(Process processes[], int n, int quantum) {
    int current_time = 0;
    int completed = 0;
    queue<int> q;
    bool is_in_queue[100] = {false};
    bool response_flag[100] = {false};

    // Thêm các process có arrival_time = 0 vào hàng đợi
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        if (processes[i].arrival_time == 0) {
            q.push(i);
            is_in_queue[i] = true;
        }
    }

    while (completed != n) {
        if (!q.empty()) {
            int idx = q.front();
            q.pop();

            // Ghi lại response time nếu chưa được tính
            if (!response_flag[idx]) {
                processes[idx].response_time = current_time - processes[idx].arrival_time;
                response_flag[idx] = true;
            }

            // Xử lý trong quantum time hoặc thời gian còn lại của process
            if (processes[idx].remaining_time <= quantum) {
                current_time += processes[idx].remaining_time;
                processes[idx].remaining_time = 0;
                processes[idx].completion_time = current_time;
                processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
                processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
                completed++;
            } else {
                current_time += quantum;
                processes[idx].remaining_time -= quantum;
            }

            // Thêm các process đến trong thời gian current_time vào hàng đợi
            for (int i = 0; i < n; i++) {
                if (processes[i].arrival_time <= current_time && !is_in_queue[i] && processes[i].remaining_time > 0) {
                    q.push(i);
                    is_in_queue[i] = true;
                }
            }

            // Đưa process vừa xử lý xong quantum về cuối hàng đợi nếu nó chưa hoàn thành
            if (processes[idx].remaining_time > 0) {
                q.push(idx);
            }
        } else {
            current_time++;
            // Thêm các process đến trong thời gian current_time vào hàng đợi
            for (int i = 0; i < n; i++) {
                if (processes[i].arrival_time <= current_time && !is_in_queue[i] && processes[i].remaining_time > 0) {
                    q.push(i);
                    is_in_queue[i] = true;
                }
            }
        }
    }
}

void printGanttChart(Process processes[], int n, int quantum) {
    cout << "\nRound Robin Gantt Chart:\n";
    int current_time = 0;
    queue<int> q;
    bool is_in_queue[100] = {false};
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        if (processes[i].arrival_time == 0) {
            q.push(i);
            is_in_queue[i] = true;
        }
    }

    while (!q.empty()) {
        int idx = q.front();
        q.pop();

        int exec_time = min(quantum, processes[idx].remaining_time);
        cout << "| P" << processes[idx].pid << " (" << current_time << " - " << current_time + exec_time << ") ";

        current_time += exec_time;
        processes[idx].remaining_time -= exec_time;

        // Thêm các process đến trong thời gian current_time vào hàng đợi
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && !is_in_queue[i] && processes[i].remaining_time > 0) {
                q.push(i);
                is_in_queue[i] = true;
            }
        }

        // Đưa process vừa xử lý xong quantum về cuối hàng đợi nếu nó chưa hoàn thành
        if (processes[idx].remaining_time > 0) {
            q.push(idx);
        }
    }
    cout << "|\n";
}

int main() {
    int n, quantum;
    cout << "Nhap so luong tien trinh: ";
    cin >> n;
    cout << "Nhap quantum time: ";
    cin >> quantum;

    Process processes[100];

    for(int i = 0; i < n; i++) {
        cout << "\nTien trinh " << i + 1 << ":\n";
        processes[i].pid = i + 1;
        cout << "Arrival time: ";
        cin >> processes[i].arrival_time;
        cout << "Burst time: ";
        cin >> processes[i].burst_time;
    }

    calculateTimes(processes, n, quantum);

    cout << "\nBang thong tin tien trinh:\n";
    cout << setw(5) << "PID" << setw(15) << "Arrival Time" << setw(15) << "Burst Time"
         << setw(20) << "Response Time" << setw(15) << "Waiting Time"
         << setw(20) << "Turnaround Time\n";

    float avg_waiting_time = 0, avg_response_time = 0, avg_turnaround_time = 0;

    for(int i = 0; i < n; i++) {
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

    printGanttChart(processes, n, quantum);

    cout << "\nThoi gian doi trung binh: " << fixed << setprecision(2) << avg_waiting_time;
    cout << "\nThoi gian dap ung trung binh: " << avg_response_time;
    cout << "\nThoi gian hoan thanh trung binh: " << avg_turnaround_time << endl;

    return 0;
}
