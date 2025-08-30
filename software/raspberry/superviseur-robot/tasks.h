/*
 * Copyright (C) 2018 dimercur
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __TASKS_H__
#define __TASKS_H__

#include <unistd.h>
#include <iostream>

#include <sys/mman.h>
#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <alchemy/mutex.h>
#include <alchemy/sem.h>
#include <alchemy/queue.h>

#include "messages.h"
#include "commonitor.h"
#include "comrobot.h"
#include "camera.h"
#include "img.h"

using namespace std;

class Tasks {
public:
    /**
     * @brief Initializes main structures (semaphores, tasks, mutex, etc.)
     */
    void Init();

    /**
     * @brief Starts tasks
     */
    void Run();

    /**
     * @brief Stops tasks
     */
    void Stop();
    
    /**
     * @brief Suspends main thread
     */
    void Join();
    
private:
    /**********************************************************************/
    /* Shared data                                                        */
    /**********************************************************************/
    ComMonitor monitor;
    ComRobot robot;
    Camera camera;
    int CompteurPosition=0;
    int robotStarted = 0;
    int robotStartedW= 0;
    int move = MESSAGE_ROBOT_STOP;
    int CompteurPing = 0;
    bool statutOpenCamera= false;
    bool StatutArena=0;
    bool GrabPosition=false;
    bool ArenaExist = false;
    Arena arenaConfirmed;
    
    /**********************************************************************/
    /* Tasks                                                              */
    /**********************************************************************/
    RT_TASK th_server;
    RT_TASK th_sendToMon;
    RT_TASK th_receiveFromMon;
    RT_TASK th_openComRobot;
    RT_TASK th_startRobot;
    RT_TASK th_move;
    RT_TASK th_battery;
    RT_TASK th_cameraStart;
    RT_TASK th_cameraStop;
    RT_TASK th_cameraSendImage;
    RT_TASK th_FindArena;
    RT_TASK th_ConfirmInfirm;
    RT_TASK th_MonitorLost;
    RT_TASK th_MessageWatchdog;
    RT_TASK th_startRobotWatchdog;
    RT_TASK th_PingRobot;
    RT_TASK th_RobotLost;
    /**********************************************************************/
    /* Mutex                                                              */
    /**********************************************************************/
    RT_MUTEX mutex_monitor;
    RT_MUTEX mutex_robot;
    RT_MUTEX mutex_robotStarted;
    RT_MUTEX mutex_robotStartedW;
    RT_MUTEX mutex_move;
    RT_MUTEX mutex_camera;
    RT_MUTEX mutex_position;
    RT_MUTEX mutex_arena;
    RT_MUTEX mutex_CompteurPing;
    /**********************************************************************/
    /* Semaphores                                                         */
    /**********************************************************************/
    RT_SEM sem_barrier;
    RT_SEM sem_openComRobot;
    RT_SEM sem_serverOk;
    RT_SEM sem_startRobot;
    RT_SEM sem_Battery;
    RT_SEM sem_startCamera;
    RT_SEM sem_startCameraSend;
    RT_SEM sem_stopCamera;
    RT_SEM sem_findArena;
    RT_SEM sem_ArenaCameraConfirm;
    RT_SEM sem_MonitorLost;
    RT_SEM sem_startRobotWatchdog;
    RT_SEM sem_RobotLost;


    /**********************************************************************/
    /* Message queues                                                     */
    /**********************************************************************/
    int MSG_QUEUE_SIZE;
    RT_QUEUE q_messageToMon;
    
    /**********************************************************************/
    /* Tasks' functions                                                   */
    /**********************************************************************/
    /**
     * @brief Thread handling server communication with the monitor.
     */
    void ServerTask(void *arg);
     
    /**
     * @brief Thread sending data to monitor.
     */
    void SendToMonTask(void *arg);
        
    /**
     * @brief Thread receiving data from monitor.
     */
    void ReceiveFromMonTask(void *arg);
    
    /**
     * @brief Thread opening communication with the robot.
     */
    void OpenComRobot(void *arg);

    /**
     * @brief Thread starting the communication with the robot.
     */
    void StartRobotTask(void *arg);
    
    /**
     * @brief Thread handling control of the robot.
     */
    void MoveTask(void *arg);
    
     /**
     * @brief Battery State.
     */
    void BatteryState(void *arg);
    
     /**
     * @brief Open Camera.
     */
    void OpenCamera(void *arg);
    
    /**
     * @brief Close Camera.
     */
    void CloseCamera(void *arg);
    
    /**
     * @brief Display Image.
     */
    void DisplayImagePosition(void *arg);
    
    /**
     * @brief Find Arena.
     */
    void TaskFindArena(void *arg);

    /**
     * @brief Confirm or Infirm Arena.
     */
    void CameraConfirmInfirm(void *arg);
    
    /**
     * @brief Lost Connection With Monitor.
     */
    void LostConnectionWithMonitor(void *arg);
    
    /**
     * @brief Send the message for the Watchdog.
     */
    void SendWatchdogMessage(void *arg);
    
    /**
     * @brief Start robot with Watchdog.
     */
    void StartRobotTaskWatchdog(void *arg);
    
    /**
     * @brief Checking if the robot is still connected.
     */
    void SendPingRobot(void *arg);
    
    /**
     * @brief Lost Connection With Robot.
     */
    void LostConnectionWithRobot(void *arg);
    
    /**********************************************************************/
    /* Queue services                                                     */
    /**********************************************************************/
    /**
     * Write a message in a given queue
     * @param queue Queue identifier
     * @param msg Message to be stored
     */
    void WriteInQueue(RT_QUEUE *queue, Message *msg);
    
    /**
     * Read a message from a given queue, block if empty
     * @param queue Queue identifier
     * @return Message read
     */
    Message *ReadInQueue(RT_QUEUE *queue);

};

#endif // __TASKS_H__ 
