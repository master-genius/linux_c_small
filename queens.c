#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>

#define Q_MIN	6
#define Q_MAX	32
#define Q_DEFT	8

int ques(int n);
int upd(int**,int y, int x, int n);
int back(int**, int y, int x, int n);
int calt(int**, int);

void success(int**qe, int n);
void destroy(int**,int n);

struct qe_pos
{
	int q_y;
	int q_x;
};

int main(int args, char* argv[])
{
	int n=0;
    if(args < 2){
        return ques(8);
    }

    int i=1,tmp = 0,ret;
    while(i<args){
        tmp = atoi(argv[i]);
        ret = (tmp<Q_MIN || tmp>Q_MAX)?printf("Error: %d is a illegal number\n",tmp):ques(tmp);
        i+=1;
        printf("\n");
    }

    return 0;
}

int ques(int n)
{	
	int** qe=(int**)malloc(sizeof(int*)*n);
	if(!qe)
	{
		perror("malloc:");
		return -1;
	}
	for(int i=0;i<n;i++)
	{
		qe[i]=(int*)malloc(sizeof(int)*n);
		if(!qe[i])
		{
			for(int j=i-1;j>=0;j--)
				free(qe[j]);
			free(qe);
			perror("malloc");
			return -1;
		}
	}
	
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
			qe[i][j]=0;
	
	if(calt(qe,n)==-1)
	{
		destroy(qe,n);
		return -1;
	}
	
	destroy(qe,n);

    return 0;
}

int calt(int**qe, int n)
{
	if(!qe || n<=0)
		return -1;
	
	struct qe_pos qp[n];
	
	int k=0;
	
	qp[0].q_y=0;
	qp[0].q_x=0;
	
	int flag=0;
	
	while(k<n)
	{
		for(int y=qp[k].q_y;y<n;y++)
			for(int x=qp[k].q_x;x<n;x++)
			{
				if(qe[y][x]==0)
				{
					upd(qe,y,x,n);
					qp[k].q_y=y;
					qp[k].q_x=x;
					flag=1;
					goto next;
				}
			}
		next:;
		
		if(flag==1)
		{
			k++;
			if(k>=n)
				break;
			qp[k].q_y=qp[k-1].q_y+1;
			qp[k].q_x=0;
			flag=0;
		}
		else
		{
			k--;
			if(k<0)
				return -1;
			back(qe, qp[k].q_y, qp[k].q_x, n);
			
			if(qp[k].q_x<n-1)
				qp[k].q_x++;
			else
			{
				qp[k].q_y++;
				qp[k].q_x=0;
			}
		}
		
	}
	success(qe,n);

    return 0;
}

int back(int**qe, int y, int x, int n)
{
	if(!qe || y<0 || x<0 || n<=0)
		return -1;

	qe[y][x]=0;

	for(int i=0;i<n;i++)
	{
		if(qe[y][i]>0)
			qe[y][i]--;
		if(qe[i][x]>0)
			qe[i][x]--;
	}
	
	for(int i=y,j=x;i>=0 && j>=0;i--,j--)
		if(qe[i][j]>0)
			qe[i][j]--;

	for(int i=y,j=x;i>=0 && j<n;i--,j++)
		if(qe[i][j]>0)
			qe[i][j]--;

	for(int i=y, j=x;i<n && j>=0;i++,j--)
		if(qe[i][j]>0)
			qe[i][j]--;
	for(int i=y, j=x; i<n && j<n;i++,j++)
		if(qe[i][j]>0)
			qe[i][j]--;

    return 0;
}

int upd(int**qe, int y, int x, int n)
{
	if(!qe || y<0 || x<0 || n<=0)
		return -1;

	qe[y][x]=-1;

	for(int i=0;i<n;i++)
	{
		if(qe[y][i]>=0)
			qe[y][i]++;
		if(qe[i][x]>=0)
			qe[i][x]++;
	}
	
	for(int i=y,j=x;i>=0 && j>=0;i--,j--)
		if(qe[i][j]>=0)
			qe[i][j]++;

	for(int i=y,j=x;i>=0 && j<n;i--,j++)
		if(qe[i][j]>=0)
			qe[i][j]++;

	for(int i=y, j=x;i<n && j>=0;i++,j--)
		if(qe[i][j]>=0)
			qe[i][j]++;

	for(int i=y, j=x; i<n && j<n;i++,j++)
		if(qe[i][j]>=0)
			qe[i][j]++;

    return 0;
}

void success(int**qe, int n)
{
	if(!qe || n<=0) return ;
	
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
			if(qe[i][j]==-1)
				printf("|Q");
			else
				printf("| ");
		printf("|\n");
	}
}

void destroy(int**qe,int n)
{
	if(!qe) return ;
	
	for(int i=0;i<n;i++)
		free(qe[i]);
	free(qe);
}
