#ifndef _SJTU_CPP_FINAL_SRC_HPP_
#define _SJTU_CPP_FINAL_SRC_HPP_

#include <vector>
#include "task.hpp"

using namespace std;

typedef unsigned int uint;

namespace sjtu {
    // CPU base class, modifications is not allowed.
    class CPU {
    protected:
        CPUState state;
        vector<Task> tasks;

    public:
        CPU() : tasks() { state = idle; }

        int addTask(const Task &t) {
            tasks.push_back(t);
            return 1;
        }

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

    class CPU_FCFS : public CPU {
    public:
        pair<CPUState, uint> run() override {
            if (tasks.empty()) {
                state = idle;
                return make_pair(idle, 0u);
            }
            state = busy;
            Task &t = tasks.front();
            uint id = t.task_id;
            if (t.time > 0) --t.time;
            if (t.time == 0) tasks.erase(tasks.begin());
            return make_pair(busy, id);
        }
    };

    class CPU_SRTF : public CPU {
    public:
        pair<CPUState, uint> run() override {
            if (tasks.empty()) {
                state = idle;
                return make_pair(idle, 0u);
            }
            size_t idx = 0;
            for (size_t i = 1; i < tasks.size(); ++i)
                if (tasks[i].time < tasks[idx].time) idx = i;
            state = busy;
            uint id = tasks[idx].task_id;
            if (tasks[idx].time > 0) --tasks[idx].time;
            if (tasks[idx].time == 0) tasks.erase(tasks.begin() + static_cast<long>(idx));
            return make_pair(busy, id);
        }
    };

    class CPU_PRIORITY : public CPU {
    public:
        pair<CPUState, uint> run() override {
            if (tasks.empty()) {
                state = idle;
                return make_pair(idle, 0u);
            }
            size_t idx = 0;
            for (size_t i = 1; i < tasks.size(); ++i)
                if (tasks[i].priority < tasks[idx].priority) idx = i;
            state = busy;
            uint id = tasks[idx].task_id;
            if (tasks[idx].time > 0) --tasks[idx].time;
            if (tasks[idx].time == 0) tasks.erase(tasks.begin() + static_cast<long>(idx));
            return make_pair(busy, id);
        }
    };
}

#endif
