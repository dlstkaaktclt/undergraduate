import java.util.StringTokenizer;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Stack;

/*
   1. 아래와 같은 명령어를 입력하면 컴파일이 이루어져야 하며, Solution1 라는 이름의 클래스가 생성되어야 채점이 이루어집니다.
       javac Solution1.java -encoding UTF8

   2. 컴파일 후 아래와 같은 명령어를 입력했을 때 여러분의 프로그램이 정상적으로 출력파일 output1.txt 를 생성시켜야 채점이 이루어집니다.
       java Solution1

   - 제출하시는 소스코드의 인코딩이 UTF8 이어야 함에 유의 바랍니다.
   - 수행시간 측정을 위해 다음과 같이 time 명령어를 사용할 수 있습니다.
       time java Solution1
   - 일정 시간 초과시 프로그램을 강제 종료 시키기 위해 다음과 같이 timeout 명령어를 사용할 수 있습니다.
       timeout 0.5 java Solution1   // 0.5초 수행
       timeout 1 java Solution1     // 1초 수행
 */

class Solution1 {
	static final int MAX_N = 10000;
	static final int MAX_E = 500000;

	static int N, E;
	static int[] U = new int[MAX_E], V = new int[MAX_E], W = new int[MAX_E];
	static int[] Answer = new int[MAX_N];			// 계산의 편의를 위해 최대 인덱스를 하나 늘렸습니다. 
	
	static int[] sortedVertex = new int[MAX_N+1]; // topological sort된 vertex들의 index들을 저장
	static int[] Vertexindex = new int[MAX_N+1]; // 각 vertex들이 topological sort가 어떻게 되었는지를 저장.
	
	static int remain;			// topological sort할때, 몇개의 원소들이 남았는지 저장하는 값.
	
	
	private static void topologicalDFS(int now, boolean[] visited,ArrayList[] adjlist) // DFS를 통해 위상정렬을 실시하는 함수.
	{
		visited[now] = true;
		for(int j=0;j<adjlist[now].size();j++)
		{
			int adjver = V[(int) adjlist[now].get(j)];
			if(visited[adjver] == false)
			{
				topologicalDFS(adjver,visited,adjlist);
			}
		}
		sortedVertex[remain] = now;
		Vertexindex[now] = remain;
		remain--;
	}
	// DFS를 수행하는데 총 Edge와 vertex를 2번, 1번씩 방문하므로 theta(E+V)의 시간이 든다.

	public static void main(String[] args) throws Exception {
		/*
		   동일 폴더 내의 input1.txt 로부터 데이터를 읽어옵니다.
		   또한 동일 폴더 내의 output1.txt 로 정답을 출력합니다.
		 */
		BufferedReader br = new BufferedReader(new FileReader("input1.txt"));
		StringTokenizer stk;
		PrintWriter pw = new PrintWriter("output1.txt");

		/*
		   10개의 테스트 케이스가 주어지므로, 각각을 처리합니다.
		 */
		for (int test_case = 1; test_case <= 10; test_case++) {
			/*
			   각 테스트 케이스를 표준 입력에서 읽어옵니다.
			   먼저 정점의 개수와 간선의 개수를 각각 N, E에 읽어들입니다.
			   그리고 각 i번째 간선의 시작점, 끝점, 가중치를 각각 U[i], V[i], W[i]에 읽어들입니다. (0 ≤ i ≤ E-1, 1 ≤ U[i] ≤ N, 1 ≤ V[i] ≤ N)
			 */
			stk = new StringTokenizer(br.readLine());
			N = Integer.parseInt(stk.nextToken()); E = Integer.parseInt(stk.nextToken());
			
			ArrayList adjlist[] = new ArrayList[N+1];		// 인접리스트 초기화
			for(int i = 1; i <= N; i++)
			{
				adjlist[i] = new ArrayList();
			}
			
			for (int i = 0; i < E; i++) {
				stk = new StringTokenizer(br.readLine());
				U[i] = Integer.parseInt(stk.nextToken());
				V[i] = Integer.parseInt(stk.nextToken());
				W[i] = Integer.parseInt(stk.nextToken());
				
				adjlist[U[i]].add(i);		//adjlist에 i번째 edge가 연결되어있다는것을 표기.
			}
			//인접리스트를 만들고, 입력 전체를 받는데 theta(V+E)의 시간이 듬. (v: vertex 개수, e : edge 개수)
			
			
			boolean visited[] = new boolean[N+1];		//topological sort를 하기 위해 DFS를 하고, 그를 위해 만드는 visited 행렬
			for (int i = 1; i <= N; i++)
			{
				visited[i] = false;
			}		//visited 행렬 초기화. theta(V)시간.
			
			remain = N;		//topological sort를 하기위해 remain 값을 초기화.
			
			for(int i = 1; i<=N; i++)		//모든 vertex에 대해 DFS를 수행하면서 topological sort 진행
			{
				if(visited[i]==false)
				{
					topologicalDFS(i,visited,adjlist);
				}
			}
			//모든 vertex에 대해 DFS를 할때, 모든 vertex를 한번씩 체크하고, 한번씩 방문하고, edge를 2번씩 들여다보므로 총 theta(V+E)의 시간이 듬.
			//따라서 vertex를 topological sorting할때 총 theta(V+E)의 시간이 든다.
			
			
			for(int i=1;i<=N;i++)		// 1번정점을 시작으로 할떄,visited배열을 재사용해서 DAG에서 닿았는지를 체크함. Answer 배열도 초기화. 초기화에 상수시간이므로 theta(V)
			{
				visited[i] = false;  
				Answer[i-1] = 0;
			}
			
			visited[1] = true;
			
			for(int i=Vertexindex[1]; i<=N; i++)	//topological sorting 된 순서대로 정점을 방문하면서 각 노드의 값을 갱신함.
			{
				int now = sortedVertex[i];				//현재 노드의 값을 저장하는 value.
				for(int j=0;j<adjlist[now].size();j++)		//인접한 노드들에 대해 진행
				{
					int adjver = V[(int) adjlist[now].get(j)];				//인접한 노드의 번호
					int newweight = Answer[now-1] + W[(int) adjlist[now].get(j)];   //현재 노드에서 다음노드로 이동하는데 드는 비용
					if(visited[adjver] == false)			//들른적이 없는경우 초기화
					{
						visited[adjver] = true;
						Answer[adjver-1] = newweight;
					}
					else								    //들른적이 있는경우 필요에따라 값을 갱신
					{
						if(Answer[adjver-1]>newweight)		//만약 지금 있는 노드로부터 이동하는것이 더 가중치가 저렴할경우, 갱신해준다.
						{
							Answer[adjver-1] = newweight;
						}
					}
					
				}
			}
			// 최악의 경우 이 루프 전체에서, 모든 vertex에 대해(i-forloop), 모든 edge를 한번씩 탐색(j-forloop)하므로  총 O(V+E)의 시간이 든다.
			// 따라서 Answer를 구하는데 theta(V+E) + O(V+E) -> theta(V+E)의 시간이 든다.
			
			
			/////////////////////////////////////////////////////////////////////////////////////////////
			/*
			   이 부분에서 여러분의 알고리즘이 수행됩니다.
			   문제의 답을 계산하여 출력해야 할 전체 N-1개의 수를 Answer[0], Answer[1], ... , Answer[N-2]에 저장하는 것을 가정하였습니다.
			 */
			/////////////////////////////////////////////////////////////////////////////////////////////
			

			// output1.txt로 답안을 출력합니다. 문자 'X'를 출력하기 위해 필요에 따라 아래 코드를 수정하셔도 됩니다.
			pw.print("#" + test_case);
			for (int i = 1; i < N; i++) {			//answer[i]는 i+1번 노드의 가중치를 갖고있음. answer[0] (1번노드의 가중치)를 뺀 나머지 값이 필요하기 때문에 그걸 출력합니다. 
				if(visited[i+1])
				{
					pw.print(" " + Answer[i]);
				}
				else
				{
					pw.print(" " + "X");
				}
			}
			pw.println();
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

