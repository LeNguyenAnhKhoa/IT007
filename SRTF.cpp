#include <iostream>
#include <algorithm>
#include <iomanip>
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
};

void calculateTimes(Process processes[], int n) {
    int current_time = 0;
    int completed = 0;
    bool is_completed[100] = {0};
    int start_time[100];

    // Khởi tạo remaining_time và start_time
    for(int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        start_time[i] = -1;
    }

    while(completed != n) {
        int shortest_job = -1;
        int min_remaining = INT_MAX;

        // Tìm process có remaining time ngắn nhất tại thời điểm hiện tại
        for(int i = 0; i < n; i++) {
            if(processes[i].arrival_time <= current_time && !is_completed[i]) {
                if(processes[i].remaining_time < min_remaining) {
                    min_remaining = processes[i].remaining_time;
                    shortest_job = i;
                }
                if(processes[i].remaining_time == min_remaining) {
                    if(processes[i].arrival_time < processes[shortest_job].arrival_time) {
                        shortest_job = i;
                    }
                }
            }
        }

        if(shortest_job == -1) {
            current_time++;
        } else {
            // Ghi nhận thời điểm bắt đầu xử lý process lần đầu tiên
            if(start_time[shortest_job] == -1) {
                start_time[shortest_job] = current_time;
                processes[shortest_job].response_time = start_time[shortest_job] - processes[shortest_job].arrival_time;
            }

            processes[shortest_job].remaining_time--;
            current_time++;

            // Kiểm tra nếu process đã hoàn thành
            if(processes[shortest_job].remaining_time == 0) {
                processes[shortest_job].completion_time = current_time;
                processes[shortest_job].turnaround_time = processes[shortest_job].completion_time - processes[shortest_job].arrival_time;
                processes[shortest_job].waiting_time = processes[shortest_job].turnaround_time - processes[shortest_job].burst_time;

                is_completed[shortest_job] = true;
                completed++;
            }
        }
    }
}

void printGanttChart(Process processes[], int n) {
    // Tạo mảng timeline để lưu trữ thứ tự thực thi
    int total_time = 0;
    for(int i = 0; i < n; i++) {
        total_time = max(total_time, processes[i].completion_time);
    }

    int timeline[1000];
    for(int i = 0; i < total_time; i++) {
        timeline[i] = -1;
    }

    // Mô phỏng lại quá trình thực thi để lấy timeline
    int remaining_time[100];
    for(int i = 0; i < n; i++) {
        remaining_time[i] = processes[i].burst_time;
    }

    int current_time = 0;
    while(current_time < total_time) {
        int shortest_job = -1;
        int min_remaining = INT_MAX;

        for(int i = 0; i < n; i++) {
            if(processes[i].arrival_time <= current_time && remaining_time[i] > 0) {
                if(remaining_time[i] < min_remaining) {
                    min_remaining = remaining_time[i];
                    shortest_job = i;
                }
                if(remaining_time[i] == min_remaining) {
                    if(processes[i].arrival_time < processes[shortest_job].arrival_time) {
                        shortest_job = i;
                    }
                }
            }
        }

        if(shortest_job != -1) {
            timeline[current_time] = processes[shortest_job].pid;
            remaining_time[shortest_job]--;
        }
        current_time++;
    }

    // In Gantt chart
    cout << "\nGantt Chart:\n";
    cout << " ";

    // In dòng đầu tiên của biểu đồ
    int prev_pid = timeline[0];
    int count = 1;
    for(int i = 1; i <= total_time; i++) {
        if(i == total_time || timeline[i] != prev_pid) {
            for(int j = 0; j < count; j++) cout << "--";
            cout << " ";
            count = 1;
            prev_pid = (i < total_time) ? timeline[i] : -1;
        } else {
            count++;
        }
    }
    cout << "\n|";

    // In dòng giữa của biểu đồ (process ID)
    prev_pid = timeline[0];
    count = 1;
    for(int i = 1; i <= total_time; i++) {
        if(i == total_time || timeline[i] != prev_pid) {
            for(int j = 0; j < count-1; j++) cout << " ";
            if(prev_pid != -1) cout << "P" << prev_pid;
            else cout << " ";
            for(int j = 0; j < count-1; j++) cout << " ";
            cout << "|";
            count = 1;
            prev_pid = (i < total_time) ? timeline[i] : -1;
        } else {
            count++;
        }
    }
    cout << "\n ";

    // In dòng cuối của biểu đồ
    prev_pid = timeline[0];
    count = 1;
    for(int i = 1; i <= total_time; i++) {
        if(i == total_time || timeline[i] != prev_pid) {
            for(int j = 0; j < count; j++) cout << "--";
            cout << " ";
            count = 1;
            prev_pid = (i < total_time) ? timeline[i] : -1;
        } else {
            count++;
        }
    }
    cout << "\n";

    // In thời gian
    current_time = 0;
    cout << current_time;
    prev_pid = timeline[0];
    count = 1;
    for(int i = 1; i <= total_time; i++) {
        if(i == total_time || timeline[i] != prev_pid) {
            current_time += count;
            if(current_time < 10) cout << "  ";
            else cout << " ";
            for(int j = 1; j < count; j++) cout << "  ";
            cout << current_time;
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
