#pragma once
#include <iostream>
#include <vector>
#include "Src\yolo_v2_class.hpp"


using namespace std;


void SplitString(const string& s, vector<string>& v, const string& c);
bool sortFun(bbox_t p1, bbox_t p2);
