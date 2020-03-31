#pragma once

#include "FreeRTOS.h"
#include "event_groups.h"

#include "config.hpp"
#include "utility/log.hpp"
#include "utility/rtos.hpp"

namespace sjsu
{
namespace rtos
{
// Forward declaration of TaskInterface to prevent cyclic references.
class TaskInterface;

/// A FreeRTOS task scheduler responsible for scheduling tasks that inherit the
/// Task interface.Tasks inheriting Task interface are automatically added to
/// the scheduler when constructed.
class TaskScheduler final
{
 public:
  TaskScheduler()
      : task_list_{ nullptr },
        task_count_(0),
        pre_run_event_group_handle_(NULL),
        pre_run_sync_bits_(0x0)
  {
  }

  /// @return The PreRun event group handler used to notify that PreRun of all
  ///         tasks have completed successfully.
  EventGroupHandle_t GetPreRunEventGroupHandle() const
  {
    return pre_run_event_group_handle_;
  }

  /// @return The sync bits of the PreRun event group.
  EventBits_t GetPreRunSyncBits() const
  {
    return pre_run_sync_bits_;
  }

  /// @return The current number of scheduled tasks.
  uint8_t GetTaskCount() const
  {
    return task_count_;
  }

  /// Add a task to the task scheduler. If the scheduler is full, the task will
  /// not be added and a fatal error will be asserted.
  ///
  /// @note When a task inheriting the TaskInterface is constructed, it will
  ///       automatically call this function to add itself to the scheduler.
  ///
  /// @param task Task to add.
  void AddTask(TaskInterface * task);

  /// Removes a specified task by its name and updates the task_list_ and
  /// task_count_.
  ///
  /// @param task_name  Name of the task to remove.
  void RemoveTask(const char * task_name);

  /// Retreive a task by its task name.
  ///
  /// @param  task_name Name of the task.
  /// @return A nullptr if the task does not exist. Otherwise, returns a pointer
  ///         reference to the retrieved task with the matching name.
  TaskInterface * GetTask(const char * task_name) const
  {
    const uint32_t kTaskIndex = GetTaskIndex(task_name);
    if (kTaskIndex > task_count_)
    {
      return nullptr;
    }
    return task_list_[kTaskIndex];
  }

  /// Used to get a task's index to determine the sync bit for the PreRun event
  /// group.
  ///
  /// @param  task_name Name of the task.
  /// @return The index of the specified task. If the task is not scheduled,
  ///         kTaskSchedulerSize + 1 will be returned.
  uint8_t GetTaskIndex(const char * task_name) const;

  /// @return A pointer reference to an immutable array of all currently
  ///         scheduled tasks.
  TaskInterface * const * GetAllTasks() const
  {
    return task_list_;
  }

  /// Starts the scheduler and attempts to initialize all tasks.
  /// If there are currently no tasks scheduled, a fatal error will be
  /// asserted.
  void Start()
  {
    SJ2_ASSERT_FATAL(
        task_count_ != 0,
        "Attempting to start TaskScheduler but there are no tasks that "
        "are currently scheduled.");
    InitializeAllTasks();
    vTaskStartScheduler();
    // does not reach this point
  }

 private:
  /// Function used during InitializeAllTasks() for xTaskCreate() for running
  /// scheduled tasks.
  ///
  /// @param task_pointer Pointer reference of the task to run.
  static void RunTask(void * task_pointer);

  /// Attempt to initialize each scheduled task with xTaskCreate() and execute
  /// each task's Setup(). A fatal error is asserted if either
  /// xTaskCreateStatic() or Setup() fails.
  void InitializeAllTasks();

  /// Array containing all scheduled tasks.
  TaskInterface * task_list_[config::kTaskSchedulerSize];
  /// Current number of scheduled tasks in task_list_.
  uint8_t task_count_;
  /// Buffer to hold the static allocation of the PreRun Event Group.
  StaticEventGroup_t pre_run_event_group_buffer_;
  /// Event Group handle to be used by tasks running their PreRun callback to
  /// indicate that all of them have completed it.
  EventGroupHandle_t pre_run_event_group_handle_;
  /// All PreRun sync bits for PreRun Event Group.
  EventBits_t pre_run_sync_bits_;
};
}  // namespace rtos
}  // namespace sjsu
