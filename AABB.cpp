#include "AABB.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
using namespace std;
AABBManager::AABBManager(){
    aabbs = vector<AABB>();
}
void AABBManager::add_aabb(AABB to_add){
    aabbs.push_back(to_add);
}

bool AABBManager::check_collision(AABB to_check){
    for (int i = 0; i < aabbs.size(); i++){
        // cout<<aabbs[i].min.x<<" "<<aabbs[i].min.y<<" "<<aabbs[i].min.z<<" "<<aabbs[i].max.x<<" "<<aabbs[i].max.y<<" "<<aabbs[i].max.z<<endl;
        if (aabbs[i].max.x > to_check.min.x && aabbs[i].min.x < to_check.max.x && aabbs[i].max.y > to_check.min.y && aabbs[i].min.y < to_check.max.y && aabbs[i].max.z > to_check.min.z && aabbs[i].min.z < to_check.max.z){
            // cout<<"Something somewhere has collided"<<endl;
            return true;
        }
    }
    // cout << "to_check AABB: (" << to_check.min.x << "," << to_check.min.y << "," << to_check.min.z 
    // << ") to (" << to_check.max.x << "," << to_check.max.y << "," << to_check.max.z << ")" << endl;
    
    return false;
}

bool AABBManager::check_intersect(AABB aabb1, AABB aabb2){
    cout<<aabb1.min.x<<" "<<aabb1.min.y<<" "<<aabb1.min.z<<" "<<aabb1.max.x<<" "<<aabb1.max.y<<" "<<aabb1.max.z<<endl;
    cout<<aabb2.min.x<<" "<<aabb2.min.y<<" "<<aabb2.min.z<<" "<<aabb2.max.x<<" "<<aabb2.max.y<<" "<<aabb2.max.z<<endl;
    if (aabb1.max.x > aabb2.min.x && aabb1.min.x < aabb2.max.x && aabb1.max.y > aabb2.min.y && aabb1.min.y < aabb2.max.y && aabb1.max.z > aabb2.min.z && aabb1.min.z < aabb2.max.z){
        cout<<"we have a collision"<<endl;
        return true;
    }
    return false;
}