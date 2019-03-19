#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>

using namespace std;
int sizee =0;

///////////////// General ////////////////////

template<class T>
double avg_WTime(vector<T>& x )
{
	double sum = 0.0 ;
	for (int i =0 ; i<sizee ;i++)
		sum += x[i].waiting_time();
	return sum/sizee ;
}

template<class T>
void printAll(vector<T>& x)
{
	cout<< endl <<endl;
	cout <<"Name \t"<< "Arrival\t" <<"Burst\t" <<"Departure "<<endl ;
	for (int j=0 ; j<sizee ;j++)
		cout <<x[j].name<<"\t "<< x[j].arrival << "\t " <<x[j].burst <<"\t " <<x[j].departure <<endl ;

}


////////////////// FCFS ////////////////////

class FCFS
{
public:
    string name;
    int arrival ,burst , departure ;

	FCFS()
	{
		arrival =0;
		burst = 0;
		departure = 0;
	}

	FCFS( int a , int b)
		{
			arrival = a;
			burst = b;
		}

	double waiting_time()
	{
		return (double)(departure - arrival - burst) ;
	}

};

void calc_departure(vector<FCFS>& x )
{
	int start , time = 0 ;
		for (int j=0 ; j<sizee ;j++)
	{
		while((x[j].arrival > time)){time++;}
		start = x[j].burst ;
		cout << x[j].name <<"("<< time << ")";
		for( start ; start> 0 ;start--)
		{
			time++;
			cout << "*" ;
		}

		x[j].departure = time ;
	}
		cout <<"("<< time << ")";
		cout<<endl ;

}

bool sortFCFSByArrival(const FCFS &lhs, const FCFS &rhs) { return lhs.arrival < rhs.arrival; }

int FCFSmain()
{
	ifstream ip ("data.csv");
    int i=0 ,inputType;
	string a,b;

	cout<<"Enter no. of processes\n";
    cin>>sizee ;
	cout<<"Enter 1 for input file OR enter 0 for CMD \n";
	cin>>inputType;

	vector<FCFS> process(sizee);
    cout<<endl ;

	//Reading From File

	if(inputType == 1)
	{
		while(ip.good() && i<sizee)
		{
			getline(ip,process[i].name,',');
			getline(ip,a,',');
			getline(ip,b,'\n');
			process[i].arrival = stoi(a);
			process[i].burst = stoi(b) ;
			i++;
		}
	}

	//Reading from CMD

	if (inputType==0)
	{
		for (vector<FCFS>::size_type i = 0; i != sizee; ++i)
		{
			cout << "Process #" << i + 1 << " name: ";
			cin >> process[i].name;

			cout << "Process #" << i + 1 << " arrival time: ";
			cin >> process[i].arrival;

			cout << "Process #" << i + 1 << " burst time ";
			cin >> process[i].burst;
	
    }
	}
    cout << "\n";


    // Sort by arrival
    sort(process.begin(), process.end(), sortFCFSByArrival);

	calc_departure(process);

	printAll(process) ;
    
    cout << endl;
	cout <<"Average Waiting time = "<< avg_WTime(process) <<endl;


    return 0;
}


////////////////// SJF ////////////////////
class SJF
{
public:
    string name;
    int arrival ,burst , departure ,timeLeft ,finished;

	SJF()
	{
		arrival =0;
		burst = 0;
		departure = 0;
		timeLeft = 0;
		finished = 0;
	}

	SJF( int a , int b)
		{
			arrival = a;
			burst = b;
		}

	double waiting_time()
	{
		return (double)(departure - arrival - burst) ;
	}

};


void sjf_calc (vector<SJF>& x , int p)
{
	int time =0 ,count =0 ,check =0;
	SJF* Current_process;
	Current_process = &x[0];
	while((*Current_process).arrival > time){time++;}
	cout << (*Current_process).name <<"("<< time << ")";
	while(count < sizee)
	{
		count = 0;
		check =0 ;

		//Non-Preemptive
		if(p == 0)
		{
			for(int j=0; j< sizee ; j++)
			{
				if(x[j].arrival<= time &&  (*Current_process).finished ==1)
				{

					if(x[j].finished !=1)
					{Current_process = &x[j];}

					for(int l=j ; l < sizee ; l++)
					{
						if(x[l].arrival<= time && x[l].timeLeft < (*Current_process).timeLeft)
							if(x[l].finished !=1)
							{Current_process = &x[l];}
					}
					
					check++;

				}
				if(x[j].finished ==1)
					count ++;

			}

		}

		//Preemptive

		if(p==1)
		{
			for(int j=0; j< sizee ; j++)
			{
				if(x[j].arrival<= time && x[j].timeLeft < (*Current_process).timeLeft)
				{
					Current_process = &x[j];
					check ++;
					
				}
				if(x[j].finished ==1)
					count ++;

			}

		}


		if((*Current_process).finished == 1) 
		{time++; continue;}

		if(check >0) cout << (*Current_process).name <<"("<< time << ")"; //printing gauntt chart

		if((*Current_process).timeLeft ==0  )
		{
			
			(*Current_process).finished = 1;
			(*Current_process).timeLeft = 999999999;
			(*Current_process).departure = time ;
			time--;
			
		}

		else
		{
			(*Current_process).timeLeft--;
			cout << "*" ;
		}
		time++;
		
	}
		cout <<"("<< time-1 << ")";
	cout<<endl<<endl;
}

bool sortSJFByArrival(const SJF &lhs, const SJF &rhs) { return lhs.arrival < rhs.arrival; }

int SJFmain()
{
	ifstream ip ("data.csv");
    int i=0 ,inputType , preemptive;
	string a,b;


    cout<<"Enter no. of processes\n";
    cin>>sizee ;
	cout<<"Enter 1 for input file OR enter 0 for CMD \n";
	cin>>inputType;
	cout<<"Enter 1 for preemptive OR enter 0 for Nonpreemptive \n";
	cin>>preemptive;

	vector<SJF> Sprocess(sizee);
    cout<<endl ;

	if (inputType==1)
	{
		while(ip.good()&& i<sizee)
		{
			getline(ip,Sprocess[i].name,',');
			getline(ip,a,',');
			getline(ip,b,'\n');
			Sprocess[i].arrival = stoi(a);
			Sprocess[i].burst = stoi(b) ;
			Sprocess[i].timeLeft = Sprocess[i].burst ;
			i++;
		}
	}
	if (inputType==0)
	{

		for (vector<SJF>::size_type i = 0; i != sizee; ++i)
		{
			cout << "Process #" << i + 1 << " name: ";
			cin >> Sprocess[i].name;

			cout << "Process #" << i + 1 << " arrival time: ";
			cin >> Sprocess[i].arrival;

			cout << "Process #" << i + 1 << " burst time ";
			cin >> Sprocess[i].burst;
			Sprocess[i].timeLeft = Sprocess[i].burst ;
	
		}
	}
    cout << "\n";


    // Sort by Arrival
    sort(Sprocess.begin(), Sprocess.end(), sortSJFByArrival);


	sjf_calc (Sprocess , preemptive);

	printAll(Sprocess) ;
    
    cout << endl;
	cout <<"Average Waiting time = "<< avg_WTime(Sprocess) <<endl;


    return 0;
}

/////////////////PRIORTY ////////////////////

class Priority
{
public:
    string name;
    int arrival ,burst , departure ,timeLeft ,finished, prior ,priorityCheck;

	Priority()
	{
		arrival =0;
		burst = 0;
		departure = 0;
		timeLeft = 0;
		finished = 0;
		prior = 0 ;
	}

	Priority( int a , int b , int p)
		{
			arrival = a;
			burst = b;
			prior = p;
		}

	double waiting_time()
	{
		return (double)(departure - arrival - burst) ;
	}

};

void Priority_calc (vector<Priority>& x , int p)
{
	int time =0 ,count =0 ,check =0;
	Priority* Current_process;
	Current_process = &x[0];
	while((*Current_process).arrival > time){time++;}
	cout << (*Current_process).name <<"("<< time << ")";
	while(count < sizee)
	{
		count = 0;
		check =0 ;
		if(p == 0)
		{
			for(int j=0; j< sizee ; j++)
			{
				if(x[j].arrival<= time &&  (*Current_process).finished ==1 && x[j].finished !=1)
				{
					Current_process = &x[j];
					check++;
					for (int k=0 ; k<sizee ; k++)
					{
						if(x[k].arrival<= time &&  x[k].priorityCheck < (*Current_process).priorityCheck)
						{
							Current_process = &x[k];
							check++;
						}

					}
				}
				if(x[j].finished ==1)
					count ++;
			}
		}
		if(p==1)
		{
			for(int j=0; j< sizee ; j++)
			{
				if(x[j].arrival<= time && x[j].priorityCheck < (*Current_process).priorityCheck )
				{
					Current_process = &x[j];
					check++;
				}
				if(x[j].finished ==1)
					count ++;

			}

			
		}
		if(check>0) cout << (*Current_process).name <<"("<< time << ")";


		if((*Current_process).finished == 1)
		{time++; continue;}

		if((*Current_process).timeLeft ==0  )
		{
			
			(*Current_process).finished = 1;
			(*Current_process).timeLeft = 999999999;
			(*Current_process).priorityCheck = 999999999;
			(*Current_process).departure = time ;
			time--;
		}

		else
		{
			(*Current_process).timeLeft--;
			cout << "*";
		}
		
		time++;
	}
	
	cout <<"("<< time-1 << ")" ;
	cout<<endl<<endl;
}

void PriorPrintAll(vector<Priority>& x)
{
	cout<< endl <<endl;
	cout <<"name \t"<< "Arrival\t" <<"Burst\t" <<"Departure\t"<<"Priority"<<endl ;
	for (int j=0 ; j<sizee ;j++)
		cout <<x[j].name<<"\t "<< x[j].arrival << "\t " <<x[j].burst <<"\t " <<x[j].departure <<"\t\t " <<x[j].prior <<endl ;

}

bool sortPriorByArrival(const Priority &lhs, const Priority &rhs) { return lhs.arrival < rhs.arrival; }

int Prioritymain()
{
	ifstream ip ("data.csv");
    int i=0 ,inputType , preemptive;
	string a,b,c;


    cout<<"Enter no. of processes\n";
    cin>>sizee ;
	cout<<"Enter 1 for input file OR enter 0 for CMD \n";
	cin>>inputType;
	cout<<"Enter 1 for preemptive OR enter 0 for Nonpreemptive \n";
	cin>>preemptive;

	vector<Priority> Sprocess(sizee);
    cout<<endl ;

	if (inputType==1)
	{
		while(ip.good() && i<sizee)
		{
			getline(ip,Sprocess[i].name,',');
			getline(ip,a,',');
			getline(ip,b,',');
			getline(ip,c,'\n');
			Sprocess[i].arrival = stoi(a);
			Sprocess[i].burst = stoi(b) ;
			Sprocess[i].prior = stoi(c) ;
			Sprocess[i].timeLeft = Sprocess[i].burst ;
			Sprocess[i].priorityCheck = Sprocess[i].prior ;
			i++;
		}
	}
	if (inputType==0)
	{

		for (vector<Priority>::size_type i = 0; i != sizee; ++i)
		{
			cout << "Process #" << i + 1 << " name: ";
			cin >> Sprocess[i].name;

			cout << "Process #" << i + 1 << " arrival time: ";
			cin >> Sprocess[i].arrival;

			cout << "Process #" << i + 1 << " burst time ";
			cin >> Sprocess[i].burst;
			Sprocess[i].timeLeft = Sprocess[i].burst ;

			cout << "Process #" << i + 1 << " Priority ";
			cin >> Sprocess[i].prior;
			Sprocess[i].priorityCheck = Sprocess[i].prior ;
	
		}
	}
    cout << "\n";


    // Sort by Arrival
    sort(Sprocess.begin(), Sprocess.end(), sortPriorByArrival);

	Priority_calc (Sprocess , preemptive);

	PriorPrintAll(Sprocess) ;
    
    cout << endl;
	cout <<"Average Waiting time = "<< avg_WTime(Sprocess) <<endl;


    return 0;
}


/////////////////// Round Ropin //////////////

class RR
{
public:
    string name;
    int arrival ,burst , departure ,timeLeft ,finished;

	RR()
	{
		arrival =0;
		burst = 0;
		departure = 0;
		timeLeft = 0;
		finished = 0;
	}

	RR( int a , int b)
		{
			arrival = a;
			burst = b;
		}

	double waiting_time()
	{
		return (double)(departure - arrival - burst) ;
	}

};

void RR_calc (vector<RR>& x , int q )
{
	int time =0 ,count =0 ,n=0 ,check =q , d = 0;
	RR* Current_process , *prevProcess = NULL;
	Current_process = &x[0];
	while((*Current_process).arrival > time){time++;}
	while(count < sizee)
	{
		check = q;
		count = 0;
		
		
			for(int j=0; j< sizee ; j++)
			{	
				if(x[j].finished ==1)
					count ++;
			}

		   if (x[(n+1)%sizee].arrival<= time && x[(n+1)%sizee].finished != 1 )
				{
					Current_process = &x[(n+1)%sizee];
					n++;
					if(prevProcess!= Current_process)
						cout << (*Current_process).name <<"("<< time << ")";
				}
		   else 
		   {n++; 
		   if (d<=sizee) 
		   {d++; continue;}
		   }
		d = 0;
		prevProcess = Current_process ;
		if((*Current_process).finished == 1)
		{time++; continue;}

		
		else
		{
			(*Current_process).timeLeft -=q;
			while((*Current_process).timeLeft < 0)
			{
				check --;
				time-- ;
				(*Current_process).timeLeft++ ;
			}
			for(int l=0 ; l<check ; l++)
			{cout << "*";}
		}
		time+=q;


		if((*Current_process).timeLeft == 0  )
		{
			
			(*Current_process).finished = 1;
			(*Current_process).departure = time ;
		}

	}
	cout << "("<< time << ")";
	cout <<endl<<endl;
}

bool sortRRByArrival(const RR &lhs, const RR &rhs) { return lhs.arrival < rhs.arrival; }

int RRmain()
{
	ifstream ip ("data.csv");
    int i=0 ,inputType , QTime;
	string a,b;


    cout<<"Enter no. of processes\n";
    cin>>sizee ;
	cout<<"Enter 1 for input file OR enter 0 for CMD \n";
	cin>>inputType;
	cout<<"Enter Time Quantum  \n";
	cin>>QTime;

	vector<RR> Sprocess(sizee);
    cout<<endl ;

	if (inputType==1)
	{
		while(ip.good() && i<sizee)
		{
			getline(ip,Sprocess[i].name,',');
			getline(ip,a,',');
			getline(ip,b,'\n');
			Sprocess[i].arrival = stoi(a);
			Sprocess[i].burst = stoi(b) ;
			Sprocess[i].timeLeft = Sprocess[i].burst ;
			i++;
		}
	}
	if (inputType==0)
	{

		for (vector<RR>::size_type i = 0; i != sizee; ++i)
		{
			cout << "Process #" << i + 1 << " name: ";
			cin >> Sprocess[i].name;

			cout << "Process #" << i + 1 << " arrival time: ";
			cin >> Sprocess[i].arrival;

			cout << "Process #" << i + 1 << " burst time ";
			cin >> Sprocess[i].burst;
			Sprocess[i].timeLeft = Sprocess[i].burst ;

	
		}
	}
    cout << "\n";


    // Sort by Arrival
    sort(Sprocess.begin(), Sprocess.end(), sortRRByArrival);

	RR_calc (Sprocess , QTime );

	printAll(Sprocess) ;
    
    cout << endl;
	cout <<"Average Waiting time = "<< avg_WTime(Sprocess) <<endl;


    return 0;
}

//////////////////////// Main ////////////////////////////////

int main()
{

	int x,y;
do {
	cout << "Enter \n   0  for (First Come First Served) \n   1  for (Shortest Job First) \n   2  for (Prority) \n   3  for (Round Robin) \n\n";
	cout <<" Type of scheduler =  "; cin >>x;
	cout<< endl;


	if ( x==0 ){ 
		cout<<"\n/////// First Come First served \\\\\\\\\\\\\\ \n\n"; 
		FCFSmain();}
	if ( x==1 ){ 
		cout<<"\n/////// Shortest Job First \\\\\\\\\\\\\\ \n\n"; 
		SJFmain();}
	if ( x==2 ){ 
		cout<<"\n/////// Priority \\\\\\\\\\\\\\ \n\n"; 
		Prioritymain();}
	if ( x==3 ){ 
		cout<<"\n/////// Round Robin \\\\\\\\\\\\\\ \n\n"; 
		RRmain();}


	cout << "\n////////////////////////DONE//////////////////////////\n \nTo try again press 1 \n";
	cin>>y;
	cout<< endl <<endl;
} 
while (y == 1);

	return 0;
	
}



