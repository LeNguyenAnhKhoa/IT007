#include <iostream>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int response_time;
    int completion_time;
    int turnaround_time;  // Thêm trường turnaround_time
};

void calculateTimes(Process processes[], int n) {
    int current_time = 0;
    int completed = 0;
    bool is_completed[100] = {0};

    int start_time[100];
    for(int i = 0; i < n; i++) {
        start_time[i] = -1;
    }

    while (completed != n) {
        int shortest_job = -1;
        int min_burst = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && !is_completed[i]) {
                if (processes[i].burst_time < min_burst) {
                    min_burst = processes[i].burst_time;
                    shortest_job = i;
                }
                if (processes[i].burst_time == min_burst) {
                    if (processes[i].arrival_time < processes[shortest_job].arrival_time) {
                        shortest_job = i;
                    }
                }
            }
        }

        if (shortest_job == -1) {
            current_time++;
        } else {
            if(start_time[shortest_job] == -1) {
                start_time[shortest_job] = current_time;
            }

            processes[shortest_job].completion_time = current_time + processes[shortest_job].burst_time;
            processes[shortest_job].response_time = start_time[shortest_job] - processes[shortest_job].arrival_time;
            processes[shortest_job].waiting_time = processes[shortest_job].response_time;
            // Tính thêm turnaround time
            processes[shortest_job].turnaround_time = processes[shortest_job].completion_time - processes[shortest_job].arrival_time;

            is_completed[shortest_job] = true;
            completed++;
            current_time = processes[shortest_job].completion_time;
        }
    }
}

void printGanttChart(Process processes[], int n) {
    // Sắp xếp theo thời gian hoàn thành
    Process temp[100];
    for(int i = 0; i < n; i++) {
        temp[i] = processes[i];
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n-1; j++) {
            if(temp[j].completion_time > temp[j+1].completion_time) {
                Process t = temp[j];
                temp[j] = temp[j+1];
                temp[j+1] = t;
            }
        }
    }

    cout << "\nGantt Chart:\n";
    cout << " ";
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < temp[i].burst_time; j++) cout << "--";
        cout << " ";
    }
    cout << "\n|";

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < temp[i].burst_time-1; j++) cout << " ";
        cout << "P" << temp[i].pid;
        for(int j = 0; j < temp[i].burst_time-1; j++) cout << " ";
        cout << "|";
    }
    cout << "\n ";
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < temp[i].burst_time; j++) cout << "--";
        cout << " ";
    }
    cout << "\n";

    int curr_time = 0;
    cout << curr_time;
    for(int i = 0; i < n; i++) {
        curr_time += temp[i].burst_time;
        if(curr_time < 10)cout << "  ";
        else cout << " ";
        for(int j = 1; j < temp[i].burst_time; j++) cout << "  ";
        cout << curr_time;
    }
    cout << endl;
}

int main() {
    int n;
    cout << "Nhap so luong tien trinh: ";
    cin >> n;

    Process processes[100];

    for(int i = 0; i < n; i++) {
        cout << "\nTien trinh " << i+1 << ":\n";
        processes[i].pid = i+1;
        cout << "Arrival time: ";
        cin >> processes[i].arrival_time;
        cout << "Burst time: ";
        cin >> processes[i].burst_time;
    }

    calculateTimes(processes, n);

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

    printGanttChart(processes, n);

    cout << "\nThoi gian doi trung binh: " << fixed << setprecision(2) << avg_waiting_time;
    cout << "\nThoi gian dap ung trung binh: " << avg_response_time;
    cout << "\nThoi gian hoan thanh trung binh: " << avg_turnaround_time << endl;

    return 0;
}
