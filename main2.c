/** @file       encoderpassword.c

 */
#define FILE_ENCODERPASSWORD_C

#include "encoderpassword.h"
#include "dev/pattern.h"
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#define DEBUG_FILE_NAME ""

// #define DEBUG_PORT_
// DEBUG_START(0)
    int i = 0;
    int level=3;
    int colors[15]={0};
    int true_colors[15]={0};
    int isTrue=1;
    const int MAX_LEVEL = 5;
    
/**
 *  @addtogroup ENCODERPASSWORD
 *  @{
 */

tsPatternNode locked[] =
{
    PATTERN_NODE(NOTE_7_SOL, 2000),
    PATTERN_NODE_END(0)
};


const tsPatternNode unLocked[] =
{
    PATTERN_NODE(NOTE_7_SOL, 75),
    PATTERN_NODE(0, 50),
    PATTERN_NODE(NOTE_7_SOL, 75),
    PATTERN_NODE(0, 50),
    PATTERN_NODE(NOTE_7_SOL, 75),
    PATTERN_NODE(0, 50),
    PATTERN_NODE(NOTE_7_SOL, 75),
    PATTERN_NODE_END(0)
};

const tsPatternNode buttonPress[] =
{
    PATTERN_NODE(NOTE_7_SOL, 150),
    PATTERN_NODE(0, 10),
    PATTERN_NODE(NOTE_7_SOL, 100),
    PATTERN_NODE_END(0)
};


static PT_THREAD(encoderPasswordThread)
{
    tsEncoderPasswordParams *params       = process->parameters;
    const tsEncoderPasswordConsts *consts = process->constants;
    
    UNUSED(consts);
    UNUSED(params);

    PT_BEGIN();

    for (;;)
    {
        PT_YIELD();
    }

    PT_END();
}

static PROCESS_HANDLER_PROTO(encoderPasswordInitialStateHandler);
static PROCESS_HANDLER_PROTO(encoderPasswordLockingStateHandler);
static PROCESS_HANDLER_PROTO(encoderPasswordUnLockingStateHandler);

/// @brief  Initialization function of encoderPassword
PROCESS_INIT_PROTO(encoderPasswordInit)
{
    tsEncoderPasswordParams *params       = process->parameters;
    const tsEncoderPasswordConsts *consts = process->constants;

    PROCESS_STATE_CHANGE(process, encoderPasswordInitialStateHandler);
    eventPost(process->enumeration, eEPEventUIUpdate, NULL, 0);

    devIoInit(consts->redLed, NULL);
    devIoInit(consts->blueLed, NULL);
    devIoInit(consts->yellowLed, NULL);
    devIoInit(consts->sevenSegmentDisplay, NULL);
    devIoInit(consts->encoder, NULL);

    params->timerUIGeneral.post.destination = process->enumeration;
    params->timerUIGeneral.post.source = process->enumeration;

    threadStart(process, encoderPasswordThread);
    process->initialized = 1; // If process needs other checks, set this another time
}

/// @brief  Deinitialization function of encoderPassword
PROCESS_DEINIT_PROTO(encoderPasswordDeinit)
{
    tsEncoderPasswordParams *params       = process->parameters;
    const tsEncoderPasswordConsts *consts = process->constants;

    UNUSED(params); // REMOVE IF USED
    UNUSED(consts); // REMOVE IF USED

    PROCESS_STATE_CHANGE(process, NULL);
    threadStop(process, process->threadFunction);
    process->initialized = 0; // If process needs other checks, clear this another time
}

/// @brief  Event handler function of encoderPassword Initial state


/// @brief  Event handler function of encoderPassword locking state
static PROCESS_HANDLER_PROTO(encoderPasswordInitialStateHandler)
{
    
    tsEncoderPasswordParams *params       = process->parameters;
    const tsEncoderPasswordConsts *consts = process->constants;

    UNUSED(params); // REMOVE IF USED
    UNUSED(consts); // REMOVE IF USED
///guessing state
//    while(i<level){
    switch (eventCurrent.event)
    {
         case eStateInit:
            
        {
            timerEventStart(&(params->timerUIGeneral), 100);
            eventPost(process->enumeration,eStateSimon,NULL,0);  
        }
        break;
        case eStateGuess:
        {
            
            switch(eventCurrent.event)
            {
                case p46Pressed:
                
            {
                
                devIoPut(consts->redLed, 1);
                eventPost(process->enumeration,eStateGuess,NULL,0);
            }
            break;
            case p46Released:
            {   
                   if (true_colors[i] == 1)
                    {
                        i++; 
                        if(i==level)
                        {
                            level++;devIoPut(consts->sevenSegmentDisplay, level); devIoPut(consts->buzzer, (uint32_t)locked); // you failed
                            devIoPut(consts->sevenSegmentDisplay, level);  eventPost(process->enumeration,eStateSimon,NULL,0);
                        }// true, increase index
                        devIoPut(consts->sevenSegmentDisplay, level); 
                    }
                    else 
                    {
                        isTrue = 0;i=0;level=3;
                        devIoPut(consts->buzzer, (uint32_t)unLocked); // you failed
                        eventPost(process->enumeration,eStateInit,NULL,0);
                        
                    }

                devIoPut(consts->redLed, 0);
                eventPost(process->enumeration,eStateGuess,NULL,0);
            }
            break;

            case p70Pressed:
            {
                devIoPut(consts->blueLed, 1);
                eventPost(process->enumeration,eStateGuess,NULL,0);
            }
            break;
            case p70Released:
            {
                
                   if (true_colors[i] == 2)
                    {
                        i++; // true increase index
                         if(i==level)
                        {
                            level++;devIoPut(consts->sevenSegmentDisplay, level); devIoPut(consts->buzzer, (uint32_t)locked); // you failed
                            devIoPut(consts->sevenSegmentDisplay, level); eventPost(process->enumeration,eStateSimon,NULL,0);
                        }// true, increase index
                        devIoPut(consts->sevenSegmentDisplay, level); 
                         
                    }
                    else 
                    {
                        isTrue = 0;i=0;level=3;
                        devIoPut(consts->sevenSegmentDisplay, level); devIoPut(consts->buzzer, (uint32_t)locked); // you failed
                        eventPost(process->enumeration,eStateInit,NULL,0);
                        
                    }
                 devIoPut(consts->blueLed, 0);
                eventPost(process->enumeration,eStateGuess,NULL,0);
     
                }

            break;
                case p71Pressed:
           
            {
                
                devIoPut(consts->yellowLed, 1);
                eventPost(process->enumeration,eStateGuess,NULL,0);
            }
            break;
                case p71Released:
            {
                
                   if (true_colors[i] == 3)
                    {
                        
                        i++; // true increase index
                        if(i==level)
                            {
                                level++;devIoPut(consts->sevenSegmentDisplay, level); devIoPut(consts->buzzer, (uint32_t)locked); // you failed
                                devIoPut(consts->sevenSegmentDisplay, level);  eventPost(process->enumeration,eStateSimon,NULL,0);
                            }// true, increase index
                            devIoPut(consts->sevenSegmentDisplay, level); 
                        
                    }
                    else 
                    {
                        isTrue = 0;i=0;level=3;
                        devIoPut(consts->buzzer, (uint32_t)unLocked); // you failed
                        eventPost(process->enumeration,eStateInit,NULL,0);
                        
                    }

               devIoPut(consts->yellowLed, 0); 
            eventPost(process->enumeration,eStateGuess,NULL,0);
            }
            break;
            default:
            eventPost(process->enumeration,eStateGuess,NULL,0);
            break;
            }
            
        }
          
            case eStateSimon:
            {
                
                for(int k=0;k<level;k++)
                {
                    true_colors[k] = rand() % 3 +1; // fill simon says array with random numbers between 1 and 3
                    
                }
                while(i<level)
                {
                    if(true_colors[i] == 1)
                    
                    {
                      devIoPut(consts->redLed, 1);CyDelay(500);devIoPut(consts->redLed, 0);
                    }
                    else if(true_colors[i] == 2)
                    {
                       devIoPut(consts->blueLed, 1);CyDelay(500);devIoPut(consts->blueLed, 0);
                    }
                    else if(true_colors[i] == 3)
                    {
                      devIoPut(consts->yellowLed, 1);CyDelay(500);devIoPut(consts->yellowLed, 0);
                    }
                i++;
                }
                eventPost(process->enumeration,eStateGuess,NULL,0);
                i=0;
            }
            break;
        
        
        default:
            
            eventPost(process->enumeration,eStateInit,NULL,0);
            break;
    
    }
    }
/*
static PROCESS_HANDLER_PROTO(encoderPasswordInitialStateHandler)
{
    tsEncoderPasswordParams *params       = process->parameters;
    const tsEncoderPasswordConsts *consts = process->constants;

    UNUSED(params); // REMOVE IF USED
    UNUSED(consts); // REMOVE IF USED
    devIoPut(consts->sevenSegmentDisplay, level); 
    srand(10000);     
           for(int k=0;k<level;k++)
            {
                true_colors[k] = rand() % 3 +1; // fill simon says array with random numbers between 1 and 3
                
            }
            while(i<level)
            {
                if(true_colors[i] == 1)
                {
                  devIoPut(consts->redLed, 1);CyDelay(1000);devIoPut(consts->redLed, 0);
                }
                else if(true_colors[i] == 2)
                {
                   devIoPut(consts->blueLed, 1);CyDelay(1000);devIoPut(consts->blueLed, 0);
                }
                else if(true_colors[i] == 3)
                {
                  devIoPut(consts->yellowLed, 1);CyDelay(1000);devIoPut(consts->yellowLed, 0);
                }
            i++;
          
            }
            i=0;
            PROCESS_STATE_CHANGE(process, encoderPasswordUnLockingStateHandler);
        }
        */

