#include "agent_dll.h"
#include <bits/stdc++.h>
using namespace std;
const size_t NUM_ACTIONS = 9;
enum actions { NOOP = 0, UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT };

#ifdef __cplusplus
extern "C"
{
#endif
//�Q�Ϊ��z����v = v0t + 1/2att �D�Xt(�ɶ�)
int count_time(int a, int V0, int dis){
    return (-V0 + sqrt((V0 * V0) + 2 * a * dis)) / a;
}
int go (int x_a, int y_a){
    if(x_a > 0){
        if(y_a > 0){
            return DOWN_RIGHT;
        }
        else if(y_a == 0){
            return RIGHT;
        }
        else{
            return UP_RIGHT;
        }
    }
    else if(x_a == 0){
        if(y_a > 0){
            return DOWN;
        }
        else if(y_a == 0){
            return NOOP;
        }
        else{
            return UP;
        }
    }
    else{
        if(y_a > 0){
            return DOWN_LEFT;
        }
        else if(y_a == 0){
            return LEFT;
        }
        else{
            return UP_LEFT;
        }
    }
}
__declspec(dllexport) void controller(int &action, const size_t agent, const size_t num_agents, const size_t num_resources, const int* circleRadius,
                                      const int* xCoordinate, const int* yCoordinate, const int* xVelocity, const int* yVelocity) // the coordinates of  balls and resource centers are in turn placed in the array xCoordinate, and yCoordinate
{
    int MAX_SPEED = 6;
    double mintime = double(INT_MAX);
    int my_radius = circleRadius[0];
    pair<double, double>a;
    pair<int, int>ans_a;
    int pos[9][2];
    memset(pos, 0, sizeof(pos));
    pos[1][0] = my_radius, pos[1][1] = 0;
    pos[2][0] = sqrt(my_radius) / 2, pos[2][1] = -sqrt(my_radius) / 2;
    pos[3][0] = 0, pos[3][1] = -my_radius;
    pos[4][0] = -sqrt(my_radius) / 2, pos[4][1] = -sqrt(my_radius) / 2;
    pos[5][0] = -my_radius, pos[5][1] = 0;
    pos[6][0] = -sqrt(my_radius) / 2, pos[6][1] = sqrt(my_radius) / 2;
    pos[7][0] = 0, pos[7][1] = my_radius;
    pos[8][0] = sqrt(my_radius) / 2, pos[8][1] = sqrt(my_radius) / 2;

    //�Q�βy�骺�K���P��ߨӧ�Z��
    for(int cnt = 0; cnt <= 8; cnt++){
        int my_x = xCoordinate[0] + pos[cnt][0];
        int my_y = yCoordinate[0] + pos[cnt][1];
        for(int i = 1; i < 15; i++){
            //�p�G��L�y��resources�b�|��0��continue
            if(circleRadius[i] == 0) {
                continue;
            }
            //�p�G��L�y�b�|��ڤj�ɩΥ��s�b�ܾa����ɮ�continue
            else if((circleRadius[i] >= my_radius && i < 10) || !(20 <= xCoordinate[i] && xCoordinate[i] <= 1580) || !(20 <= yCoordinate[i] && yCoordinate[i] <= 880)) {
                continue;
            }
            //��X��L�y�θ귽�b�ڭ̫e���һݭn����V�P�ɶ��A�ç�X�h���ɶ��̧֪�
            else {
                double xtime, ytime;
                if(xCoordinate[i] > my_x) {
                    a.first = 1;
                }
                else if(xCoordinate[i] == my_x) {
                    a.first = 0;
                }
                else{
                    a.first = -1;
                }
                if(yCoordinate[i] > my_y) {
                    a.second = 1;
                }
                else if(yCoordinate[i] == my_y) {
                a.second = 0;
                }
                else{
                    a.second = -1;
                }
                if(a.first == 0){
                    xtime = 0;
                }
                else{
                    xtime = count_time(a.first, (double)xVelocity[i], xCoordinate[i] - my_x);
                }
                if(a.second == 0){
                    ytime = 0;
                }
                else{
                    ytime = count_time(a.second, (double)yVelocity[i], yCoordinate[i] - my_y);
                }
                if(mintime > max(xtime, ytime)){
                    mintime = max(xtime, ytime);
                    ans_a = a;
                }
            }
        }
    }

    // �b�b�|�p��75�ɳ]���t�A�H���ɽ�
    if(abs(xVelocity[0]) >= MAX_SPEED && ans_a.first * xVelocity[0] >= 0 && circleRadius[0] <= 75 ){
        ans_a.first = 0;
    }
    if(abs(yVelocity[0]) >= MAX_SPEED && ans_a.second * yVelocity[0] >= 0 && circleRadius[0] <= 75 ){
        ans_a.second = 0;
    }
    // �]����ɡA�H�K���𦩤�
    if(xCoordinate[0] - my_radius <= 0 + 2 * MAX_SPEED + 5){
        ans_a.first = 1;
    }
    if(xCoordinate[0] + my_radius >= 1600 - 2 * MAX_SPEED + 5){
        ans_a.first= -1;
    }
    if(yCoordinate[0] - my_radius <= 0 + 2 * MAX_SPEED + 5){
        ans_a.second = 1;
    }
    if(yCoordinate[0] + my_radius >= 900 - 2 * MAX_SPEED + 5){
        ans_a.second = -1;
    }
    action = go(ans_a.first, ans_a.second);
    return;
}
#ifdef __cplusplus
}
#endif

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}