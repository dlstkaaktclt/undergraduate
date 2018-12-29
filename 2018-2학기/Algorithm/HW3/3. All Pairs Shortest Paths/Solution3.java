import java.util.ArrayList;
import java.util.StringTokenizer;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.PrintWriter;

/*
   1. 아래와 같은 명령어를 입력하면 컴파일이 이루어져야 하며, Solution3 라는 이름의 클래스가 생성되어야 채점이 이루어집니다.
       javac Solution3.java -encoding UTF8

   2. 컴파일 후 아래와 같은 명령어를 입력했을 때 여러분의 프로그램이 정상적으로 출력파일 output3.txt 를 생성시켜야 채점이 이루어집니다.
       java Solution3

   - 제출하시는 소스코드의 인코딩이 UTF8 이어야 함에 유의 바랍니다.
   - 수행시간 측정을 위해 다음과 같이 time 명령어를 사용할 수 있습니다.
       time java Solution3
   - 일정 시간 초과시 프로그램을 강제 종료 시키기 위해 다음과 같이 timeout 명령어를 사용할 수 있습니다.
       timeout 0.5 java Solution3   // 0.5초 수행
       timeout 1 java Solution3     // 1초 수행
 */

class Solution3 {
	static final int MAX_N = 200;
	static final int MAX_E = 10000;

	static int N, E;
	static int[] U = new int[MAX_E], V = new int[MAX_E], W = new int[MAX_E];
	static int Answer;

	public static void main(String[] args) throws Exception {
		/*
		   동일 폴더 내의 input3.txt 로부터 데이터를 읽어옵니다.
		   또한 동일 폴더 내의 output3.txt 로 정답을 출력합니다.
		 */
		BufferedReader br = new BufferedReader(new FileReader("input3.txt"));
		StringTokenizer stk;
		PrintWriter pw = new PrintWriter("output3.txt");

		/*
		   10개의 테스트 케이스가 주어지므로, 각각을 처리합니다.
		 */
		for (int test_case = 1; test_case <= 10; test_case++) {
			/*
			   각 테스트 케이스를 표준 입력에서 읽어옵니다.
			   먼저 정점의 개수와 간선의 개수를 각각 N, E에 읽어들입니다.
			   그리고 각 i번째 간선의 양 끝점의 번호를 U[i], V[i]에 읽어들이고, i번째 간선의 가중치를 W[i]에 읽어들입니다. (0 ≤ i ≤ E-1, 1 ≤ U[i] ≤ N, 1 ≤ V[i] ≤ N)
			 */
			
			// 플로이드 - 워샬 알고리즘을 사용한다.
			stk = new StringTokenizer(br.readLine());
			N = Integer.parseInt(stk.nextToken()); E = Integer.parseInt(stk.nextToken());
			
			ArrayList adjlist[] = new ArrayList[N+1];		// 인접리스트 초기화
			for(int i = 1; i <= N; i++)
			{
				adjlist[i] = new ArrayList();
			}
			
			stk = new StringTokenizer(br.readLine());
			for (int i = 0; i < E; i++) {
				U[i] = Integer.parseInt(stk.nextToken());
				V[i] = Integer.parseInt(stk.nextToken());
				W[i] = Integer.parseInt(stk.nextToken());
				
				adjlist[U[i]].add(i);		//adjlist에 i번째 edge가 연결되어있다는것을 표기.
			} // 초기화에 총 theta(N+E)의 시간이 걸림.
			
			int sol[][][] = new int[N+1][N+1][N+1];			//최단경로를 담을 배열. sol[k][i][j]는 vertex set {1~k}를 거쳐서 i->j로 갈수있는 최단경로.
			
			for(int i=1;i<=N;i++)			
			{
				for(int j=1;j<=N;j++)		//N번
				{
					sol[0][i][j] = 20000000;		//2000만. 200만보다는 크게, 더할때 오버플로우가 나지 않게 세팅.
				}
				sol[0][i][i] = 0;			// 자기로 가는것은 0으로 초기화.
				for(int j=0;j<adjlist[i].size();j++)	//최대 N번. 인접한곳과 연결된곳은 값을 수정.
				{
					int adjvertex = V[(int) adjlist[i].get(j)];
					int weight = W[(int) adjlist[i].get(j)];
					sol[0][i][adjvertex] = weight;
				}
			}
			// 정답 배열 초기화. sol[0][i][j]를 모두 구한다. 그래프가 모두 연결되었음이 가정되었고, 음의 사이클이 없으므로 i->j로 가는 
			// 최단경로는 vertex가 N개일때 최대 N-1개의 엣지만으로 구성된다.
			// (만약 N개 이상의 엣지가 사용되면 사이클이 생기는데, 음의 사이클이 없으므로 사이클을 이루는 엣지는 전체 경로를 짧게만드는데 도움이 되지 않음. 따라서 그 사이클을 뺀 경로가 최단경로.)
			// 이므로 최단경로가 가질 수 있는 최대값은 vertex가 최대 200개, 간선의 값이 최대 1000이므로 2000000을 넘지 않는다.
			// 따라서 2000000 이상의 값을 엣지에 넣어놓으면 그 엣지는 끊어졌다고 가정할 수 있다. 초기화를 수행하는데는 theta(n^2)의 시간이 걸린다.
			
			
			
			for(int k=1;k<=N;k++)		//vertex set은 1부터 시작.
			{
				for(int i=1;i<=N;i++)
				{
					for(int j=1;j<=N;j++)
					{
						int minimum = (sol[k-1][i][j] < (sol[k-1][i][k] + sol[k-1][k][j])) ? sol[k-1][i][j] : (sol[k-1][i][k] + sol[k-1][k][j]);
						sol[k][i][j] = minimum;
					}
				}
			}
			// minimum을 구하는데 theta(1)의 시간이 걸리고, k=1~n, i=1~n, j=1~n의 범위에서 수행되므로 총 theta(n^3)의 시간이 걸린다.
			
			
			// 값을 구할때, 모든 그래프가 연결되었다고 가정하였으므로 초기화할때 넣어줬던 2000만이 결과에 영향을 줄일은 없다.
			
			Answer = 0;
			for(int i=1;i<=N;i++)
			{
				for(int j=1;j<=N;j++)
				{
					Answer += sol[N][i][j]; 
				}
			}
			// answer를 구하는데 theta(n^2)의 시간이걸림.
			
			// 따라서 총 theta(n^3)의 시간이 걸린다. (N : vertex의 개수)

			/////////////////////////////////////////////////////////////////////////////////////////////
			/*
			   이 부분에서 여러분의 알고리즘이 수행됩니다.
			   문제의 답을 계산하여 그 값을 Answer에 저장하는 것을 가정하였습니다.
			 */
			/////////////////////////////////////////////////////////////////////////////////////////////



			// output3.txt로 답안을 출력합니다.
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

