#include <sys/types.h>
#include <bits/stdc++.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define el "\n"
#define sp " "
#define ll long long 
using namespace std;

int main(int argc,char *argv[])
{
	int shmid;
	char *shmem;

	//Initialization of the shared memory - Unique already generated in server 
	if ((shmid = shmget((key_t)1080, 27, 0666)) < 0) 
	{
		cout<<"\nError in shmget";
		exit(1);
	}

	//Attachment to the shared memory
	if ((shmem = (char *)shmat(shmid, NULL, 0)) == (char *) -1) {
		cout<<"\nError in shmat";
		exit(1);
	}

	//Exchange of messages
	*(shmem)= '!';
	char s=argv[1][0];
	int i;
	while(*(shmem)!='*')
	{		
		//switching of program
		while(*(shmem)!='^'&&*(shmem)!='*')
			sleep(1);
		if(*(shmem)=='*')
		{
			cout<<"Connection closed by Server";
			break;
		}
		else
		{
			cout<<"\nServer: ";
			for(i=0;*(shmem+i+1)!='\0';i++)
				cout<<*(shmem+i+1);
			cout<<el;
			string a;
			cout<<"\nClient: ";
			getline(cin,a);
			if(a[0]=='*')
			{
				*(shmem)='*';
				continue;
			}
			for(i=0;i<a.size();i++)
				*(shmem+i+1)=a[i];
			*(shmem+i+1)='\0';
			*(shmem)=s;
		}
		
	}
	cout<<endl<<"If you want to see your chat history type 1 else type 0"<<endl;
	int chec=0;
	cin>>chec;
	if(chec)
	{
		cout<<"Chat history:"<<endl;
		fstream fptr;
		string x;
		x.push_back(s);				
		x=x+".txt";
		char file_name[100];
		for(i=0;i<x.size();i++)
			file_name[i]=x[i];
		file_name[i]='\0';
		fptr.open(file_name);
		while( getline(fptr,x))
			cout<<x<<endl;	
		fptr.close();
	}

	//Deattach the shared memory
	shmid = shmdt(shmem);
	exit(0);
}
