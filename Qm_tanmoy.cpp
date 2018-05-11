#include<bits/stdc++.h>
using namespace std;

map<int,string> terms;
map<set<int>,string> single_terms,dual_terms,quad_terms,octal_terms,hexa_terms,implicants;
map<char,vector<int>> pichart;
set<char> EPI,DominantRow,Identicals,Finalterms;

string convert(int n,int noofvariable)
{
    int num=n;

    string str="        ";
    for(int i=noofvariable-1;i>=0;i--)
    {
        if(num>=pow(2,i))
        {
            str[4-i]='1';
            num=num-pow(2,i);
        }
        else
            str[4-i]='0';
    }
    return str;
}

void create()
{
    int n,noofvariable;
    map<int,string>::iterator iter;
    cout<<"Enter number of variables (upto 6):";
    cin>>noofvariable;
    cout<<"Enter minterms (upto "<<pow(2,noofvariable)-1<<"):";
    while(1)
    {
        scanf("%d",&n);
        if(n>=pow(2,noofvariable))
        break;
        terms.insert(pair<int,string>(n,convert(n,noofvariable)));
    }
    iter=terms.begin();
    cout<<"TERMS :"<<endl;
    while(iter!=terms.end())
    {
        cout<<endl<<iter->first<<'\t'<<iter->second;
        iter++;
    }
    map<int,string>::iterator it;
    for(it=terms.begin();it!=terms.end();it++)
    {
        string str;
        set<int> arr;
        arr.insert(it->first);
        str=it->second;
        str[7]='X';
        single_terms.insert(pair<set<int>,string>(arr,str));
    }
    return;
}

void grouping(map<set<int>,string> *OLD ,map<set<int>,string> *NEW ,string name )
{
    map<set<int>,string>::iterator i,j;
    for(i=(*OLD).begin();i!=(*OLD).end();i++)
    {
        j=i;
        j++;
        for(;j!=(*OLD).end();j++)
        {   set<int> arr;
            string str;
            int counter=0,pos=-1;
            for(int k=0;k<5;k++)
                if((i->second[k])!=(j->second[k]))
                {
                    counter++;
                    pos=k;
                }
            if(counter==1)
            {
                set<int>::iterator z;
                for(z=(i->first).begin();z!=(i->first).end();z++)
                {
                    arr.insert(*z);
                }
                for(z=(j->first).begin();z!=(j->first).end();z++)
                {
                    arr.insert(*z);
                }
                str=(i->second);
                str[pos]='_';
                str[7]='X';
                (*NEW).insert(pair<set<int>,string>(arr,str));
                i->second[7]='T';
                j->second[7]='T';
            }
        }
    }

    map<set<int>,string>::iterator iter=(*NEW).begin();
    cout<<"\n"<<name<<" :"<<endl;
    while(iter!=(*NEW).end())
    {
        set<int>::iterator z;
        for(z=(iter->first).begin();z!=(iter->first).end();z++)
        {
            cout<<(*z)<<' ';
        }
        cout<<'\t'<<iter->second<<endl;;
        iter++;
    }
    iter=(*OLD).begin();
    //MODIFYING THE IMPLICANT MAP
    while(iter!=(*OLD).end())
    {
        if(iter->second[7]=='X')
            implicants.insert(pair<set<int>,string>(iter->first,iter->second));
            iter++;
    }
    return;
}

void nameImplicants()
{
    map<set<int>,string>::iterator iter=implicants.begin();
    char ch='A';
    while(iter!=implicants.end())
    {
        iter->second[7]=ch++;
        iter++;
    }
    return;
}

void printImplicants()
{
    map<set<int>,string>::iterator iter=implicants.begin();
    cout<<"\nIMPLICANTS :"<<endl;
    while(iter!=implicants.end())
    {
        set<int>::iterator z;
        for(z=(iter->first).begin();z!=(iter->first).end();z++)
        {
            cout<<(*z)<<' ';
        }
        cout<<'\t'<<iter->second<<endl;
        iter++;
    }
    return;
}

void PIChart()
{
    vector<int> T;
    map<int,string>::iterator it;
    map<set<int>,string>::iterator iter;
    for(it=terms.begin();it!=terms.end();it++)
    {
        T.push_back(it->first);
    }
    pichart.insert(pair<char,vector<int>>('/',T));
    for(iter=implicants.begin();iter!=implicants.end();iter++)
    {
        vector<int> arr;
        vector<int>::iterator i;
        for(i=T.begin();i!=T.end();i++)
        {
            set<int>::iterator z;
            z=(iter->first).find(*i);
            if(z==(iter->first).end())
                arr.push_back(0);
            else
                arr.push_back(1);
        }
        pichart.insert(pair<char,vector<int>>(iter->second[7],arr));
    }
    return;
}

void printPIChart()
{
    if(pichart['/'].empty())
    {
        cout<<endl<<"\t\tPI CHART EMPTY"<<endl;
        return;
    }
    map<char,vector<int>>::iterator iter;
    cout<<endl<<"\t\tPI-CHART"<<endl;
    for(iter=pichart.begin();iter!=pichart.end();iter++)
    {
        vector<int>::iterator z;
        cout<<iter->first<<" ";
        for(z=(iter->second).begin();z!=(iter->second).end();z++)
            cout<<setw(3)<<(*z)<<' ';
        cout<<endl;
    }
    return;
}

void essentialPI()
{
    int col=terms.size();
    for(int i=0;i<col;i++)
    {
        map<char,vector<int>>::iterator iter=pichart.begin();
        iter++;
        int counter=0;
        char pos=' ';
        while(iter!=pichart.end())
        {
            if((iter->second)[i]==1)
            {
                pos=iter->first;
                counter++;
            }
            if(counter>1)
                break;

            iter++;
        }
        if(counter==1)
        {
           EPI.insert(pos);
        }
    }
    set<char>::iterator z;
    cout<<"\nEssential Prime Implicants :";
    for(z=EPI.begin();z!=EPI.end();z++)
        cout<<(*z)<<' ';
    cout<<endl;
    return;

}

void modifyingPIchart(set<char> *p)
{
    set<char>::iterator z;
    int col=pichart['/'].size();
    map<char,vector<int>> temp=pichart;
    for(z=(*p).begin();z!=(*p).end();z++)
    {
        int del=0;
        for(int i=0,k=0;i<col;i++,k++)
        {
            if((pichart[*z])[i]==1)
            {
                map<char,vector<int>>::iterator it;
                for(it=temp.begin();it!=temp.end();it++)
                {
                     //Erasing corresponding column of Essential Prime Implicant
                    (it->second).erase((it->second).begin() + k);
                }
                k--;
                del++;
            }
        }
        pichart=temp;
        col=col-del; //resizing the table
    }
    for(z=(*p).begin();z!=(*p).end();z++)
    {
        pichart.erase((*z));
    }
}

void rowdominance()
{
    int flag;
    do
    {
        flag=0;
        char tobedeleted;
        set<char> temp;
        temp.clear();
        map<char,vector<int>>::iterator i=pichart.begin(),j;
        i++;
        while(i!=pichart.end())
        {
            j=i;j++;
            while(j!=pichart.end())
            {
                int c1=0,c2=0;
                int col=pichart['/'].size();
                for(int k=0;k<col;k++)
                {
                    if((i->second)[k]==1 && (j->second)[k]==0)
                        c1++;
                    else if((i->second)[k]==0 && (j->second)[k]==1)
                        c2++;
                }
                if(c1>0 && c2==0)
                {
                    temp.insert(i->first);
                    tobedeleted=j->first;
                    flag=1;
                }
                if(c1==0 && c2>0)
                {
                    tobedeleted=i->first;
                    temp.insert(j->first);
                    flag=1;
                }
                if(flag==1)
                    break;
                j++;
            }
            if(flag==1)
                break;
            i++;
        }
        pichart.erase(tobedeleted);
        modifyingPIchart(&temp);
        DominantRow.insert(temp.begin(), temp.end());
        printPIChart();
    }while(flag==1);
   return;
}

void identicals()
{
    if(pichart.size()<=1)
        return;
    map<char,vector<int>>::iterator i=pichart.begin(),j;
    i++;
    while(i!=pichart.end())
    {
        j=i;j++;
        while(j!=pichart.end())
        {
            if(i->second==j->second)
            {
                    Identicals.insert(i->first);
            }
            j++;
        }
        i++;
    }
}

void Finalexpression()
{
	set<char>::iterator z;
    for(z=Finalterms.begin();z!=Finalterms.end();z++)
    {
        cout<<*z;
    }
}

int main()
{
    create();
    grouping(&single_terms,&dual_terms,"DUAL TERMS");
    grouping(&dual_terms,&quad_terms,"QUAD TERMS");
    grouping(&quad_terms,&octal_terms,"OCTAL TERMS");
    grouping(&octal_terms,&hexa_terms,"HEXA TERMS");
    nameImplicants();
    printImplicants();
    PIChart();
    printPIChart();
    essentialPI();
    modifyingPIchart(&EPI);
    printPIChart();
    rowdominance();
    identicals();
    cout<<"MINIMISED TERMS ARE :";
    set<char>::iterator z;
    for(z=EPI.begin();z!=EPI.end();z++)
        Finalterms.insert(*z);
    for(z=DominantRow.begin();z!=DominantRow.end();z++)
        Finalterms.insert(*z);
    for(z=Identicals.begin();z!=Identicals.end();z++)
        Finalterms.insert(*z);
    Finalexpression();
    return 0;
}


