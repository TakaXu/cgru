#pragma once

#include "../include/afjob.h"

#include "../libafanasy/name_af.h"
#include "../libafanasy/taskprogress.h"

class MsgAf;
class JobAf;
class RenderAf;
class Block;
class TaskRun;
class MonitorContainer;
class RenderContainer;

class Task
{
public:
   Task( const Block * taskBlock, af::TaskProgress * taskProgress, int taskNumber);
   virtual ~Task();

public:

   inline int getNumber() const { return number;}

   virtual void start( af::TaskExec * taskexec, int * runningtaskscounter, RenderAf * render, MonitorContainer * monitoring);

/// Update task state.
   virtual void updateState( const af::MCTaskUp & taskup, RenderContainer * renders, MonitorContainer * monitoring, bool & errorHost);

   virtual void refresh( time_t currentTime, RenderContainer * renders, MonitorContainer * monitoring, int & errorHostId);

   void restart( bool onlyRunning, const std::string & message, RenderContainer * renders, MonitorContainer * monitoring);
   void restartError( const std::string & message, RenderContainer * renders, MonitorContainer * monitoring);
   void skip( const std::string & message, RenderContainer * renders, MonitorContainer * monitoring);

   virtual void appendLog( const std::string  & message);
   inline const std::list<std::string> & getLog() { return logStringList; }

   void errorHostsAppend( const std::string & hostname);
   bool avoidHostsCheck( const std::string & hostname) const;
   virtual void getErrorHostsListString( std::string & str) const;
   const std::string getErrorHostsListString() const;
   inline void errorHostsReset() { errorHosts.clear(); errorHostsCounts.clear(); errorHostsTime.clear();}

   int calcWeight() const;
   int logsWeight() const;
   int blackListWeight() const;

   virtual void writeTaskOutput( const af::MCTaskUp & taskup) const;  ///< Write task output in tasksOutputDir.
   const std::string getOutputFileName( int startcount) const;

/// Construct message for request output from render if task is running, or filename to read output from, if task is not running.
/** Return \c true on success (and valid message or filename) or \c false on fail with error message for client
**/
   bool getOutput( int startcount, MsgAf *msg, std::string & filename, RenderContainer * renders) const;

   void listenOutput( af::MCListenAddress & mclisten, RenderContainer * renders);

   virtual void monitor( MonitorContainer * monitoring) const;
   virtual void updateDatabase() const;

   virtual const std::string getInfo( bool full = false) const;
   virtual void stdOut( bool full = false) const;

   inline bool isReady()   const { return progress->state & AFJOB::STATE_READY_MASK;   }
   inline bool isRunning() const { return progress->state & AFJOB::STATE_RUNNING_MASK; }
   inline bool isDone()    const { return progress->state & AFJOB::STATE_DONE_MASK;    }
   inline bool isError()   const { return progress->state & AFJOB::STATE_ERROR_MASK;   }

protected:
   af::TaskProgress * progress;
   std::list<std::string> logStringList;    ///< Task log.
   const Block * block;

private:
   void deleteRunningZombie();

private:
   int number;

   TaskRun * run;

   std::list<std::string>  errorHosts;       ///< Avoid error hosts list.
   std::list<int>          errorHostsCounts; ///< Number of errors on error host.
   std::list<time_t>       errorHostsTime;   ///< Time of the last error
};
