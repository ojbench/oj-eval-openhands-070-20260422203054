#ifndef _SJTU_CPP_FINAL_SRC_HPP_
#define _SJTU_CPP_FINAL_SRC_HPP_

#include <vector>
#include <utility>

using namespace std;

typedef unsigned int uint;

namespace sjtu {
    // Task definition
    struct Task {
        uint task_id;
        uint priority;
        uint time;

        explicit Task(uint _task_id = 0, uint _priority = 0, uint _time = 0) {
            task_id = _task_id;
            priority = _priority;
            time = _time;
        }

        Task(const Task &rhs) {
            task_id = rhs.task_id;
            priority = rhs.priority;
            time = rhs.time;
        }

        ~Task() = default;
    };

    enum CPUState { idle = 0, busy = 1 };

    // CPU base class, modifications is not allowed.
    class CPU {
    protected:
        CPUState state;
        vector<Task> tasks;

    public:
        CPU() : tasks() { state = idle; }

        // Add a new task.
        int addTask(const Task &t) {
            tasks.push_back(t);
            return 1;
        }

        // Change the priority of one process, return 1 if success and return 0 if fail.
        int changePriority(uint task_id, uint priority) {
            for (auto &task : tasks)
                if (task.task_id == task_id) {
                    task.priority = priority;
                    return 1;
                }
            return 0;
        }

        virtual pair<CPUState, uint> run() = 0;

        virtual ~CPU() = default;
    };

    // FCFS method based CPU.
    class CPU_FCFS : public CPU {
    public:
        pair<CPUState, uint> run() override {
            if (tasks.empty()) {
                state = idle;
                return make_pair(idle, 0u);
            }
            // Execute the earliest added task
            state = busy;
            Task &t = tasks.front();
            uint id = t.task_id;
            if (t.time > 0) {
                --t.time;
            }
            if (t.time == 0) {
                tasks.erase(tasks.begin());
            }
            return make_pair(busy, id);
        }
    };

    // SRTF method based CPU.
    class CPU_SRTF : public CPU {
    public:
        pair<CPUState, uint> run() override {
            if (tasks.empty()) {
                state = idle;
                return make_pair(idle, 0u);
            }
            // Find task with minimum remaining time; tie by arrival order (index)
            size_t idx = 0;
            for (size_t i = 1; i < tasks.size(); ++i) {
                if (tasks[i].time < tasks[idx].time) idx = i;
            }
            state = busy;
            uint id = tasks[idx].task_id;
            if (tasks[idx].time > 0) {
                --tasks[idx].time;
            }
            if (tasks[idx].time == 0) {
                tasks.erase(tasks.begin() + static_cast<long>(idx));
            }
            return make_pair(busy, id);
        }
    };

    // priority method based CPU.
    class CPU_PRIORITY : public CPU {
    public:
        pair<CPUState, uint> run() override {
            if (tasks.empty()) {
                state = idle;
                return make_pair(idle, 0u);
            }
            // Find task with highest priority (smallest priority value); tie by arrival order
            size_t idx = 0;
            for (size_t i = 1; i < tasks.size(); ++i) {
                if (tasks[i].priority < tasks[idx].priority) idx = i;
            }
            state = busy;
            uint id = tasks[idx].task_id;
            if (tasks[idx].time > 0) {
                --tasks[idx].time;
            }
            if (tasks[idx].time == 0) {
                tasks.erase(tasks.begin() + static_cast<long>(idx));
            }
            return make_pair(busy, id);
        }
    };
}

#endif
