import java.util.StringTokenizer;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.PrintWriter;
import java.util.ArrayList;

/*
   1. 아래와 같은 명령어를 입력하면 컴파일이 이루어져야 하며, Solution5 라는 이름의 클래스가 생성되어야 채점이 이루어집니다.
       javac Solution5.java -encoding UTF8


   2. 컴파일 후 아래와 같은 명령어를 입력했을 때 여러분의 프로그램이 정상적으로 출력파일 output5.txt 를 생성시켜야 채점이 이루어집니다.
       java Solution5

   - 제출하시는 소스코드의 인코딩이 UTF8 이어야 함에 유의 바랍니다.
   - 수행시간 측정을 위해 다음과 같이 time 명령어를 사용할 수 있습니다.
       time java Solution5
   - 일정 시간 초과시 프로그램을 강제 종료 시키기 위해 다음과 같이 timeout 명령어를 사용할 수 있습니다.
       timeout 0.5 java Solution5   // 0.5초 수행
       timeout 1 java Solution5     // 1초 수행
 */

class Solution5 {
	static final int max_n = 1000;

	static int n, H;
	static int[] h = new int[max_n], d = new int[max_n-1];
	static int Answer;

	public static void main(String[] args) throws Exception {
		/*
		   동일 폴더 내의 input5.txt 로부터 데이터를 읽어옵니다.
		   또한 동일 폴더 내의 output5.txt 로 정답을 출력합니다.
		 */
		long s = System.nanoTime();
		BufferedReader br = new BufferedReader(new FileReader("input5.txt"));
		StringTokenizer stk;
		PrintWriter pw = new PrintWriter("output5.txt");

		/*
		   10개의 테스트 케이스가 주어지므로, 각각을 처리합니다.
		 */
		for (int test_case = 1; test_case <= 10; test_case++) {
			/*
			   각 테스트 케이스를 표준 입력에서 읽어옵니다.
			   먼저 블록의 개수와 최대 높이를 각각 n, H에 읽어들입니다.
			   그리고 각 블록의 높이를 h[0], h[1], ... , h[n-1]에 읽어들입니다.
			   다음 각 블록에 파인 구멍의 깊이를 d[0], d[1], ... , d[n-2]에 읽어들입니다.
			 */
			stk = new StringTokenizer(br.readLine());
			n = Integer.parseInt(stk.nextToken()); H = Integer.parseInt(stk.nextToken());
			stk = new StringTokenizer(br.readLine());
			for (int i = 0; i < n; i++) {
				h[i] = Integer.parseInt(stk.nextToken());
			}
			stk = new StringTokenizer(br.readLine());
			for (int i = 0; i < n-1; i++) {
				d[i] = Integer.parseInt(stk.nextToken());
			}
			
			int cut = 1000000;		//cut = 백만
			
			int[] disarr = new int[H+1]; // 바로 전 블록이 구멍을 고려하지 않아도 되는 블록의 경우의 수를 담는 배열
			int[] conarr = new int[H+1]; // 바로 전 블록이 구멍을 고려해야하는 블록으로 쌓은 경우의 수를 담는 배열 (연속으로 쌓는경우)
			int[] temparr = new int[H+1]; // 값을 옮길때 사용하는 임시배열
			int height; // 블록의 높이를 저장할 값
			int diff; //블록의 구멍을 저장할 값
			int disnexth;	// 블록을 쌓은 후의 높이를 저장할 값
			int connexth;	// 블록을 쌓은 후의 높이를 저장할 값 (구멍 고려)
			// 반복문 내부의 최적화를 위해 선언문을 하나로 모았다.
			
			height = h[0];
			diff = 0;
			if(height<=H) conarr[height] += 1;		// 초기화 : 1번 블록을 쌓는다.
			
			for(int i=1;i<n;i++)			// 블록의 개수만큼
			{
				
				height = h[i];							// 블록의 높이
				diff = d[i-1];				// 블록의 구멍 (첫번째 블록의 경우는 구멍을 고려하지 않아도 된다.)
				
				for(int j=1;j<disarr.length;j++)		// 높이의 합이 j인 경우부터 하나씩 체크
				{
					/*
					int concurrnum = conarr[j];			// 높이가 j일때 다음 쌓을 블록이 연속한경우의 경우의수.
					int discurrnum = disarr[j];			// 높이가 j일때 다음 쌓을 블록이 연속하지 않은 경우의 수.
					*/
	
					disnexth = j + height;			// 블록을 쌓은 후의 높이
					connexth = j + height - diff;	// 블록을 쌓은 후의 높이 (구멍 고려)
					if(disnexth<=H) temparr[disnexth] = (temparr[disnexth] + disarr[j]) % cut;
					if(connexth<=H) temparr[connexth] = (temparr[connexth] + conarr[j]) % cut;
					// 임시 배열에 각 블록을 쌓은 결과를 저장. (i+1)번째 블록을 쌓았다.
					disarr[j] = (disarr[j] + conarr[j]) % cut;
					// 블록을 쌓지 않은 경우를 disarr에 넣어줌.
				} // 블록합의 최대 높이를 n이라하면 theta(n)시간이 걸린다.
				
				temparr[height] = (temparr[height] + 1) % cut; // 아무것도 없는 상태에서 블록을 집어넣는다. 
				//문제 조건상 블록의 높이가 높이제한보다 작을 수 없으므로 그냥 집어넣음. 최적화를 위해 블록 높이가 높이제한보다 작은지 보는 if문을 없앴다. 
					
				for(int j=1;j<conarr.length;j++)
				{
					conarr[j] = temparr[j];
					temparr[j] = 0;
				}		// temparr의 결과를 다음 conarr에 옮겨줌. 그리고 temparr를 초기화. theta(n)시간.
				
					
			}
		    // 블록의 개수를 m이라 하면 이 과정을 m번 반복하므로 총 theta(mn)의 시간이 걸린다.
			
			
			
			Answer = 0;
			for(int i=1;i<disarr.length;i++)
			{
				Answer = (Answer + disarr[i] + conarr[i]) % cut;
			}
			// 연산이 끝난후 모든 경우의 수를 더해준다. theta(n)시간.
			
			
			//따라서 이 알고리즘의 전체 시간복잡도는 theta(mn)이다. (m은 블록의 개수, n은 최대 블록높이의 합.)

			/////////////////////////////////////////////////////////////////////////////////////////////
			/*
			   이 부분에서 여러분의 알고리즘이 수행됩니다.
			   문제의 답을 계산하여 그 값을 Answer에 저장하는 것을 가정하였습니다.
			 */
			/////////////////////////////////////////////////////////////////////////////////////////////
			

			// output5.txt로 답안을 출력합니다.
			pw.println("#" + test_case + " " + Answer);
			/*
			   아래 코드를 수행하지 않으면 여러분의 프로그램이 제한 시간 초과로 강제 종료 되었을 때,
			   출력한 내용이 실제로 파일에 기록되지 않을 수 있습니다.
			   따라서 안전을 위해 반드시 flush() 를 수행하시기 바랍니다.
			 */
			pw.flush();
		}
		long e = System.nanoTime();
		
		System.out.println((e-s)/1000000000.0);
		
		br.close();
		pw.close();
	}
	
	
	
	
	
}

