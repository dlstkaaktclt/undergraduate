import java.util.ArrayList;
import java.util.StringTokenizer;

import java.io.FileReader;
import java.io.BufferedReader;
import java.io.PrintWriter;

/*
   1. 아래와 같은 명령어를 입력하면 컴파일이 이루어져야 하며, Solution2 라는 이름의 클래스가 생성되어야 채점이 이루어집니다.
       javac Solution2.java -encoding UTF8

   2. 컴파일 후 아래와 같은 명령어를 입력했을 때 여러분의 프로그램이 정상적으로 출력파일 output2.txt 를 생성시켜야 채점이 이루어집니다.
       java Solution2

   - 제출하시는 소스코드의 인코딩이 UTF8 이어야 함에 유의 바랍니다.
   - 수행시간 측정을 위해 다음과 같이 time 명령어를 사용할 수 있습니다.
       time java Solution2
   - 일정 시간 초과시 프로그램을 강제 종료 시키기 위해 다음과 같이 timeout 명령어를 사용할 수 있습니다.
       timeout 0.5 java Solution2   // 0.5초 수행
       timeout 1 java Solution2     // 1초 수행
 */

class Solution2 {
	static final int MAX_N = 20000;
	static final int MAX_E = 80000;

	static int N, E;
	static int[] U = new int[MAX_E], V = new int[MAX_E], W = new int[MAX_E];
	static int Answer;
	
	
	static class DisjointSet		// disjointset을 이용하기 위한 클래스. rank와 pass compression을 구현.
	{
		private int num;
		private DisjointSet parent;
		private int rank;
		
		public DisjointSet(int num)
		{
			this.num = num;
			parent = this;
			this.rank = 0;
		}
		
		public DisjointSet find(DisjointSet other)
		{
			if(other.getParent() == other)
			{
				return other;
			}
			else
			{
				DisjointSet root = find(other.getParent());
				other.setParent(root);
				return root;
			}
		}
		
		public void union(DisjointSet one,DisjointSet two)
		{
			DisjointSet fst = find(one);
			DisjointSet snd = find(two);
			
			if(fst.getNum() != snd.getNum())
			{
				int fstrank = fst.getRank();
				int sndrank = snd.getRank();
				
				if(fstrank>sndrank)
				{
					snd.setParent(fst);
				}
				else if(fstrank<sndrank)
				{
					fst.setParent(snd);
				}
				else
				{
					snd.setParent(fst);
					fst.rankUp();
				}
			}
		}
		
		public int getNum()
		{
			return this.num;
		}
		
		public void setParent(DisjointSet p)
		{
			this.parent = p;
		}
		
		public DisjointSet getParent()
		{
			return this.parent;
		}
		
		public int getRank()
		{
			return this.rank;
		}
		
		public void rankUp()
		{
			this.rank++;
		}
		
		public boolean isSameset(DisjointSet one,DisjointSet two)
		{
			DisjointSet fst = find(one);
			DisjointSet snd = find(two);
			
			if(fst.getNum()==snd.getNum()) return true;
			else return false;
		}
		
	}
	
	public static void swap(int a,int b)		// 두 원소의 자리를 바꾼다.
	{
		int tempU,tempV,tempW;
		tempU = U[a];
		U[a] = U[b];
		U[b] = tempU;
		tempV = V[a];
		V[a] = V[b];
		V[b] = tempV;
		tempW = W[a];
		W[a] = W[b];
		W[b] = tempW;
	}
	
	
	public static int extractMax(int arr[],int heapsize) // 최대원소가 있는곳의 인덱스를 리턴. 힙 크기를 n이라 하면 힙을 수선하는데 theta(logn)시간이 걸린다.
	{
		if(heapsize>=1)
		{
			swap(0,heapsize-1);
			percolateDownMaxheap(0,arr,heapsize-1);
		}
		return heapsize-1;
	}
	
	public static void percolateDownMaxheap(int now,int arr[],int n) //version index 0,1,2....n-1.   n means heapsize.
	{
		int leftchild = 2*now + 1;
		int rightchild = 2*now + 2;
		
		if(leftchild>(n-1))
		{
			return;
		}
		else if(leftchild==(n-1))
		{
			if(arr[leftchild]>arr[now])
			{
				swap(leftchild,now);
			}
		}
		else
		{
			int bigger = arr[leftchild]>arr[rightchild] ? leftchild : rightchild;
			if(arr[bigger]>arr[now])
			{
				swap(now,bigger);
				percolateDownMaxheap(bigger,arr,n);
			}
		}
	}
	//percolatedown을 할때 힙의 높이 h에 비례하는 시간이 걸리는데, 힙의 높이는 h 는 힙의 크기를 n이라 할 때 logn에 비례하므로
	// theta(logn)의 시간이 걸린다.
	
	public static void buildMaxheap(int arr[],int n)	//n is size of heap. ex) arr[0..3] -> n is 4!
	{
		for(int i=(n/2);i>=0;i--)
		{
			percolateDownMaxheap(i,arr,n);
		}
	}
	// size n짜리 힙을 만드는데, percolateDown에 힙 원소 개수가 k라 하면 logk의 시간이 걸리므로,
	// 힙 전체를 만드는데 드는 시간은 (0 * n/2) + (1 * n/4) + (2 * n/8) ... (h * 1) 이다. h는 전체 힙의 높이다. (h = (log(n+1)의 내림값)+ 1)
	// 이를 다 더한값이 무한등비급수를 이용해 n보다 작음을 보일 수 있고, n/16보다 큼을 보일 수 있으므로 theta(n)의 시간만에 힙을 만들 수 있다.

	public static void main(String[] args) throws Exception {
		/*
		   동일 폴더 내의 input2.txt 로부터 데이터를 읽어옵니다.
		   또한 동일 폴더 내의 output2.txt 로 정답을 출력합니다.
		 */
		BufferedReader br = new BufferedReader(new FileReader("input2.txt"));
		StringTokenizer stk;
		PrintWriter pw = new PrintWriter("output2.txt");
		/*
		   10개의 테스트 케이스가 주어지므로, 각각을 처리합니다.
		 */
		for (int test_case = 1; test_case <= 10; test_case++) {
			/*
			   각 테스트 케이스를 표준 입력에서 읽어옵니다.
			   먼저 정점의 개수와 간선의 개수를 각각 N, E에 읽어들입니다.
			   그리고 각 i번째 간선의 양 끝점의 번호를 U[i], V[i]에 읽어들이고, i번째 간선의 가중치를 W[i]에 읽어들입니다. (0 ≤ i ≤ E-1, 1 ≤ U[i] ≤ N, 1 ≤ V[i] ≤ N)
			 */
			stk = new StringTokenizer(br.readLine());
			N = Integer.parseInt(stk.nextToken()); E = Integer.parseInt(stk.nextToken());
			
			
			stk = new StringTokenizer(br.readLine());
			for (int i = 0; i < E; i++) {
				U[i] = Integer.parseInt(stk.nextToken());
				V[i] = Integer.parseInt(stk.nextToken());
				W[i] = Integer.parseInt(stk.nextToken());
			} // 초기화를 수행하는데 theta(E)의 시간이 걸린다.


			/////////////////////////////////////////////////////////////////////////////////////////////
			/*
			   이 부분에서 여러분의 알고리즘이 수행됩니다.
			   문제의 답을 계산하여 그 값을 Answer에 저장하는 것을 가정하였습니다.
			 */
			/////////////////////////////////////////////////////////////////////////////////////////////
			// 크루스칼 알고리즘을 개량한 버전을 이용해서 maximal spanning tree를 구한다.
			
			DisjointSet sets[] = new DisjointSet[N+1];		// disjointset을 위한 클래스. tree로 구현했으며, rank와 path compression이 구현됨.
			
			for(int i=1;i<=N;i++)			// 각 vertex에 대해 disjointset을 만들어준다.
			{
				sets[i] = new DisjointSet(i);
			}			//disjointset을 처음 생성할때 theta(1)의 시간이 걸리므로 theta(V)의 시간이 걸림.
			
			Answer = 0;
			int addededge=0;
			
			buildMaxheap(W,E);				// edge들을 maxheap으로 만들어준다. theta(E)의 시간이 걸림. -> maxheap 함수 정의는 위에..
			for(int i=E;i>0;i--)
			{
				int maxedgeindex = extractMax(W,i);			//max heap에서 weight이 가장큰 edge를 가져온다. 힙 수선에 theta(logE)의 시간이 걸림.
				DisjointSet one = sets[U[maxedgeindex]];
				DisjointSet two = sets[V[maxedgeindex]];
				if(!one.isSameset(one, two))		
				{
					one.union(one, two);
					Answer+=W[maxedgeindex];
					addededge++;
					if(addededge==N-1) break;
				}
			}	// Disjointset 연산을 하는데 V번의 make-set과, 최악의 경우 E번의 isSameset과 Union 연산을 하고, isSameset은 두번의 find-set연산으로 이루어져 있으므로,
			    //             수행시간은 총 O((V+3E)log*V) 에서 log*V는 상수와 같으므로 disjointset 연산에서의 수행시간은 O(V+3E) = O(V+E)
			    //             또한 루프를 돌면서 힙 수선이 최대 E번 일어날 수 있으므로 theta(ElogE)에서  전체 수행시간은 theta(V + ElogE) = theta(V + ElogV)가 된다.
				//             (simple graph에서 E는 최대 V^2이므로 logE = logV^2 = 2logV 에서..)
				//             일반적으로 E>V이므로 theta(ElogV)라 할 수 있다.



			// output2.txt로 답안을 출력합니다.
			pw.println("#" + test_case + " " + Answer);
			/*
			   아래 코드를 수행하지 않으면 여러분의 프로그램이 제한 시간 초과로 강제 종료 되었을 때,
			   출력한 내용이 실제로 파일에 기록되지 않을 수 있습니다.
			   따라서 안전을 위해 반드시 flush() 를 수행하시기 바랍니다.
			 */
			pw.flush();
		}
		


		br.close();
		pw.close();
	}
}

