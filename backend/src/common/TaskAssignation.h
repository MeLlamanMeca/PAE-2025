#pragma once

struct TaskAssignation {
    int robotID;
    int position;
    int taskID;
    
    TaskAssignation(int robotID, int position, int taskID) : robotID(robotID), position(position), taskID(taskID) {}
};