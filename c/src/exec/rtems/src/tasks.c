/*
 *  RTEMS Task Manager
 *
 *
 *  COPYRIGHT (c) 1989-1998.
 *  On-Line Applications Research Corporation (OAR).
 *  Copyright assigned to U.S. Government, 1994.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id$
 */

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/support.h>
#include <rtems/rtems/modes.h>
#include <rtems/score/object.h>
#include <rtems/score/stack.h>
#include <rtems/score/states.h>
#include <rtems/rtems/tasks.h>
#include <rtems/score/thread.h>
#include <rtems/score/threadq.h>
#include <rtems/score/tod.h>
#include <rtems/score/userext.h>
#include <rtems/score/wkspace.h>
#include <rtems/score/apiext.h>
#include <rtems/score/sysstate.h>

/*PAGE
 *
 *  _RTEMS_tasks_Create_extension
 *
 *  XXX
 */

boolean _RTEMS_tasks_Create_extension(
  Thread_Control *executing, 
  Thread_Control *created
)
{
  RTEMS_API_Control *api;

  api = _Workspace_Allocate( sizeof( RTEMS_API_Control ) );

  if ( !api )
    return FALSE;

  created->API_Extensions[ THREAD_API_RTEMS ] = api;
 
  api->pending_events = EVENT_SETS_NONE_PENDING;
  _ASR_Initialize( &api->Signal );
  return TRUE;
}

/*PAGE
 *
 *  _RTEMS_tasks_Start_extension
 *
 *  XXX
 */
 
User_extensions_routine _RTEMS_tasks_Start_extension(
  Thread_Control *executing,
  Thread_Control *started
)
{
  RTEMS_API_Control *api;

  api = started->API_Extensions[ THREAD_API_RTEMS ];
 
  api->pending_events = EVENT_SETS_NONE_PENDING;

  _ASR_Initialize( &api->Signal );
}

/*PAGE
 *
 *  _RTEMS_tasks_Delete_extension
 *
 *  XXX
 */
 
User_extensions_routine _RTEMS_tasks_Delete_extension(
  Thread_Control *executing, 
  Thread_Control *deleted
)
{
  (void) _Workspace_Free( deleted->API_Extensions[ THREAD_API_RTEMS ] );
 
  deleted->API_Extensions[ THREAD_API_RTEMS ] = NULL;
}

/*PAGE
 *
 *  _RTEMS_tasks_Switch_extension
 *
 *  XXX
 */
 
void _RTEMS_tasks_Switch_extension(
  Thread_Control *executing
)
{
  ISR_Level          level;
  RTEMS_API_Control *api;
  ASR_Information   *asr;
  rtems_signal_set   signal_set;
  Modes_Control      prev_mode;

  api = executing->API_Extensions[ THREAD_API_RTEMS ];
  asr = &api->Signal;
 
  _ISR_Disable( level );
    signal_set = asr->signals_posted;
    asr->signals_posted = 0;
  _ISR_Enable( level );
 
 
  if ( !signal_set ) /* similar to _ASR_Are_signals_pending( asr ) */
    return;
 
  asr->nest_level += 1;
  rtems_task_mode( asr->mode_set, RTEMS_ALL_MODE_MASKS, &prev_mode );
 
  (*asr->handler)( signal_set );

  asr->nest_level -= 1;
  rtems_task_mode( prev_mode, RTEMS_ALL_MODE_MASKS, &prev_mode );

}

API_extensions_Control _RTEMS_tasks_API_extensions = {
  { NULL, NULL },
  NULL,                                     /* predriver */
  _RTEMS_tasks_Initialize_user_tasks,       /* postdriver */
  _RTEMS_tasks_Switch_extension             /* post switch */
};

User_extensions_Control _RTEMS_tasks_User_extensions = {
  { NULL, NULL },
  { _RTEMS_tasks_Create_extension,            /* create */
    _RTEMS_tasks_Start_extension,             /* start */
    _RTEMS_tasks_Start_extension,             /* restart */
    _RTEMS_tasks_Delete_extension,            /* delete */
    NULL,                                     /* switch */
    NULL,                                     /* begin */
    NULL,                                     /* exitted */
    NULL                                      /* fatal */
  }
};

/*PAGE
 *
 *  _RTEMS_tasks_Manager_initialization
 *
 *  This routine initializes all Task Manager related data structures.
 *
 *  Input parameters:
 *    maximum_tasks       - number of tasks to initialize
 *
 *  Output parameters:  NONE
 */

void _RTEMS_tasks_Manager_initialization(
  unsigned32                        maximum_tasks,
  unsigned32                        number_of_initialization_tasks,
  rtems_initialization_tasks_table *user_tasks
)
{

  _RTEMS_tasks_Number_of_initialization_tasks = number_of_initialization_tasks;
  _RTEMS_tasks_User_initialization_tasks = user_tasks;

  /*
   *  There may not be any RTEMS initialization tasks configured.
   */

#if 0
  if ( user_tasks == NULL || number_of_initialization_tasks == 0 )
    _Internal_error_Occurred( INTERNAL_ERROR_RTEMS_API, TRUE, RTEMS_TOO_MANY );
#endif

  _Objects_Initialize_information(
    &_RTEMS_tasks_Information,
    OBJECTS_RTEMS_TASKS,
    TRUE,
    maximum_tasks,
    sizeof( Thread_Control ),
    FALSE,
    RTEMS_MAXIMUM_NAME_LENGTH,
    TRUE
  );

  /*
   *  Add all the extensions for this API
   */

  _User_extensions_Add_API_set( &_RTEMS_tasks_User_extensions );

  _API_extensions_Add( &_RTEMS_tasks_API_extensions );

  /*
   *  Register the MP Process Packet routine.
   */

#if defined(RTEMS_MULTIPROCESSING)
  _MPCI_Register_packet_processor(
    MP_PACKET_TASKS,
    _RTEMS_tasks_MP_Process_packet
  );
#endif

}

