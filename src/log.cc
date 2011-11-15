#include <log.hh>

bool shouldLog(const char* file, const int line, const char* func, const constants::LogLevel level)
{
   if(level < constants::static_log_level)
   {
      return false;
   }
   else
   {
      const map<string, map<string, constants::LogLevel> >& configs = constants::__private_log_configs;

      const map<string, map<string, constants::LogLevel> >::const_iterator findFileIterator = configs.find(string(file));
      if(findFileIterator == configs.end())
      {
	 return level >= constants::default_log_level;
      }
      else
      {
	 const map<string, constants::LogLevel> subConfig = (*findFileIterator).second;
	 const map<string, constants::LogLevel>::const_iterator emptyFunctionIterator = subConfig.find(string(""));
	 
	 if(emptyFunctionIterator == subConfig.end())
	 {
	    for(map<string, constants::LogLevel>::const_iterator i = subConfig.begin(); i != subConfig.end(); i++)
	    {
	       const string key = (*i).first;
	       const constants::LogLevel value = (*i).second;
	       
	       if(starts_with<string, string>(string(func), key))
	       {
		  return level >= value;;
	       }
	    }
	    return level >= constants::default_log_level;
	 }
	 else
	 {
	    return level >= (*emptyFunctionIterator).second;
	 }
      }
   }
}
