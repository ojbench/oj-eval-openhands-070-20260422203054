# include <iostream>
# include <vector>
# include "src.hpp"

using namespace std;

namespace cpu_testing {
    enum operationType {AddTask, ChangePriority};

    struct Operation {
        operationType type;
        uint trig_time;
        sjtu::Task task;

        Operation(operationType _type, uint _trig_time, const sjtu::Task &_task) : task(_task) {
            type = _type;
            trig_time = _trig_time;
        }

        ~ Operation() = default;
    };

    vector <Operation> readInputs() {
        vector<Operation> operations;
        int m, op;
        uint trig_time, task_id, priority, time;
        cin >> m;
        for (int i = 0; i < m; ++i) {
            cin >> op >> trig_time;
            if (op == 0) {
                cin >> task_id >> priority >> time;
                operations.emplace_back(Operation(AddTask, trig_time, sjtu::Task(task_id, priority, time)));
            } else {
                cin >> task_id >> priority;
                operations.emplace_back(Operation(ChangePriority, trig_time, sjtu::Task(task_id, priority)));
            }
        }
        return operations;
    }

    void processOutputs(const vector<pair<sjtu::CPUState, uint> > &ans) {
        for (auto log: ans) {
            if (log.first == sjtu::busy && log.second == 0) {
                cerr << "[Error] Busy CPU is not processing any task.\n";
                exit(1);
            }
            if (log.first == sjtu::idle && log.second != 0) {
                cerr << "[Error] Idle CPU is still processing tasks.\n";
                exit(1);
            }
            cout << log.first << ' ' << log.second << endl;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr), cout.tie(nullptr);

    vector<cpu_testing::Operation> operations(cpu_testing::readInputs());

    sjtu::CPU *processor;
    int CPUType;
    cin >> CPUType;
    switch (CPUType) {
        case 0:
            processor = new sjtu::CPU_FCFS();
            break;
        case 1:
            processor = new sjtu::CPU_SRTF();
            break;
        case 2:
            processor = new sjtu::CPU_PRIORITY();
            break;
        default:
            cerr << "[Error] Unexpected CPU type.";
            return 1;
    }

    vector<pair<sjtu::CPUState, uint> > ans;
    int time = 0, cur = 0;
    while (time <= 1000) {
        while (cur < (int)operations.size() && operations[cur].trig_time == (uint)time) {
            int success = 0;
            if (operations[cur].type == cpu_testing::AddTask)
                success = processor->addTask(operations[cur].task);
            if (operations[cur].type == cpu_testing::ChangePriority)
                success = processor->changePriority(operations[cur].task.task_id, operations[cur].task.priority);
            if (success == 0) {
                cerr << "[Error] Fail to execute operation " << cur << ".\n";
            }
            ++ cur;
        }
        ans.push_back(processor->run());
        ++ time;
    }

    cpu_testing::processOutputs(ans);
    delete processor;
    return 0;
}
