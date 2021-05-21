void runServer()
{
    service svr=NewServer();
	svr.Run();
}

void Execute()
{
    runServer();
}

int main()
{
    Execute();

	return 0;
}



class service{
    void Run();
	void handleListener();
};

void service::Run(){
    
}

void service::handleListener(){
    
}


class control{


};

