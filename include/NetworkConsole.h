#ifndef NetworkConsole_h
#define NetworkConsole_h



class NetworkConsole
{
	private:
		struct NetworkConsolePrivateData;
		NetworkConsolePrivateData * Data;
	
	public:
		NetworkConsole();
		void Init();
		~NetworkConsole();
		int send_message(const char * message);
};

#endif