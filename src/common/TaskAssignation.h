#pragma once

struct TaskAssignation {
    int robotID;
    int position;
    
    TaskAssignation(int robotID, int position) : robotID(robotID), position(position) {}
};